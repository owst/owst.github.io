---
title: Elegant combinations in Haskell, with sequenceA
---
I've been reading the excellent ["Learn You a Haskell for Great
Good!"](http://learnyouahaskell.com) tutorial recently, and it's definitely
helped me with some of the not-immediately-clear concepts in Haskell.

When I was pointed to [this blog
post](https://mojoai.blogspot.com/2010/08/learning-to-see-patterns-in-my-own.html),
which discusses this [stackoverflow.com
question](https://stackoverflow.com/questions/3510586/algorithm-to-calculate-the-number-of-combinations-to-form-100)
I couldn't help but feel that there had to be a more elegant way to do it.

I remembered the Applicative Functors section of LYAH and put it to good use.
This was the perfect *cough* application:

```haskell
make_lists xs n = sequenceA $ replicate n xs
```

Neat, isn't it!

So, `make_lists` takes a list and a number and returns a list of lists whose
lengths = `n` and whose contents are the permutations of the input list.
For example:

```haskell
make_lists [1,2] 2 == [[1,1],[1,2],[2,1],[2,2]]
```

Describing the individual steps:
`replicate n xs` returns a list, containing `xs`, `n` times e.g.

```haskell
replicate 3 [1,2] == [[1,2],[1,2],[1,2]]
```

When applied to a list of lists, `sequenceA` behaves like a list comprehension
of the following form

```haskell
[x : y : z : [] | x <- xs, y <- ys, z <- zs]
```

(assuming the `sequenceA` was passed a list containing 3 lists: `[xs, ys, zs]`)
which gives us all the possible permutations of the lists' elements.
Nifty!
