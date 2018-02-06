# A+B Problem

https://open.kattis.com/problems/aplusb

Tags: FFT

## Problem Summary

Given an array of n integers denoted A[1], A[2], ... A[n], -50000 <= A[i] <=
50000, Find the number of ordered triplets (i,j,k) which satisfy: A[i] + A[j] =
A[k].

## Solution

Consider a simplified version of the problem: 0 <= A[i] <= 50000 for all i.

Consider a polynomial f(x) = C[i] * x^i where C[i] = the number of occurances of
A[j] such that A[j] = i.

Now consider the coefficients of (f(x))^2.
Denote C^2[i] be the coefficient of the term x^i in (f(x))^2.

Then C^2[i] = the sum from j = 0 to i of C[j]*C[i-j].

But since the coefficients represent counts of occurences of integers in the
origional array, C^2[i] = the number of ways 2 integers in the given array sum
to i.

Use FFT to find (f(x))^2 in O(n\*log(n)).

A similar approach can solve these cases independently:
 - positive + positive = positive
 - positive + negative = positive
 - positive + negative = negative
 - negative + negative = negative
 
Handle These cases seperately using combinatorics:
 - 0 + 0 = 0
 - positive + 0 = positive
 - 0 + positive = positive
 - negative + 0 = negative
 - 0 + negative = negative
 - positive + negative = 0
 - negative + positive = 0

Best FFT explanation I've found:
https://www.cs.cmu.edu/afs/cs/academic/class/15451-s10/www/lectures/lect0423.txt

## Complexity

Each case can be counted in O(n\*log(n)) or O(1) which gives O(n\*log(n))
overall.
