# Import Spaghetti

https://open.kattis.com/problems/importspaghetti

Tags: Graphs, Floyd-Warshall

## Problem Summary

Find the shortest length cycle in an unweighted directed graph.

## Solution

Build the graph as a weighted graph with each edge having weight = 1. Then, run
the Floyd-Warshall algorithm to find the shortest path between all pairs of
nodes.

Special case if there is a self-edge: The shortest cycle is this one node.

Now, since the shortest length cycle has >=2 nodes, we can loop for each
distinct pair of nodes: (i,j), and find the minimum of [dist(i,j) + dist(j,i)]
where dist(i,j) is the minimum distance from node i to node j. If there is no
path from node i to node j, then dist(i,j) is considered infinite.

We can reconstruct the path from i to j (and j to i) using BFS, since BFS will
give the shortest path.

https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm

## Complexity

Running Floyd-Warshall takes O(n^3) for n nodes. This dominates the time
complexity of everything else.
