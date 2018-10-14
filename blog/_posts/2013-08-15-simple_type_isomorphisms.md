---
title: Simple type isomorphisms
---
I found some simple notes that I wrote for a class that followed Pierce's
excellent book ["Types and Programming Languages"][0] on type isomorphisms. The
course is no longer running, so I thought I'd reproduce the notes here, for
posterity. The examples are written in OCaml, but the ideas easily translate to
other languages such as Haskell or F#.

# Type Isomorphisms

In this note, we'll look at "Type Isomorphisms", which are a general notion of
conversion between types.

## Motivation

Imagine that I've written a particularly good parsing library, and that you
want to use it in your code. Unfortunately, I've written it using types that
are not *equal* to your types, so you can't use my library; disaster!

However, if it's the case that you can write conversion functions, that
transform from my types into yours, and vice-versa, and further, if these
functions don't lose or gain any information, you could write a "wrapper"
around my library, and be able to parse away to your heart's content!

This wrapper would convert function arguments into types that my library
understands, call the relevant parsing functions in my library, before
converting the result back into types that your code understands. As long as
our conversion functions don't "change" the arguments or result in a way that
loses/gains information, then this transformation is "safe", and will guarantee
the correct data goes into and out of my library.

## Isomorphisms

It turns out that this notion of conversion, or adaptation without loss of
information between types has a fancy title: an *isomorphism* between the two
types. We say that types A and B are isomorphic, if we have conversion
functions that *witness* the isomorphism:

```ocaml
f : A -> B
g : B -> A
```

such that:

```ocaml
f . g = id_B
g . f = id_A
```

the notation `.` is function composition (from right to left) and can be
defined as:

```ocaml
g . f == fun x -> g (f x)
```

that is, the composition of f and g (confusingly, we usually read "g . f" as "g
composed with f"!) is itself a function which first applies f, then applies g.

The notation id_X means "the identity function for type X" where the identity
function can be easily defined as:

```ocaml
let id x = x
```

or as a "lambda expression", or "anonymous function":

```ocaml
fun x -> x
```

So, we can study type isomorhpisms as an interesting general technique; they
are just two types `A`, `B`, with conversion functions `f : A -> B` and `g: B
-> A` such that applying the composite of `f` and `g` in either order is the
same as doing nothing.

Right, onto some examples:

## Example: (un)curry

Recall that ["Currying"][1] transforms a function that takes as input multiple
arguments (via a pair), into a function that accepts the first element of the
pair, and returns a function that accepts the second argument, before returning
the result.

We can give the functions for currying and in the "other direction", so called
"uncurrying":

```ocaml
let curry (f : (('a * 'b) -> 'c)) =
    fun (a : 'a) ->
        fun (b : 'b) ->
            f (a, b);;
```

    let uncurry (f : ('a -> 'b -> 'c)) =
        fun ((a, b) : ('a * 'b)) ->
            f a b;;

So, what is the result of using these functions like this:

```ocaml
let doBoth f = uncurry (curry f);;
```

*Exercise*: what is the type of `doBoth`? "Follow the types" through the call
to curry and then uncurry to see that it checks out.

In fact, the curried, and uncurried form of a given function type *are*
isomorphic:

```ocaml
let uncurryCurry f = curry(uncurry f);; (* = id at type "('a -> 'b -> c" *)
let curryUncurry g = uncurry(curry g);; (* = id at type "('a * 'b) -> c" *)
```

We don't prove here that the compositions really are equal to `id`, but with
some simple equational reasoning, we can convince ourselves it's ok.

*Exercise*: do the proof!

So, we've found one isomorphism - between a function and its curried form;
let's try to find some more!

## Sums and Products

Another commonly used data type is the "Sum" type, defined thus:

```ocaml
type ('a , 'b) sum = Left of ('a)
                   | Right of ('b) ;;
```

We can't write it in OCaml, but sometimes we write `('a, 'b) sum` as
`'a + 'b`.

So a value of Sum type `('a, 'b) sum` is either a value of type `'a`, *or* a
value of type `'b`. Similarly, we have the well-known Product (or pair) type
`('a * 'b)`, values of which contain a value of type `'a`, *and* a value of
type `'b`.

Are Sum and Product isomorphic? If so, we can write the witnessing functions.
If not, can we say why not?

Let's try in the Product to Sum direction:

```ocaml
let prodToSum (a, b) = Left a;; (* Any alternatives here? *)
```

Ok, that appeared to sort of work; what about in the reverse direction?

```ocaml
exception Oops;;
let sumToProd (ab : ('a, 'b) sum) : ('a * 'b) = match ab with
    | Left a -> raise Oops
    | Right b -> raise Oops;;
```

Oh. We can't construct witnesses between Product and Sum! So, Product and Sum
are *not* isomorphic. But, why not?

*Exercise*: Answer the question, thinking about missing information.

## Example: The algebra of types

Now for my final example. If we follow our intuitions from high school algebra,
we should have that for any integers a,b,c: `(a * (b + c)) = ((a * b) + (a *
c))`

Can we translate this equality into an isomorphism between the aptly-named
Sum and Product types? Yes we can:

```ocaml
type ('a, 'b, 'c) pairsum = 'a * (('b, 'c) sum);;

type ('a, 'b, 'c) sumpair = (('a * 'b), ('a * 'c)) sum;;

let trans (f: ('a, 'b, 'c) pairsum) : ('a, 'b, 'c) sumpair = match f with
    | (a, Left b) -> Left (a, b)
    | (a, Right c)-> Right (a, c) ;;

(* snart == trans backwards *)
let snart (f : ('a,'b,'c) sumpair) : ('a, 'b, 'c) pairsum = match f with
    | Left(a, b) -> (a, Left b)
    | Right(a, c) -> (a, Right c);;
```


*Exercise*: What do the following evaluate to?

```ocaml
trans (snart (Left(3, true)));;
trans (snart (Right("Hello", fun x -> x)));;

snart (trans (3, Left(true)));;
snart (trans ("Hello", Right(fun x -> x)));;
```

So, there we have it. I've described type isomorphisms: why you might want to
know about/use them, what they are, and how you know if you've got one.

[0]: https://www.cis.upenn.edu/~bcpierce/tapl/
[1]: https://en.wikipedia.org/wiki/Currying
