---
title: 'GSoC: Darcs Bridge – Week 6/7/8'
published: false
---
Another very late blog post, argh! But, good news - I've completed several of
the targets of the project:

- Branches that the bridge manages can now be tracked/untracked and listed.
- Incremental imports/exports with branches now work correctly (bar the merging
  issues).
- Git patches can be applied to a Darcs repo.

We can now tell the bridge to track new Git/Darcs branches, such that they are
synced when commits/patches are exported/imported and can also tell the bridge
to no longer track branches, so they will no longer be exported/imported.

```console
# Init a test repo.
$ darcs init --repo foo_project 
$ cd foo_project 
$ echo 1 > a 
$ darcs add a 
$ darcs rec -am 'add a'
Finished recording patch 'add a'
$ cd ..

# Create a branch of the repo.
$ darcs get foo_project foo_project_branch1
Copying patches, to get lazy repository hit ctrl-C...
Finished getting.

# Init a bridge from the repo.
$ darcs-fastconvert create-bridge foo_project
Identified darcs repo at /tmp/throwaways/foo_project
Cloning source repo from /tmp/throwaways/foo_project to /tmp/throwaways/foo_project_bridge/foo_project
Initialised target git repo at /tmp/throwaways/foo_project_bridge/foo_project_git
Created .darcs_bridge in /tmp/throwaways/foo_project_bridge
Wrote new marks files.
Wrote hook.
Wired up hook in both repos. Now syncing from darcs
Copying old sourcemarks: /tmp/throwaways/foo_project_bridge/.darcs_bridge/marks/darcs_export_marks
Doing export.
Doing import.
Copying old targetmarks: /tmp/throwaways/foo_project_bridge/.darcs_bridge/marks/git_export_marks
Doing mark update export.
Diffing marks.
1 marks to append.
Import marks updated.
Bridge successfully synced.

# Start tracking the branch we created.
$ darcs-fastconvert branch track foo_project_bridge/ foo_project_branch1/
Copying old sourcemarks: /tmp/throwaways/foo_project_bridge/.darcs_bridge/marks/darcs_export_marks
Doing export.
Doing import.
Copying old targetmarks: /tmp/throwaways/foo_project_bridge/.darcs_bridge/marks/git_export_marks
Doing mark update export.
Diffing marks.
0 marks to append.
Import marks updated.
Bridge successfully synced.

# Print a list of all tracked branches.
$ darcs-fastconvert branch list foo_project_bridge/
Tracked branches:
Name: master ~ Darcs path: foo_project_bridge/foo_project
Name: foo_project_branch1 ~ Darcs path: foo_project_bridge/foo_project_branch1

# Show that the branch was correctly imported into git.
$ (cd foo_project_bridge/foo_project_git/  && git log -p foo_project_branch1)
commit 893e08f44b0de658e00a49bc61a51c6a6621d59e
Author: Owen Stephens <darcs@owenstephens.co.uk>
Date:   Fri Jul 15 11:53:53 2011 +0000

    add a

diff --git a/a b/a
new file mode 100644
index 0000000..d00491f
--- /dev/null
+++ b/a
@@ -0,0 +1 @@
+1
```

Another main piece of work that I've completed is to properly handle
incremental import/export with branches.  Since Darcs uses a patch-based
representation, and the fast-import format uses a snapshot-based
representation, we have to jump through some hoops to properly import/export
the state requried.

To demonstrate, consider the Darcs history with 2 patches:

```text
Fri Jul 15 13:11:04 BST 2011  Owen Stephens <darcs@owenstephens.co.uk>
  * Amend shopping list, :-(
    hunk ./shopping_list.txt 4
    +Apples
    +Pears

Fri Jul 15 13:10:44 BST 2011  Owen Stephens <darcs@owenstephens.co.uk>
  * Create shopping list.
    addfile ./shopping_list.txt
    hunk ./shopping_list.txt 1
    +Beer
    +Pizza
    +Chips
```


To output this repository in the fast-import format, we need to recreate each
intermediate state of the repository, and list the file contents at that state.
This is easy - we simply apply each patch and dump the affected files'
contents, one after another. At each state, we save the pristine hash (a hash
that identifies the pristine state of the repository) and the inventory (the
list of patches that have been applied to create the pristine), which allows us
to 'reset' ourselves to a previous state at a later point in time, by restoring
the pristine/inventory.

Imagine another repository, which has a different final patch:

```text
Fri Jul 15 13:21:06 BST 2011  Owen Stephens <darcs@owenstephens.co.uk>
  * More treats needed!
    hunk ./shopping_list.txt 4
    +Cake
    +Cider

Fri Jul 15 13:10:44 BST 2011  Owen Stephens <darcs@owenstephens.co.uk>
  * Create shopping list.
    addfile ./shopping_list.txt
    hunk ./shopping_list.txt 1
    +Beer
    +Pizza
    +Chips
```

We want to output the following 2 repos:
original : AB
better   : AC

the snapshot-based history graph would look something like: 

```text
    /-- B 
A --
    \-- C
```

We export A, since it is shared by both branches, and then export B. However,
we need to reset ourselves back to the state before we applied B - we do so by
restoring the pristine/inventory that we stored when we first applied A.  Once
we have output C, we throw away the state we have generated/saved, since we
have no further need for it, and it could potentially consume a large amount of
space.

The fly in the ointment is the need to handle incremental imports/exports.
Incremental imports/exports are supported by the fast-import format, through
the use of "marks" files. Marks files contain a list of mark->patch_hash
mappings[1], where a mark is an integer that is output along with each
commit/patch in the format stream. In our example above, A would have mark 1, B
2 and C 3, along with their corresponding patch-hashes (and branch names) in
the marks file.

Imagine that we exported the repository incrementally: we would first export A
and B, and then, in a separate stream, C. The problem is the fact that to
export C, we need the state, as it was after exporting A (remember we've thrown
it away after exporting B, to save space). The solution is simple, but fairly
inelegant - we simply run through *all* the thus-far exported patches, and
recreate the state for each, which is, as expected, expensive.

On the import side, it's a little more difficult - consider reading the
incremental stream containing just C - it'll contain a single commit that looks
something like:

```text
commit refs/heads/demo2
mark :3
committer Owen Stephens <darcs@owenstephens.co.uk> 1310732466 +0000
data 20
More treats needed!
from :1
M 100644 inline shopping_list.txt
data 28
Beer
Pizza
Chips
Cake
Cider
```

This commit object names the branch on which it should be recreated
("refs/heads/demo2") the mark for the commit, the commiter, commit message,
ancestor (from) commit mark and the commit modifications.

Note the line "from :1" - this line tells the importer that this commit should
be based on the state as it was at mark 1 - i.e. commit A. We need to recreate
that state - as mentioned earlier, as we import each commit, we stash the state
for later use; however, as in export, we throw away this state, once we've
finished a particular import stream. To recreate the state in a later stream,
we take the ancestor mark (the from mark) and read the corresponding
branch-name and patch-hash from the marks file. We then issue an internal
command that performs the equivalent of "darcs get branch-name
temporary_location --to-match = 'hash: PATCH_HASH'"; once we have get'd a new
copy of the repo at the required state, we simply read the pristine and
(entire) inventory, which allows us to reset our current state to that of the
marked patch.

The final piece of work completed is being able to directly apply git-formatted
patches to a Darcs repository:

```console
$ darcs init
$ git init -q
$ echo -e '1\n2\n3' > a
$ git add a && git commit -m 'Add a'

[master (root-commit) 564e9e2] Add a
 1 files changed, 3 insertions(+), 0 deletions(-)
 create mode 100644 a

$ echo -e '4\n5' > a
$ echo -e 'a\nb\nc' > b
$ git add b 
$ git add a
$ git commit -m 'Modify a and add b'

[master 411e599] Modify a and add b
 2 files changed, 5 insertions(+), 3 deletions(-)
 create mode 100644 b

# Revert changes, so we can apply the patch to the Darcs repo.
$ rm a b

# Create a Git patch of all the repos commits.
$ git format-patch --all --stdout > git.patch

# Apply the Git patch to Darcs.
$ darcs-fastconvert apply-patch . git.patch
Attempting to parse input.
Successfully parsed 2 patches.
Attempting to apply patches.
Applying patch 1 of 2: Add a
Applying patch 2 of 2: Modify a and add b
Succesfully applied patches.

Git patches contain a SHA1 hash of each affected file, which we can use to
verify that the files are in the same state that they were in Git, prior to the
commit. The patch-apply code computes the target files' SHA1 hashes (Git
computes the SHA1 of the string "blob LEN\0CONTENT" of a file) to detect if the
files are in the same state as in the Git patch. If the hashes differ, the user
is prompted to apply anyway, with any non-applying patches being completely
rolled back (the unrecorded state of the repository is also unaffected).

```console
# Create another Git commit.
$ echo -e 'd\ne' >> b
$ git add b && git commit -m 'Modify b'

[master 59046a6] Modify b
 1 files changed, 2 insertions(+), 0 deletions(-)

# Revert, so Darcs can apply the patch.
$ darcs rev -a
Finished reverting.

# Change b, so the expected hash doesn't match, but the patch will still apply cleanly.
$ sed -i 's/a/d/' b

$ git format-patch HEAD~1 --stdout > git.patch
$ darcs-fastconvert apply-patch . git.patch
Attempting to parse input.
Successfully parsed 1 patches.
Attempting to apply patches.
Applying patch 1 of 1: Modify b
WARNING: Hash of b does not match patch
No changes will be recorded, if the patch does not apply.
Continue anyway? [yn]y
Succesfully applied patches.
```

This means someone can make their own local git clone of a Darcs repo, and send
patches to the Darcs-repo owner, who will be able to directly apply them.

Still outstanding in the next week:

- Apply Darcs patches to a Git repo.
- Merge detection - we still need to try to detect and output clean Darcs
  merges, else we'll lose them when exporting to Git.
- Performance - the performance of import is somewhat slow - we need to work
  out where and why it is performing badly.

[1] Darcs marks also contain the branch name that the given patch is part of -
since Darcs doesn't yet natively support branches, we have to provide this
information manually.
