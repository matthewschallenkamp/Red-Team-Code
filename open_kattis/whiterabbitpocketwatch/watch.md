# The White Rabbit Pocket Watch

https://open.kattis.com/problems/whiterabbit

Tags: Gauss-Jordan Elimination, Graphs, Dijkstra's, Modulo Inverse

## Problem Summary

You're given a graph, with unknown edge weights, and a list of (path-time)
pairs. For each of the given paths, you are given the nodes visited in the path
and the time (mod 13) given to complete the path. Here, time is defined as the
sum of edge weights between consecutive nodes in the path. Find the shortest
path between a given pair of nodes.

## Solution

First, build a system of linear equations out of the path information. Here,
each equation represents a (path-time) pair, each unknown variable represents
the weight for an edge in that path, and the coefficients in front of each
unknown represent the number of times that edge is traversed in that path. The
other side of each equation represents the time % 13 to traverse that path.

Solve this system using Gauss-Jordan Elimination. Since times are given mod
13, think of both sides of each equation mod 13. Thus, each operation completed
when row-reducing, must be done mod 13. The operations (+,-,\*) are trivial. But
division? See this: http://codeforces.com/blog/entry/23365

After solving for each of the edge weights, finding the shortest path between
the given pair of nodes can be done using Dijkstra's.

## Complexity

If there are |V| nodes in the graph, and |E| edges,

Gauss-Jordan Elimination takes O(|E|^3).

Dijkstra's with adjacency matrix takes O(|V|^2).

O(|E|^3) total time.
















