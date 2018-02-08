# 0-1 Sequences

Tags: combinatorics, strings

## Problem Summary

The problem statement is already pretty short:

https://open.kattis.com/problems/sequences

## Solution

Split the number of inversions into 2 parts:

Part 1) Sum the total number of swaps to move each 1 or ? to the last position.

For a particular 1 in the string, let the index of this 1 be i (0-based). Then
this 1 takes size(string) - i - 1 swaps to get to the last index. Sum these
swaps for each 1 in string. Then multiply this sum by 2^k since there are 2^k
sequences to sum over (k = number of '?'s).

Do the same calculation for each question mark: sum size(string) - i - 1 for
each index i such that there is a '?' at index i. Then multiply this sum by
2^(k-1) since each question mark is a '1' in half the strings; 2^k/2 = 2^(k-1).

Part 2) Sum the total number of swaps which we've have over-counted and subtract
this at the end. For example, if our string = 1110, then the first part of the
sum = 6 = ((4-0-1) + (4-1-1) + (4-2-1))*(2^0). So we have over counted by 3 (the
answer is 3). This is because moving all '1's to the last position gives 0001,
and the final string should be 0111. The '1' initially in index=0 should end at
index=1, but we've counted it ending at index=3 (two too many swaps). Likewise
the '1' initially at index=1 should end at index=2, but we've counted it ending
at index=3 (one too many swaps).

First, let triangle(n) = 1 + 2 + 3 + ... + n.

We need to know triangle(n), because if there are k '1's in a particular string,
then the first part of the sum over counts by triangle(k-1).

The amount we have over counted depends on the number of question marks in a
particular string. Let i = the number of '?'s in a particular string. Again, k =
total number of '?'s. There are choose(k,i) ways to choose i '?'s from k
positions. If iCount = total number of '1's, then each of these choose(k,i)
strings over count by triangle((iCount+i) - 1).

Sum this for each i such that 0 <= i <= k and subtract at the end.

## Discussion

Define choose(n,k) as n!/(k!\*(n-k)!).

Calculating choose(n,k) using pascal's identity is O(n\*k). This is too slow for
this problem.

How do you to calculate choose(n,k)%p for n,k <= 10^6 and a prime p, faster than
O(n\*k)?

Fermat's little theorem of course!

The theorem states: (a^p) % p = a % p for prime p.

Dividing by a^2 yields: a^(p-2) % p = a^-1 % p. This alone allows us to
calculate choose(n,k) in O(log(p)) using fast-exponentiation if we precalculate
n! % p in an array. Let fact[i] = (i!) % p. Then, choose(n,k) =
fact[n]\*(fact[k]\*fact[n-k])^(p-2) % p. But we can do better.

Let iFact[i] = (i!)^(-1) % p, iFact is short for inverse factorial. Calculate
iFact[10^6] first : iFact[10^6] = (10^6)^(p-2) % p.

Since (n!)^-1 \* n = ((n-1)!)^-1, iFact[i-1] = iFact[i]\*i % p.

Then, choose(n,k) = fact[n]\*iFact[k]\*iFact[n-k] % p, O(1)!

## Complexity

If n = size(input string), then:

Pre-calucation for fact,iFact tables for choose(n,k): O(n).

Calculating both parts of the sum: O(n), O(n) total.
