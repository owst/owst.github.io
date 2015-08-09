---
title: Inverting a binary tree (in Haskell)
---

Given the following definition of (node-labelled) binary trees:

```haskell
data Tree a = Node a (Tree a) (Tree a)
            | Leaf a
            deriving (Show, Eq)
```

We can write an `invert` operation:

```haskell
invert :: Tree a -> Tree a
invert t@(Leaf _) = t
invert (Node v l r) = Node v (invert r) (invert l)
```

Which passes a couple of simple tests:

```haskell
tests :: [(Tree Int, Tree Int)]
tests = [
    l 1 ==> l 1,
    n 2 (l 1) (l 3) ==> n 2 (l 3) (l 1),
    n 4 (n 2 (l 1) (l 3)) (n 7 (l 6) (l 9)) ==> n 4 (n 7 (l 9) (l 6)) (n 2 (l 3) (l 1)),
    n 3 (l 2) (n 4 (l 1) (l 5)) ==> n 3 (n 4 (l 5) (l 1)) (l 2)
  ]
 where
  (==>) = (,)
  l = Leaf
  n = Node

tester :: (Show a, Eq a) => (Tree a, Tree a) -> IO ()
tester (input, expected) = if actual == expected
                               then putStr "."
                               else do
                                   putStrLn "Error! Got: "
                                   print actual
                                   putStrLn "Expected: "
                                   print expected
  where
    actual = invert input

main :: IO ()
main = mapM_ tester tests
```

Neat!
