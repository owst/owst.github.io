---
languages:
    - Ruby
---

I improved several linters of the popular [Rubocop][rubocop] code checking
tool:

1. [Avoiding nested method definition warnings][2708]
2. [Treating loops as conditionally executed to avoid potentially-incorrect
  useless variable assignment warnings][2702],
3. [Ensuring colons in optional keyword arguments are followed by
   spaces][2994],
4. [Removing duplicate warnings regarding spacing around general and
   ternary operators][3011],
5. [Catching case statements with empty conditions][3044],
6. [Improving useless access modifier warnings][3060],
7. [Recognising useless array splats][3072]

[rubocop]: http://batsov.com/rubocop/
[2708]: https://github.com/bbatsov/rubocop/pull/2708
[2702]: https://github.com/bbatsov/rubocop/pull/2702
[2994]: https://github.com/bbatsov/rubocop/pull/2994
[3011]: https://github.com/bbatsov/rubocop/pull/3011
[3044]: https://github.com/bbatsov/rubocop/pull/3044
[3060]: https://github.com/bbatsov/rubocop/pull/3060
[3072]: https://github.com/bbatsov/rubocop/pull/3072
