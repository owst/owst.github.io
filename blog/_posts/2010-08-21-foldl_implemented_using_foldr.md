---
title: foldl implemented using foldr
---
I'm re-reading [Real World
Haskell](https://www.amazon.co.uk/gp/product/0596514980?ie=UTF8&tag=reaworhas-21&linkCode=as2&camp=1634&creative=6738&creativeASIN=0596514980)
having spent time reading online material (such as the excellent [Haskell
Wiki](https://wiki.haskell.org/Haskell)) to supplement the book, it's a much
easier read the second time through!

That said, I still found myself a little puzzled by the section that provides
an implementation of foldl, using foldr.

The type signatures of foldl and foldr are easy to understand:

```haskell
foldl :: (a -> b -> a) -> a -> [b] -> a
foldr :: (a -> b -> b) -> b -> [a] -> b
```

Both take a function of two arguments, an initial value and a list of values.
The list is 'folded' into a single value, using the function.  `foldl` sums
with left-associativity, `foldr` with right-associativity, e.g. summing
`[1,2,3]`:

```haskell
foldl == (((0 + 1) + 2) + 3)
foldr == (1 + (2 + (3 + 0)))
```


Onto foldl using foldr! The implementation given in RWH is as follows:

```haskell
myFoldl :: (a -> b -> a) -> a -> [b] -> a
myFoldl f z xs = foldr step id xs z
    where step x g a = g (f a x)
```

The key observations here are the use of the 3-parameter `step` function in
place of the usual 2-parameter function, and passing id instead of an initial
value.

To understand how this works, it's useful to see how foldr is implemented:

```haskell
foldr f e []       = e
foldr f e (x : xs) = f x (foldr f e xs)
```


Remember, in myFoldl, `e` is `id` and `f` is `step`

This means that when the foldr unwraps one layer of the list, instead of a
value, a one-parameter function (the partially applied `step` function) will be
returned; when `e` is returned by the base case, instead of an initial value, a
single-parameter function will be returned. So, we end up with a chain of one
argument functions.

To see this, let's trace an execution (bear with it, as it is a little
verbose):

```text
1) myFoldl (-) 0 [1,2] = foldr step id [1,2] 0
```


For now we ignore the `0` and calculate `foldr step id [1,2]`:

```text
2) foldr step id [1,2]
3) step 1 (foldr step id [2])
4) step 1 (step 2 (foldr step id []))
5) step 1 (step 2 (id))
6) step 1 (\a -> id ((-) a 2))
7) \b -> (\a -> id ((-) a 2))((-) b 1)
8) \b -> id ((-) ((-) b 1) 2)
9) \b -> ((-) ((-) b 1) 2)
```


Now we can see that we are left with a one-parameter function,
so we can pass in 0 as the argument:

```text
10) (\b -> ((-) ((-) b 1) 2)) 0
11) ((-) ((-) 0 1) 2)
```

The result: `((-)((-) 0 1) 2)` can be rewritten as `((0 - 1) - 2)` which shows
the left associativity.
