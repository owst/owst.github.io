---
title: 'Haskell composition operator fun: (.).(.)'
---
I've been learning [Haskell](http://www.haskell.org) over the summer, and have
definitely had my fair share of head-scratching moments!

Having seen [this reddit
thread](http://www.reddit.com/r/programming/comments/a9tb2/secret_haskell_operators/),
I attempted to figure out the type of the expression `(.).(.)` which is the
infix representation of `(.)(.)(.)` (the function composition operator, applied
to itself twice).

This was not an easy task for a novice Haskeller such as myself, so hopefully
others may benefit from my notes that follow.

The type of the operator `(.)` is:

```haskell
(.) :: (b -> c) -> (a -> b) -> a -> c
```

The first step of figuring out the type of `(.)(.)(.)` is to calculate the type
of `(.)(.)`

Since we are applying the (.) section (a section is a partially applied
operator) to itself, we shall rename the parameter types to avoid confusion.
The first argument, `(.)` (we shall call it `(.)1`) therefore has the following
type:

```haskell
(.)1 :: (b1 -> c1) -> (a1 -> b1) -> a1 -> c1
```

When applying `(.)` to the single argument `(.)1` we are 'substituting' 

```haskell
(b -> c) 
```

for 
    
```haskell
(b1 -> c1) -> (a1 -> b1) -> a1 -> c1
```

taking a three-parameter function and passing it to something that expects a
one-parameter function.  Therefore, the result is a two-parameter function.
This is as if the original function took a a two-parameter function as a
parameter.

Matching the types gives us the following 'mappings':

```haskell
b :: (b1 -> c1)
```

and 

```haskell
c :: (a1 -> b1) -> a1 -> c1
```

In `(.)`'s signature, the result of providing a single argument is of type:
`(a -> b) -> a -> c`

but using the type 'mappings', we obtain a result that looks like:

```haskell
(.)(.) :: (a -> (b1 -> c1)) -> a -> (a1 -> b1) -> a1 -> c1
```

Notice how we've replaced the original single parameter with two new
parameters.

We can then repeat for the second argument of `(.)`, which we shall name `(.)2`
and has type: 

```haskell
(b2 -> c2) -> (a2 -> b2) -> a2 -> c2
```

N.B. `(.)(.)` takes 4 parameters, the first being a function of type `a -> (b1
-> c1)`. 

When we apply `(.)(.)` to `(.)` we are providing its first parameter, which
gives us type mappings of:

```haskell
a :: (b2 -> c2)
```

and

```haskell
(b1 -> c1) :: (a2 -> b2) -> a2 -> c2
```


therefore:

```haskell
b1 :: (a2 -> b2)
```

and

```haskell
c1 :: a2 -> c2
```

The result of applying `(.)(.)` to a single argument is `a -> (a1 -> b1) -> a1
-> c1` But using our mappings we see that the type is:

```haskell
(.)(.)(.) :: (b2 -> c2) -> (a1 -> a2 -> b2) -> a1 -> a2 -> c2
```

We can rename some types to simplify to the result given by GHCi:

```haskell
(.)(.)(.) :: (b -> c) -> (a -> a1 -> b) -> a -> a1 -> c
```

Which is a function that takes a single-parameter function, a two-parameter
function, and two values.  The result is obtained by applying the two-parameter
function to the two values and passing the result to the single-parameter
function.

...and with that we've solved the puzzle!
