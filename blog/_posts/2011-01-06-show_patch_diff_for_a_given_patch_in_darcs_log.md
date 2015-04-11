---
title: Show patch diff for a given patch in darcs log
---
When using [darcs](http://darcs.net/) you may want to be able to pick a patch
from the log, and see its changes. How to achieve this wasn't particularly
obvious to my git-oriented mind, so I thought I'd blog the answer.

Say we want to see the patch that is the third "away" from our current state.
In darcs, we use a 1-indexed numbering scheme so the patch is #4 (since the
current state is 1). To show the details of the patch, issue the simple
command:

```bash
darcs log -n 4
```

If you're a [git](http://git-scm.com/) user, the equivalent revision is
`HEAD~3` (the third-generation grandparent of the current branch). To show the
patch details in a similar fashion to darcs, execute:

```bash
git log -1 HEAD~3
```

To show a unified diff of the changes introduced in the revision/patch, the
darcs command changes slightly to this:

```bash
darcs diff -u -n 4
```

whereas the git command changes to:

```bash
git log -1 -u HEAD~3
```

*N.B.* It is also possible to use darcs' interactive mode to view patch
differences (including the ability to pick which patches to view, and which to
skip). To do this, issue the command:

```bash
darcs log -i
```

and then show the chosen patch diffs by pressing the `p` key.
