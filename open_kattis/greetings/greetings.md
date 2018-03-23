# Greetings

https://open.kattis.com/problems/greetings

Tags: Dynamic Programming, Bitmasks

## Problem Summary

Given up to 15 cards, and each of their lengths, heights, and the number of
them, you can choose k different envelope sizes. Minimize the total waste for
these envelopes. You have to put each card in an envelope, so both
(length,width) must be less than or equal to the envelope size. Then waste is
the area(envelope) - area(card).

## Solution

Consider a state of 2 dimensions:

* number of envelopes used so far

* mask of cards which already have envelopes

Let memo[K][mask] represent this.

First we precalculate the amount of waste for each mask. To do this, we walk
through the cards in the mask once to find the max height and width for these
cards. Then we walk through the cards again to sum the waste.

Base case: memo[0][0] = 0. It takes 0 cost to fit 0 cards with 0 envelopes.

For our recurrence relation, we iterate over all subsets of bits in our mask. 
Let each subset be subMask, then:

memo[k][mask] = min(memo[k][mask], memo[k-1][mask&(~subMask)] + (cost for
subMask));

Intuition: First always calculate new results from old. Then, for a certain
collection of cards, we can choose any subset of those cards, and zero-out those
bits in the current mask. By adding the cost to add that subset of cards, we
consider all possible ways to choose envelope sizes.

## Complexity

We have O(k\*2^n) states to calculate, each taking O(2^(number of bits in
mask)). Total operations = the sum from i in range [0,n] : choose(n,i)\*2^i\*k.
This equals O(k\*3^n).
