---
title: 'GSoC: Darcs Bridge - Week 4/5'
published: false
---
I've had a fairly busy couple of weeks outside of GSoC, so I didn't have a
particularly interesting blogpost to make last week. That said, the bridge is
coming along nicely; we are now able to export Darcs branches into the
fast-export format (other than a TODO on detecting merges - more on that
later).

Some interesting topics that have come up recently:

Prefix sharing of Darcs branches when exporting - given branches ABCD and ABCE
we can "share" the patches A,B and C between the branches rather than simply
exporting ABCD and A'B'C'E, which would lose the common history of the two
branches. The current implementation exports the longest prefix of patches
between branches and then (to use the Git terminology) "rebases" any extra
patches on top. E.g. branches ABCD and ABDE will be exported as ABCD and ABD'E
(N.B. that D and D' are not the same). The current behaviour is somewhat a
"best-effort" (it has some complicated "reproducibility" issues) but after a
long discussion with Ganesh and Petr, a better approach wasn't found, so for
now, it is how it is.

Encoding replace patches (and other incompatible patch-types) is tricky. The
fast-export stream format simply stores file contents at each commit (just as
Git does internally), which is fine for exporting patches once - we just apply
the Darcs patches in turn, listing the changed files in full for each
patch/commit. However, a property we are keen to keep with the Bridge is that
of reproducability - multiple exports or repeated import/exporting should yield
the same changes. For example, if a Darcs replace patch was exported to Git,
and then the Git repo exported back into Darcs, we'd like to be able to recover
the same replace patch (rather than a large hunk patch).  To illustrate,
imagine the Darcs patch: [hunk file1 "foo\nfoo\nfoo" 1, replace file1 foo bar]
that adds some foos to file1 and then replaces foos with bars in file1. It is
important to know exactly where the "replace" was in the sequence of low-level
patches - if we don't know the position we will create the wrong patch when
re-importing (e.g. the newly added foos won't be changed to bars, if we place
the "replace" before the hunk). It is difficult to encode positions other than
"first" or "last", since we are unable to easily represent the intermediate
states in Git (to ensure that the states are re-exported later), so for now,
these changes will only be handled if they are first or last in a patch. N.B.
the only way to force a replace into the middle of a sequence is by using
amend-record, so the impact of this decision is *somewhat* limited.

Upcoming TODOs:

- Add branches to the bridge commands (add, rm, list branches etc.) - since we
  now support multi-head import/export on both sides, these commands will be
  very useful.  - Detecting, and making explicit in the fast-convert stream,
  merges of Darcs branches. Currently, re-exported Git merges are lost, since
  they are not detected on the Darcs side.
- Performance (import, especially is sometimes slow).
- Perhaps a way of showing progress without piping into git fast-import?
  Currently, bridge progress is mostly ignored.  - Accepting foreign
  patch-formats e.g. be able to apply emailed Darcs patches to a Git repo and
  vice-versa?
