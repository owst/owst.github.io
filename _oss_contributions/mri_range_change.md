---
languages:
    - C
weight: 1
---

I [extended the Range#cover? method][commit] in the MRI Ruby standard library
to accept other `Range` objects to allow easy checking for
[sub-ranges][ruby_bug].

[commit]: https://github.com/ruby/ruby/commit/9ca738927293df1c7a2a1ed7e2d6cf89527b5438
[ruby_bug]: https://bugs.ruby-lang.org/issues/14473
