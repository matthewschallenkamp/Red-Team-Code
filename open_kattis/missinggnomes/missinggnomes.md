# Missing Gnomes

https://open.kattis.com/problems/missinggnomes

Tags: Greedy

## Solution

We want to fill in the missing numbers in the permutation in strictly increasing
order, otherwise the solution won't be the smallest lexicographically. When
we're filling in the numbers before a number x in our array, we only want to
fill in missing numbers which are <= x.

## Complexity

O(n)
