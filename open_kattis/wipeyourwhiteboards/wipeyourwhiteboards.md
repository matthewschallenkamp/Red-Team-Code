# Wipe Your Whiteboards

https://open.kattis.com/problems/wipeyourwhiteboards

Tags: Bézout's Identity, binary search, GCD

## Problem Summary

Given (R,S,Q), find (A,B) which satisfy: R*A+S*B=Q and A > 0, B > 0,
and A is minimal. 2 <= R <= 10^8, -10^8 <= S <= -2, 1 <= Q <= 10^8

## Solution

First use the extended gcd for 2 numbers R,S which finds (A,B) such that
R*A+S*B=gcd(R,S) in O(log(max(R,S))). Since S is negative, pass in the absolute
value of S into the extended gcd to avoid a negative value for gcd(R,S).

Bézout's identity: given one solution (A,B) to (R*A+S*B=gcd(R,S)), all solutions
to that equation are in form (A'+k*S/gcd(R,S), B'-k*R/gcd(R,S)) for any integer k.

These equations adapted for S being negative are still:
(A'+k*S/gcd(R,S), B'-k*R/gcd(R,S)). If we passed in one negative value for the
gcd, then gcd(R,S) would be negative, which would cancel with the -1 factor of S.

Now to solve: R*A+S*B=Q where Q is not necessarily equal to gcd(R,S):
The pair of solutions: (A'+k*S/gcd(R,S), B'-k*R/gcd(R,S)) are off by a factor of
Q/gcd(R,S), so are new solutions are:
(A'*Q/gcd(R,S)+k*S/gcd(R,S), B'*Q/gcd(R,S)-k*R/gcd(R,S)).

This pair gives all possible solutions, and we want to find the pair (A,B) such
that A > 0, B > 0, and A is minimal. Since the coefficients (A,B) follow linear
progression both with negative slope, we can use binary search on k to find
maximal k which give positive coefficients.

## Complexity

It can be shown that the extended gcd runs in O(log(max(a,b))) time.
O(log(2*10^9)+log(max(a,b)) = O(log(2*10^9)) per test case.
