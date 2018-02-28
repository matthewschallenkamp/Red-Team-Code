# Burglary

https://open.kattis.com/problems/burglary
https://swerc.eu/2017/theme/slides/problemanalysis.pdf

Tags: Dynamic Programming

## Problem Summary

Given a bunch of shelves, with candies and ladders, maximize the amount of
candies we can grab decending the ladders, then ascending the ladders. We can
only go down once, then up once. We can only visit each location containing a
candy once.

## Solution

For n rows, and m columns:

First we precalculate the following (0-based indexing), in O(n\*m):

* prefixSum[row][index] = the sum of candies of the row on the prefix ending at
the index
* prefixCnt[row][index] = the count of candies of the row on the prefix ending
at the index
* nextCandy[row][index] = the minimum index of all candies after the index on
the row
* prevCandy[row][index] = the maximum index of all candies before the index on
the row
* ladders[row] = a vector of indexes of ladders on the row

Consider a dynamic programming state memo[row][j][k].

memo[row][j][k] = the maximum amount of cookies you can grab starting on the
row, decending the ladder at index ladders[row][j], going down a number of
levels, ascending back, and ascending the ladder at index ladders[row][k]. Note
we memoize on the \*index\* of the ladders.

Base case: bottom row: this row is always empty, memo[n-1][j][k] = 0 for all j
and for all k.

Only one Transition:

memo[row][j][k] = max(memo[row][j][k], memo[row+1][jLower][kLower] + sum of
cookies ranges on both ranges)

Let L be the maximum number of ladders for a single row (L <= 10).

Then this transition takes O(L^4) because we iterate over all
(j,k,jLower,kLower).

How to calculate sum of cookies ranges on both ranges? Prefix sums:

Consider decending the ladder at position (left) on row (row), then decend
ladder at position (leftLower) on row (row+1). Here (row+1) is the row below
(row) because of the way the 2d input array is inputted.

Also assume left <= leftLower: can collect all the cookies on the range

prevCandy[row][left], nextCandy[row][leftLower], inclusive:

prefixSum[row][nextCandy[row][leftLower]]-prefixSum[row][prevCandy[row][left]-1]

Similarly, if we ascend through ladder at position (rightLower) on the row
(row+1) and then through ladder at position (right) on the row (row), and
assuming right <= rightLower then we can collect all cookies on range:

prevCandy[row][right], nextCandy[row][rightLower] inclusive:

prefixSum[row][nextCandy[row][rightLower]]-prefixSum[row][prevCandy[row][right]-1]

Things to consider:

* decending, then ascending using the same ladder on a certain row
* not double-counting overlap of ranges
* Correctly classifying when a cookie position is visited >= 2 times (and
skipping this case when calculating memo) Here, we use prefixCnt to count the
amount of candies on a range.

## Complexity

O(n\*m) preprocessing.

We take O(n\*L^4) time to calculate all dymanic programming states.

We find the answer in O(L^2).

Time: O(n\*m + n\*L^4)

Space: O(n\*m + n\*L^2)














