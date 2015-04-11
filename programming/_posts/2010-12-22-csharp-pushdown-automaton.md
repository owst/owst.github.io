---
title: C# Pushdown Automaton
sub_title: A C# implementation of a pushdown automaton
---
I needed to write a [Pushdown Automaton (PDA)][pda] implementation as part of a
genetic algorithm, used to evolve PDA, for a university project.

I've created an implementation using C# and have made the code available on
[Github, here][github_link]. It allows for non-deterministic PDA (NPDA) but
does not allow for &epsilon;-moves.

An example usage, which will match the language of balanced and nested
parentheses:

```csharp
using System;
using System.Collections.Generic;
using PushdownAutomaton;

namespace PDATesting
{
    class Program
    {
        static void Main(string[] args)
        {
            var inputAlphabet = new List<char> { '(', ')' };
            var stackAlphabet = new List<char> { '(' };
            var states = new HashSet<int> { 0 };
            var transitions = new List<PDATransition>
            {
                // Input char, stack head, from state, to state, stack replace.
                new PDATransition('(', '_', 0, 0, "("), // '_' is the empty stack.
                new PDATransition('(', '(', 0, 0, "(("),
                new PDATransition(')', '(', 0, 0, "")
            };

            var pda = new PDA(inputAlphabet, stackAlphabet, states, 0, transitions);

            Console.WriteLine(pda.DoesMatch(""));             // True
            Console.WriteLine(pda.DoesMatch("(())"));         // True
            Console.WriteLine(pda.DoesMatch("(()"));          // False
            Console.WriteLine(pda.DoesMatch("((())()(()))")); // True
        }
    }
}
```

[pda]: http://en.wikipedia.org/wiki/Pushdown_automaton
[github_link]: https://github.com/owst/CSharp-Pushdown-Automata
