---
title: "Open Source"
weight: 5
---

A list of OSS contributions I've made; some are small, but it's nice to
be able to remember them:

- *Clojure, Elixir, NodeJS, PHP, Python, Ruby*: [I fixed a bug, normalised
  multi-language implementations and improved test coverage][mailchecker-pr] of
  the [Mailchecker][mailchecker] project that rejects invalid/throwaway email
  addresses,
- *Ruby*: I improved two linters of the popular [Rubocop][rubocop] code
  checking tool:
    1. [avoiding nested method definition warnings][rubocop-nested-method-defs]
    2. [treating loops as conditionally executed to avoid potentially-incorrect
      useless variable assignment warnings][rubocop-loop-assignments],
- *Haskell*: I was a core member of the [Darcs][darcs] VCS team, contributing
  [many patches][darcs-patches],
- *Python*: I added support for IPv6 extension headers to the [dpkt][dpkt]
  library, the [google code issue][dpkt-headers] no longer exists, but the
  changelog (now on GitHub) [bears my name][dpkt-changelog],
- *Haskell*: [I improved certain error messages][cabal-errs] of the
  [Cabal][cabal] packaging tool,
- *Haskell*: [I added an instance][cond-pr] to the Boolean algebra library
  [Cond][cond] to recognise that Boolean algebras are closed under pairing.

[darcs]: http://www.darcs.net
[darcs-patches]: http://bugs.darcs.net/patch?@sort0=activity&@sortdir0=on&@group=status&@filter=status,creator&@columns=id,activity,title,assignedto,status,&status=-1,16,17,1,2,3,13,14,15,12&creator=2035
[dpkt]: https://github.com/kbandla/dpkt
[dpkt-headers]: http://code.google.com/p/dpkt/issues/detail?id=31
[dpkt-changelog]: https://github.com/kbandla/dpkt/blob/master/CHANGES#L12
[cond]: https://hackage.haskell.org/package/cond
[cond-pr]: https://github.com/kallisti-dev/cond/pull/5
[cabal]: https://www.haskell.org/cabal/
[cabal-errs]: https://github.com/haskell/cabal/pull/1824
[rubocop]: http://batsov.com/rubocop/
[rubocop-nested-method-defs]: https://github.com/bbatsov/rubocop/pull/2708
[rubocop-loop-assignments]: https://github.com/bbatsov/rubocop/pull/2702
[mailchecker]: https://github.com/FGRibreau/mailchecker
[mailchecker-pr]: https://github.com/FGRibreau/mailchecker/pull/54
