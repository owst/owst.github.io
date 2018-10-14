---
title: Haskell "Weasel Program" Genetic Algorithm
---
As part of a university coursework, I had to create an implementation of a
variant of the [Weasel Program][weasel_wiki].

The aim of the program is to show the effect of crossover when evolving a
population towards a target string. The program first evolves a solution
without crossover, then with crossover.
The program in Haskell, looks like this:

```haskell
module Main (main) where

import Control.Applicative
import Control.Monad
import Data.Char
import Data.List
import System.Random

targetStr = "Methinks it is like a weasel"
targetLen = 28
populationSize = 500
replaceProbability = 1 / fromIntegral targetLen

-- Sum the "difference" of two strings' chars.
strDiff :: String -> Int
strDiff input = sum $ zipWith (\a b -> if a == b then 0 else 1) input targetStr

-- Generate a random char (in the range 32d - 127d)
rndChar :: IO Char
rndChar = randomRIO (' ', '~')

-- Generate a string of random chars, of length targetLen
rndString :: IO String
rndString = replicateM targetLen rndChar

-- Replace the input character with a random
-- new character with a probability of (1/length of target)
rndMutateChar :: Char -> IO Char
rndMutateChar input = do
    probability <- randomRIO (0 :: Double, 1)
    if probability <= replaceProbability
        then rndChar
        else return input

-- Randomly mutate the chars of a String
rndMutateStr :: String -> IO String
rndMutateStr = mapM rndMutateChar

-- Take 2 strings and return a new string that is
-- created by randomly picking each char from either
-- string
crossoverStrings :: String -> String -> IO String
crossoverStrings = zipWithM cross
  where
    cross x y = (\b -> if b then x else y) <$> randomIO

-- Generate a list (of the given length) of mutually unique random elements with
-- in the given bounds, using the given generator.
uniqueRandoms :: (Eq a) => (Random a) => (a, a) -> Int -> [a] -> IO [a]
uniqueRandoms bounds targetLen acc
    | length acc == targetLen = return acc
    | otherwise = do
        randVal <- randomRIO bounds
        uniqueRandoms bounds targetLen $ if randVal `notElem` acc
                                            then randVal : acc
                                            else acc

-- Generate a list of unique, random population indices
uniqueRandomIndices :: Int -> IO [Int]
uniqueRandomIndices n = uniqueRandoms (0, populationSize - 1) n []

-- Take 2 individuals, compare using the passed function, returning the
-- first element of the chosen individual (its index)
chooseIndividual :: ((a, b) -> (a, b) -> Bool) -> (a, b) -> (a, b) -> a
chooseIndividual cmp indvA indvB = fst $ if cmp indvA indvB then indvA else indvB

-- choose chooses the more fit individual of form (String, Score)
choose = chooseIndividual (\a b -> snd a <= snd b)
-- chooseR chooses the less fit individual of form (Index, (String, Score))
chooseR = chooseIndividual (\a b -> (snd.snd) a > (snd.snd) b)

-- Takes a new String and a population and replaces the weaker
-- of two randomly chosen individuals with the mutated string.
updatePopulation :: String -> [(String, Int)] -> IO [(String, Int)]
updatePopulation mutatee population = do
    newChildStr <- rndMutateStr mutatee
    replaceIndices <- uniqueRandomIndices 2
    -- Keep hold of the indices, so we can split/merge the population
    let [r1,r2] = map (\i -> (i, population !! i)) replaceIndices
    -- chose the less fit individual to be replaced
    let (start,_:end) = splitAt (chooseR r1 r2) population
    return $ (newChildStr, strDiff newChildStr) : start ++ end

withoutCrossover :: [(String, Int)] -> Int -> IO (String, Int)
withoutCrossover population count = do
    -- pick two random individuals
    indices <- uniqueRandomIndices 2
    let individuals@[x,y] = map (population !!) indices
    -- If we've not evolved the target string yet, carry on
    case find ((== 0) . snd) individuals of
        Just val -> return (fst val, count)
        Nothing  -> do
            -- pick the fitter individual to be the parent
            let parentStr = choose x y
            newPopulation <- updatePopulation parentStr population
            withoutCrossover newPopulation (count + 1)

withCrossover :: [(String, Int)] -> Int -> IO (String, Int)
withCrossover population count = do
    -- pick four random individuals
    indices <- uniqueRandomIndices 4
    let individuals@[a1,a2,b1,b2] = map (population !!) indices
    -- If we've not evolved the target string yet, carry on
    case find ((== 0) . snd) individuals of
        Just val -> return (fst val, count)
        Nothing  -> do
            -- Pick the fitter two of two pairs of individuals to be parents
            let parentA = choose a1 a2
            let parentB = choose b1 b2
            -- create the "crossover" of the two parents
            crossoverStr <- crossoverStrings parentA parentB
            newPopulation <- updatePopulation crossoverStr population
            withCrossover newPopulation (count + 1)

main = do
    -- Create a list of random individuals (just strings)
    individuals <- replicateM populationSize rndString
    let individualFitnesses = map (\i -> (i, strDiff i)) individuals
    -- Now evolve, first without and then with crossover
    result2 <- withoutCrossover individualFitnesses populationSize
    print result2
    result3 <- withCrossover individualFitnesses populationSize
    print result3
```

[weasel_wiki]: https://en.wikipedia.org/wiki/Weasel_program
