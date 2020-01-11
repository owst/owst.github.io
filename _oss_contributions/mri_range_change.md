---
languages:
    - C
    - Ruby
weight: 20
---

I [extended the Range#cover? method][commit] in the MRI Ruby standard library
to accept other `Range` objects to allow easy checking for
[sub-ranges][ruby_bug], as demonstrated in the [BigBinary blog][blog]
that introduces new features coming in Ruby 2.6. Later, [I fixed a bug][rails]
in the Rails implementation of the same feature.

[commit]: https://github.com/ruby/ruby/commit/9ca738927293df1c7a2a1ed7e2d6cf89527b5438
[ruby_bug]: https://bugs.ruby-lang.org/issues/14473
[blog]: https://blog.bigbinary.com/2018/10/24/ruby-2-6-range-cover-now-accepts-range-object.html
[rails]: https://github.com/rails/rails/pull/35776
