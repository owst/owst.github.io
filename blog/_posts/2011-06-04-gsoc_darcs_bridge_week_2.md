---
title: 'GSoC: Darcs Bridge - Week 2'
published: false
---
So, it's the end of the 2nd week of GSoC, but only my first full week of work,
due to my uni finals; Thankfully, they're over and GSoC is going well! I didn't
write a week 1 post, since I'd only done < 2 days of work, but this week I've
got some good things to discuss.

This week, I've created a working (but not stringently tested, yet!) automatic
Darcs<->Git bridge, by extending darcs-fastconvert. The bridge creates a Git
clone of an input Darcs repository (vice versa for a Git input repo), using the
fast-import data format to import the data from Darcs. The bridge inserts a
"hook" into both repos, (pre-receive for Git, and pre-apply for Darcs) that
ensures that the bridge is synced, before allowing new patches to be pushed. If
the bridge was out-of-sync, the new commits will be imported and the push/apply
will be rejected; the user should then pull in the imported changes and resolve
any conflicts locally. We use a mutex to disallow concurrent pushes to the two
repos.

If you'd like to test the bridge, you can get a copy of my darcsden repo. The
bridge can be created and tested as follows:

```bash
darcs get http://darcsden.com/owst/darcs-fastconvert-gsoc
cabal configure; cabal build; cabal install
cd DIR_CONTAINING_REPO
darcs-fastconvert create-bridge --input-repo=REPO_DIR
```

a directory named REPO_DIR_bridge should have been created, with a clone of the
input repo, and a Git copy. These repos should be used as the master repos, and
should be pushed to, not edited directly (otherwise, the bridge-syncing
commands won't run).

The current tool has limitations, particularly with regard to branches in Darcs
and Git, but on simple, linear history repos, the bridge should have no
problems. 

Some TODOs, for the bridge:

- Improve the help (possibly by modifying cmdlib, the command-line argument
  parser being used), particularly removing flags for mandatory parameters.
- Create a typeclass for the monad that the "export" command runs in, to allow
  easy redirection of output. Currently I have to shell-out to my own
  executable, due to the design of darcs-fastconvert, when I want to internally
  run an export/import command. This sucks (but at least the hack works as
  is!), and I will need to do some re-engineering to fix it.
- Create some tests! I need to create some simple shell-tests that will ensure
  I don't introduce regression errors, when adding features to the bridge.


At the end of the week, I started work on adding Rename/Move handling to the
import mechanism of darcs-fastconvert. Git is able to infer moves/copies, using
the -C and -M options to git-fast-export, but darcs-fastconvert cannot
currently import them. Adding this behaviour will reduce the likelihood of
loosing information, if a previously converted repo with moves/renames was
converted back to its original format.

My next task is to implement simple multi-head importing - currently
darcs-fastconvert linearises Git repos with multiple branches, usually leading
to "strange" patch contents. I will map Git branches to multiple Darcs repos
(the method of branching in Darcs). One particularly tricky problem to solve is
that of creating "good" Darcs patches, from a Git merge commit.
