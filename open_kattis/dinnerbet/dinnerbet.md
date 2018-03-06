# Dinner Bet

https://open.kattis.com/problems/dinnerbet

Tags: Dynamic Programming, Probabilities, Combinatorics

## Problem Summary

Given the total number of balls, the number of balls drawn from the box in each
round, the sets of numbers each player (A,B) wrote down, find the expected
number of rounds the game will last. Players cross off numbers which are drawn.
The game is finished once either player crosses off every number on their list. 

## Solution

Let N = total number of balls.

Let C = number of cards in either player's hand.

Let D = number of rounds.

First, find the set of numbers in both player's sets, and consider these
seperately from the rest of the numbers in the player's sets.

Consider a dp state: expected[s][a][b] = expected number of turns for the game
to end given s = number of numbers in both players sets not crossed off, a =
number of numbers only in player A's set not crossed off, and b = number of
numbers only in player B's set not crossed off.

Base Case: when each number both player's sets is crossed off (0 remaining),
and at least 1 player has crossed off all the numbers only in their set.

* expected[0][x][0] = 0 turns, for any x in range: [1,C]

* expected[0][0][x] = 0 turns, for any x in range: [1,C]

Transitions: First we need to define prob(sizeS, sizeA, sizeB, i, j, k) =
assuming there are (sizeS) numbers not crossed off in both player's sets,
(sizeA) numbers not crossed off in player A's set, and (sizeB) numbers not
crossed off in player B's set, the probability that i numbers in both player's
sets are crossed off, j numbers in player A's set are crossed off, and k numbers
in player B's set are crossed off.

prob(sizeS, sizeA, sizeB, i, j, k) = choose(sizeS,i) \* choose(sizeA,j) \*
choose(sizeB,k) \* choose(N-sizeS-sizeA-sizeB, D-i-j-k) / choose(N,D)

Intuition: There are choose(N,D) ways to choose D balls from the total N balls.
We just need to count how many of these ways result in crossing off the required
amounts (i,j,k) in the required sets. There are choose(sizeS,i) ways to choose i
balls to cross off from the remaining sizeS balls in both player's sets,
choose(sizeA,j) ways to choose j balls from the remaining sizeA balls in only
player A's set, and likewise for player B. There are choose(N-sizeS-sizeA-sizeB,
D-i-j-k) ways to choose the remaining balls from the balls not in any set.

Let P = prob(s,a,b,0,0,0), i.e. the probability that no numbers in either
player's sets (or the shared set) are crossed off in the current round.

expected[s][a][b] = 1/(1-P) \* (1 + sum over all distinct (i,j,k) triplets such
that !(i==0 && j==0 && k==0) : prob(s,a,b,i,j,k) \* expected[s-i][a-j][b-k])

Intuition: first, 1 + P + P^2 + P^3 + ... = 1/(1-P). Thus, we can cross off 0
numbers in a given round arbitrarily many number of times, and account for it
once (by multiplying by 1/(1-P) at the end). Now, we consider transitions
between rounds which remove at least 1 number, hence !(i==0 && j==0 && k==0).
The term: prob(s,a,b,i,j,k) \* expected[s-i][a-j][b-k], is the probability of
crossing the given amounts of numbers (i,j,k) off the given sets times the
expected value for the number of rounds given the updated sizes of numbers in
each set. We add 1 to the summation to account for round spend removing numbers
off the lists. 

## Complexity

Number of dp states: O(C^3).

Time to calculate a single dp state: O(D^3).

O(C^3\*D^3) time.
