# Suffix Array Reconstruction
open.kattis.com/problems/suffixarrayreconstruction

Tags: Strings

## Problem Summary

Reconstruct a string given its suffixes. The suffixes can have
at most 1 '*' representing an arbitrary long (possible zero length) sequence of
characters.

## Solution

Start with a string representing the answer with blank characters.

If a suffix doesn't have an asterix, then we can naively fill in these characters.
If a suffix does have an asterix, then it will break the string into 2 pieces: a
beginning piece and an end piece. Both these pieces can have positive or zero
length. We can fill in the end piece like we do suffixes without asterix. We can
fill in the characters for the beginning piece starting at the given index.

If at any time, we are filling in a character, and a different character other
than the black character is already in that position, the answer is impossible.

At the end, if there are still black or unfilled characters, the answer is
impossible.

Otherwise, output the filled string.

## Complexity

Since there are at most 500,000 characters per test, this linear solution passes
within the time limit.
