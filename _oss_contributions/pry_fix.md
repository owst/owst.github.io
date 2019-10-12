---
languages:
    - Ruby
weight: 20
---

I [diagnosed][issue] and [fixed][pr] a crash-causing bug in the [pry][pry] Ruby
REPL when encountering NULL bytes, which included some MRI
[source-diving][source_dive].

[pr]:https://github.com/pry/pry/pull/1790
[issue]:https://github.com/pry/pry/issues/1789
[pry]:https://pryrepl.org/
[source_dive]:https://github.com/pry/pry/issues/1789#issuecomment-428708235
