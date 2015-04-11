---
title: 'GSoC: Darcs Bridge – Detecting Merges'
published: false
---
As I mentioned in my previous post, a problem with exporting multiple Darcs
branches is that the patch-based model of Darcs makes it particularly difficult
to detect merges of two branches.

We want to be able to detect merges, and export them in the fast-import data
stream, for import by Git or other fast-import aware VCSs, since otherwise, it
would appear as if two branches never converged, even if they had been merged
in Darcs.

As an example, say we have two Darcs repos:
master : ABCD'E'
branch1: ADE

where D' and E' are the commuted versions of D and E, having been pulled into
master from branch1. We'd like to see this history in git:

```text

  _ B _ C _ M
 /          / 
A _ D _ E _/
```


i.e. a merge of ABC and ADE; Git makes these merge points explicit (a commit
with >1 parent SHA1s is a merge commit), whereas Darcs does not.

So, the question is, how do we detect merges that are clean (i.e. no conflicts)
and non-clean (with conflicts)?

First, a discussion on re-ordering and selective-pulling:
To Darcs, the repositories containing {ABC} and {ACB} are equal and are already
sharing all their patches. Git (and thus, the fast-import format) does not see
it this way - branches only share patches if the SHA1s of those patches match.
This means that we can only treat Darcs branches as equal if they share their
patches *and* ordering.  Darcs always provides the user with the ability to
cherry-pick patches - selectively choosing the patches to operate on. If some
patches are not pulled in, we cannot treat the resulting repository as a merge
of branches.

So the resulting constraints are: to use the darcs-bridge effectively, patches
should not be unpulled or otherwise re-ordered, and pulls between branches
should always use the "--all|-a" option to pull in all changes.

Finally, how do we attempt to find the merge points? What we essentially need
to know is where does a merge start in a sequence of patches, and where do the
merged patches come from? So in our merged example, we'd need to know that D'
is the start of a merge, and the patches are coming from branch1. Knowing this
information allows us to export the patches of branch1, and then create a merge
commit between the state of branch1 and the state of master.

One potential way of doing this is to create a tag, immediately before pulling
a branch, and after the pull has completed. In our example, we'd get:

master: {A,B,C,T1,D',E',T1}
branch1: {A,D,E}

To export these branches, we follow these steps, starting with master:

1. While the head-patches of all branches are equal, output a copy of that
   patch and move the 'reset point' of the branches to the state after
   exporting the patch.
1. When we find a non-equal patch, save the current state as the branches reset
   point. And keep exporting the current branch.
1. When we hit a merge tag (identified by a unique tag message), we read the
   patches in until we hit the corresponding merge tag (with equal message). We
   now have a set of merged patches, and need to find the origin branch.
1. For each branch, try and match the list of merged patches, if we find a
   match, we can export that branches patches (having reset the current state
   to that of the branch reset-state), if we don't find the patches, something
   has been changed, so the only thing we can do is just output the patches as
   they are on the merge target branch (since we can't find the source branch).

The next question is how we obtain these "merge marker" tags, which is what I
am currently pondering... watch this space!
