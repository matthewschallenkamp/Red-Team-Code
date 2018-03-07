# Curious Cupid

https://open.kattis.com/problems/cupid

Tags: Mo's Algorithm, Square-Root Decomposition

## Problem Summary

Given 2 arrays of length N, answer Q queries of the form: for the subsegment
from index L to index R (inclusive), find the sum over all languages Lang:
min(count of men who speak Lang, count of women who speal Lang).

N <= 50,000; Q <= 50,000; number of languages <= 1,000,000

## Solution

Mo's Algorithm. There are many articles which explain the algorithm, so I will
not go over it here.

* https://blog.anudeep2011.com/mos-algorithm/

* https://www.hackerearth.com/practice/notes/mos-algorithm/

I'll just explain how to add/remove a single element onto a segment. Since we
have to pair up as many men to women who speak the same language, consider the
count of these pairs differently from the count of men/women who speak a certain
language without a partner. Let:

* cntShared[i] = count of (men/women) pairs who speak language i on the segment

* cntMen[i] = count of men without a partner who speak language i on the segment

* cntWomen[i] = count of women without a partner who speak language i on the
segment

* totalMatches = sum of cntShared over all languages. i.e. answer to the problem
on the segment

Now consider adding 1 element to the segment, i.e. if the current segment is
[L,R], we want to update the above info to get either [L-1,R] or [L,R+1]. These
are identical cases in terms of how to process them. Consider adding the
man/woman pair at index pos. If the man and woman at index pos speak the same
language, then increment cntShared for the language they both speak. Also
increment totalMatches. Otherwise, they speak different languages. So we need to
check if there already exists a partner who speaks that language on the segment
and update the information accordingly. We do this for both the man and the
woman.

Also consider removing an element from the segment, i.e. if the current segment
is [L,R], we want our new segment to be [L+1, R] or [L, R-1]. Again, we check if
the man and woman at index pos speak the same language. If so, we decrement the
cntShared at that language and we decrement totalMatches. If they speak
different languages, for both the man and woman, we check if there are single
people of the same sex who speak the same language on the segment. If so, update
the according info. Otherwise, we know that that person must have a partner, and
we again update the according info.

## Complexity

Since adding and removing one element to the segment is O(1), we have
O(n\*sqrt(n)) time overall.
