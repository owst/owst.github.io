---
title: 'GSoC: Darcs Bridge - Week 0'
published: false
---
This week I've been participating in some productive revision-procrastination,
by starting work on my [GSoC project - Darcs
Bridge](http://wiki.darcs.net/DarcsBridge); I still have university exams until
next week, so wanted to make sure I lost no GSoC time (and besides, Haskell
hacking is much more fun than revising Elliptic curves!).


- First up, I created a darcsden repository, to host my work-in-progress over
  the summer,  [here](http://darcsden.com/owst/darcs-fastconvert-gsoc).
    
- To kick things off code-wise, I wanted to make sure I could build
  [darcs-fastconvert](http://hackage.haskell.org/package/darcs-fastconvert) ,
  mornfall's existing method of converting to/from darcs/git, using the
  ["fast-import" de facto
  standard](http://www.kernel.org/pub/software/scm/git/docs/git-fast-import.html#_input_format).
  This meant updating the code, to build against the latest dev-version of
  Darcs 2.7.3 and GHC 7.
    
- I found, [and fixed](http://bugs.darcs.net/patch609) a bug in Darcs itself,
  which manifested itself when attempting to add non-existent files within a
  newly added folder within a repo.
    
Ensuring darcs-fastconvert compiled was a great first challenge for my project;
fixing the new typing requirements forced me to understand how many of Darcs
low-level concepts were implemented, particularly how types are used to
represent the contexts of a patch.

Ealier today, my mentor Ganesh and I had an online chat about ideas for the
project:

- We want conversion between darcs<->git to be as seamless as possible - a
  "sync" command.
        
- Darcs-fastconvert can do this to some extent, but requires manually managing
  "marks" files (for git and darcs), to persist the state of the conversion.
  These are somewhat fragile and prone to errors (and also make the
  export/import command invocations more noisy than they should be).

- We can "shell-out" to git, rather than relying on the user to manage the git
  side of the conversion. This also allows us to easily get hold of data such
  as the git commit ids (something that is not exported in the fast-import
  data-format.) - this'll allow us hopefully to better manage multi-head repos.

- We can assume that the bridge will be responsible for "managing" the darcs
  and git repositories; particularly, we envisage a "bridge lock" that will
  allow us to ensure that users cannot commit to the darcs and git repositories
  simultaneously - the pre-commit hooks in both git and darcs shall fail, if
  this lock is active. (We initially thought we could use the separate darcs
  and git locks, but this could well lead to lock ordering or race problems.)

- We can use git "hooks" to ensure that a darcs and git repository are in sync,
  before new commits can be pushed to the git repo.

That's it for now, but I'll make sure to keep this blog updated, as my work
progresses.
