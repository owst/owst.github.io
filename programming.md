---
title: "Programming"
weight: 3
---
A list of code projects/snippets that I want to show the world:

{% include page_posts.md %}

A list of OSS contributions I've made; some are small, but it's nice to
be able to remember them:

- I was a core member of the [Darcs][darcs] VCS team, contributing [many patches][darcs-patches]
- I added support for IPv6 extension headers to the [dpkt][dpkt] Python
  library, in the [google code issue][dpkt-headers]
- I improved certain error messages of the Haskell packaging tool
  cabal, [the merged pull request is on github][cabal-errs]
- Boolean algebras are closed under pairing; I added an instance to a 
  Haskell Boolean algebra library cond to recognise this [on github][cond]

[darcs]: http://www.darcs.net
[darcs-patches]: http://bugs.darcs.net/patch?@sort0=activity&@sortdir0=on&@group=status&@filter=status,creator&@columns=id,activity,title,assignedto,status,&status=-1,16,17,1,2,3,13,14,15,12&creator=2035
[dpkt]: http://code.google.com/p/dpkt
[dpkt-headers]: http://code.google.com/p/dpkt/issues/detail?id=31
[cond]: https://github.com/kallisti-dev/cond/commit/aeb9313babef18aab15e7771baae795cc7a1d5bb
[cabal-errs]: https://github.com/haskell/cabal/pull/1824 
