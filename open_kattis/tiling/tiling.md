# Domino Tiling
Taken from: https://open.kattis.com/problems/tiling

Tags: Backtracking

## Problem Summary
Given a <= 20x20 grid of numbers in the range 0-9 and a set of domino tiles, determine how many ways the dominos can be placed to fill the board. The board may already have some unmovable dominos placed.

## Solution
This problem can be solved by a simple backtracking algorithm. Our first attempt was incredibly over-engineered, and still ended up taking too long. We attempted to find the spaces that could only be covered by a single domino, and the dominos that could only be placed on a single space; this allowed us to build up part of the solution before starting the backtrack. Unfortunately, doing a backtrack after that while placing dominos was still too long.

The solution was to do a naive backtracking solution with one modification: At each step, find the square of the board with least number of options and place a domino there. This keeps the tree very small and allows all solutions to be counted within time.

After discovering that the solution was to pick the backtracking order this way, I modified our first solution to use a similar method; that made it fast enough to be valid. The modified first solution is also included as 'tiling_long.cpp'