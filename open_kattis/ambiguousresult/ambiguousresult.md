# Ambiguous Result

https://open.kattis.com/problems/result

Tags: Dynamic Programming

## Problem Summary

Given a string such as "2+1\*0" (without quotes), find both the max and min
value of the expression over all possible ways to place parentheses around the
numbers in the string, ex: "(2+1)\*0" or "2+(1\*0)" (again without quotes).

## Solution

Memoize on each substring to find max/min value for that substring.

First, parse the text into numbers and operations.

State: let memo[i][j] = max value for the expression starting from the i-th
number to the j-th number (inclusive).

Base Case: memo[i][i] = the i-th number

Transitions:

memo[i][j] = max over all indexes pos, i <= pos < j : memo[i][pos] + or \*
memo[pos+1][j].

Answer for max value is memo[0][size-1]. Use the same appoach for the min value.

## Complexity

There are O(n^2) states, each taking O(n) time to compute giving O(n^3) per test
