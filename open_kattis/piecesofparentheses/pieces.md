# Pieces of Parentheses

https://open.kattis.com/problems/piecesofparentheses

Tags: Dynamic Programming

## Solution

For each piece of parentheses, we care only about these properties:

* length of string

* number of un-matched open and closed parentheses

* Net difference: number of un-matched open parentheses - number of un-matched
closed parentheses

Let memo[i][cnt] = max length for a string of parentheses considering only the
first i parentheses, and a net difference of cnt.

Base Case: memo[0][0] = 0: You can form an empty string of parentheses
considering no given strings, and a net difference of 0.

Recurrence relations:

* memo[i][cnt] = max(memo[i][cnt], memo[i-1][cnt]) : If you don't consider the
i-th piece, then we can always do at least as good as the previous cnt.

* memo[i][cnt] = max(memo[i][cnt], memo[i-1][cnt - (num of unmatched opens -
number of unmatched closes)] + currSize) : First, you can only use the piece at
index i, if cnt - ((num of unmatched opens - number of unmatched closes)) -
currLow >= 0. In other words, you can only use the piece at index i if there are
never a greater number of unmatched closed parentheses (this results in a
non-balanced string).

The only thing left to consider is how to sort the pieces in an optimal way. We
consider strings first which have positive net difference (more opens then
closes).

Of all strings which positive net difference, it is always optimal to consider
pieces with lower number of unmatched closed parentheses first. Among pieces
with equal number of unmatched closed parentheses, consider pieces with greater
net difference.

Of all pieces with negative net difference, it is always optimal to consider
pieces with a greater number of unmatched opens. If this is equal, then consider
pieces with larger net difference.

## Complexity

There are O(n^3) dp states, each taking O(1) time to compute: O(n^3)
