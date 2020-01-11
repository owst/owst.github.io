---
languages:
    - C
weight: 40
---

[I tracked down and proposed a fix][ruby_bug] for a segfault in the MRI Ruby
interpreter when using forked processes and the default psuedo random number
generator, as [triggered by the Faker][faker_issue] test data library.

[faker_issue]: https://github.com/stympy/faker/issues/982#issuecomment-317390797
[ruby_bug]: https://bugs.ruby-lang.org/issues/13753
