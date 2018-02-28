# Candy Chain

https://open.kattis.com/problems/candychain
https://swerc.eu/2017/theme/slides/problemanalysis.pdf

Tags: Dynamic Programming

## Problem Summary

Given a string s, and a list of {string, value} pairs, you want to maximize your
profit. Profit is the sum of all values for all sales. If a substring of s
equals one of the strings of any pair, you can sell this substring for the value
given also in the pair. After the sale, you concatenate the prefix and suffix of
s after removing the substring.

## Solution

First, for each {string, value} pair given in the input, we take the string, and
the reverse of the string, and greedily find the max value for each of these
substrings. Store the final {string, value} pairs in an array called deals.

Consider a dymanic programming state of 4 dimensions: Let memo[i][j][p][k] be
the max profit obtainable on the range [i,j) of s using the deal at index p in
our array deals, and starting at index k of the substring of deals[p].

Base Cases:

* i = j -> range [i,j) is empty so max profit is 0
* k = length of substring deals[p] -> we have completed matching characters in
the substring deals[p], so we have made that sale: max profit is the sum of the
value of deals[p] and memo[i][j][p+1][0] (the max money we can make for the
substring using the remaining deals)

There are 3 types of transitions:

* If s[i] = substring of deals[p] at index k, then we can move onto the next
letter in our substring, and the next letter in s:

memo[i][j][p][k] = max(memo[i][j][p][k], memo[i+1][j][p][k+1])

* Move onto the next deal:

memo[i][j][p][k] = max(memo[i][j][p][k], memo[i][j][p+1][0])

* For m in range [i+1,j], we can skip the range [i,m), then maximize the
remaining part of the range: [m,j):

memo[i][j][p][k] = max(memo[i][j][p][k], memoFull[i][m][0][0]+memo[m][j][p][k])

memoFull????? What is that?

Here is the catch: When we skip the range [i,m), we need to sell every letter in
this range, so that we can concatenate together string s to sell the remaining
part of the substring of deals[p]. Thus we need a second dymanic programming
state: memoFull. 

memoFull[i][j][p][k] = memo[i][j][p][k] if each letter in the range [i,j) is
sold.

memoFull[i][j][p][k] = -infinity otherwise (I used -2 to represent -infinity in
implementation)

memoFull has the same base cases and transitions as memo.

## Complexity

Let |s| = length of s.

Let n = number of deals.

Number of dymanic programming states = O(n\*|s|^3), thus O(n\*|s|^3) memory.

Since each state takes O(|s|) to calculate, we take O(n\*|s|^4) time.


















