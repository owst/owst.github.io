---
title: Nickel, an exercise in JIT evaluation
---

As I hinted in my [previous post][blog_post], I have (very) slowly been working
on implementing a simple [JIT][jit]-based evaluator for a toy language, which
I've called [Nickel][nickel]. Nickel, which I've now released on GitHub, uses
the [LLVM][llvm] C library to leverage the power of the LLVM optimising
compiler.

For full details, see the [repo on GitHub][nickel], but I'll try in this post
to give a brief overview. Consider the following Nickel program[^1], which computes
and prints a numeric value:

```ruby
# Saved to input.nkl...

def g(x)
  9 * x
end

def f(x, y)
  (57005 << x) + g(y + 1)
end

puts f(16, 5430)
```

We can evaluate this program using the `nickel` evaluator in simple interpreter
mode:

```bash
$ ./nickel --interpreter < input.nkl
3735928559
```

However, the particular value printed will be more familiar with a hex
representation:

```bash
$ printf "0x%x\n" $(./nickel --interpreter < input.nkl)
0xdeadbeef
```

A JIT evaluator performs compilation during execution - essentially generating
code to execute at run-time. We can witness the generated code by using a
debugger such as [`lldb`][lldb] to break at the point at which we [obtain a
pointer to the dynamically generated code][breakpoint]:

```c
int (*func)(void) = (int (*)(void))LLVMGetFunctionAddress(engine, "__anon_tl"); //<=
func();
```

Let's run `lldb` and break before we call `func`:

```
$ lldb ./nickel -- --jit
[...]
(lldb) breakpoint set --file jit.c --line 284
[...]
(lldb) process launch -i input.nkl
[...]
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x00000001000035f9 nickel`jit(p=0x0000000101a08940) at jit.c:284
   281 	    }
   282
   283 	    int (*func)(void) = (int (*)(void))LLVMGetFunctionAddress(engine, "__anon_tl");
-> 284 	    func();
   285
   286 	    LLVMDisposeExecutionEngine(engine);
   287 	}
[...]
```

Now, we can ask lldb to [disassemble][disassembler] the `func` function, which
will reveal the function's implementation:

```
(lldb) disassemble -s func
    0x101943030: movabsq $0x101944000, %rdi        ; imm = 0x101944000
    0x10194303a: movabsq $0x7fff67760710, %rcx     ; imm = 0x7FFF67760710
    0x101943044: movl   $0xdeadbeef, %esi         ; imm = 0xDEADBEEF
    0x101943049: xorl   %eax, %eax
    0x10194304b: jmpq   *%rcx
    0x10194304d: addb   %al, (%rax)
```

and indeed, we can see the literal value `0xdeadbeef`, which means our Nickel
program has been fully optimisated away by LLVM into the return of a single
literal value - very cool!

[blog_post]: {% post_url blog/2018-09-25-getting-started-with-the-newer-llvm-c-api %}
[jit]: https://en.wikipedia.org/wiki/Just-in-time_compilation
[llvm]: https://llvm.org/
[nickel]: https://github.com/owst/nickel
[lldb]: https://lldb.llvm.org/
[llvm-dis]: https://llvm.org/docs/CommandGuide/llvm-dis.html
[breakpoint]: https://github.com/owst/nickel/blob/404b685af5f0ef494e68e6a0c7068d6a8f97ade8/src/jit.c#L283
[disassembler]: https://en.wikipedia.org/wiki/Disassembler
[^1]: Nickel syntax is superficially similar to Ruby.
