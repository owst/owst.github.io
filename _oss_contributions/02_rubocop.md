---
languages:
    - Ruby
---

I improved linters of the popular [Rubocop][rubocop] code checking tool:

1. [avoiding nested method definition warnings][rubocop-nested-method-defs]
2. [treating loops as conditionally executed to avoid potentially-incorrect
  useless variable assignment warnings][rubocop-loop-assignments],
3. [ensure colons in optional keyword arguments are followed by
   spaces][rubocop-optkwarg-spaces],
4. [removing duplicate warnings regarding spacing around general and
   ternary operators][rubocop-duplicates],

[rubocop]: http://batsov.com/rubocop/
[rubocop-nested-method-defs]: https://github.com/bbatsov/rubocop/pull/2708
[rubocop-loop-assignments]: https://github.com/bbatsov/rubocop/pull/2702
[rubocop-optkwarg-spaces]: https://github.com/bbatsov/rubocop/pull/2994
[rubocop-duplicates]: https://github.com/bbatsov/rubocop/pull/3011
