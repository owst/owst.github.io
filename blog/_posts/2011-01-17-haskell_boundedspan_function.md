---
title: Haskell boundedSpan function
---
I was recently playing with an interesting toy problem: Given a string, return
a list of strings, where each element is a grouping of consecutive equal
characters in the string of maximum length 9.

As an example, given `"aaaaaaaaaaaabb"` I wanted to obtain `["aaaaaaaaa",
"aaa", "bb"].`

To help me do this, I wanted a function that was almost the same as the 'group'
function in Data.List, but with the added size bound of 9, on the groups.

Initially, I used span followed by a "wrapping" function, to correctly wrap my
results to <= 9 in length:

```haskell
runs :: (Eq a) => [a] -> [[a]]
runs [] = []
runs xs = let (eqs, rest) = chomp xs in (wrap eqs) ++ (runs rest)

chomp :: (Eq a) => [a] -> ([a], [a])
chomp xs@[]    = (xs,xs)
chomp xs@(x:_) = span (== x) xs

wrap :: [a] -> [[a]]
wrap xs | length xs <= 9 = [xs]
        | otherwise      = before : (wrap after)
    where (before, after) = splitAt 9 xs
```

This seemed somewhat inefficient, as I was having to use append to flatten the
list of "wrapped" strings. I thought that if could fuse the wrap and span
functions, I'd have a more efficient implementation.

Using the Prelude definition of span for guidance, I was able to create the
following function:

```haskell
-- | The 'boundedSpan' function takes an integer @n@, predicate 
-- @f@ and list @xs@, and returns a tuple containing the longest 
-- prefix (possibly empty) of size <= @n@ of @xs@, of elements 
-- that satisfy @p@, and the remaining elements of @xs@.
boundedSpan :: Int -> (a -> Bool) -> [a] -> ([a], [a])
boundedSpan _ _ xs@[]   = (xs, xs)
boundedSpan 0 _ xs      = ([], xs)
boundedSpan n p xs@(x:xs')
    | p x               = let (ys, zs) = boundedSpan (n-1) p xs' in (x:ys, zs)
    | otherwise         = ([], xs)
```

Using this function allowed me to simply cons together a list of strings,
rather than using append:

```haskell
chomp9 :: (Eq a) => [a] -> ([a], [a])
chomp9 xs@[]      = (xs, xs)
chomp9 xs@(x:xs') = boundedSpan 9 (== x) xs

runs2 :: (Eq a) => [a] -> [[a]]
runs2 [] = []
runs2 xs = let (chomped, rest) = chomp9 xs in chomped : runs2 rest
```

Nice!
