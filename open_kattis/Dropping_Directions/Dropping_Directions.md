# Dropping Directions
https://open.kattis.com/problems/droppingdirections

Tags: Graphs

## Problem Summary
You are given an undirected graph with each node having degree=4 representing an
intersection. You can go straight, or turn left/right at each intersection. If
there is no sign at an intersection, you go straight. Otherwise go in the
direction of the sign. Signs can only point left or right.

## Solution

Since each node/intersection has degree=4, if you traverse this graph only going
straight, you will always end up where you started. Thus we can traverse each
cycle once, going only straight. If, after completely traversing a cycle, we
have not reached the destination, we add 1 to our sign count. The starting
location has 2 directions to go: the direction of (a,c) given in the problem
statement, and the direction of (b,d). Thus we need to check both of these
directions.

## Complexity
Each node part of a cycle, and each cycle is traversed once. Thus O(n) per test
case.
