# Invasion

https://open.kattis.com/problems/invasion

Tags: DFS

## Problem Summary

Given a weighted, undirected graph, and A vertices which denote alien bases,
find for each base, the number of safe nodes. A safe node is a node which has
distance greater than k to all alien bases built so far. Alien bases are built
in the order they are given in the input.

## Solution

Let minDist[i] = the minimum distance from node i to any of the alien bases
built so far. Initially, minDist[i] = k for each node i.

Now for each base, run dfs from that base. Keep track of the distance from the
start base. We stop the search if the distance from the start base
exceeds/equals minDist[current node]. Otherwise, we update
minDist[current node].

Keep track of the number of safe nodes, and update it continually. Since
minDist[i] = k initially, the first time we reach each node during a dfs with
distance < k, we will decrease the number of total safe nodes.

Why does this work?

If we have been to node i previously with a certain distance, and we get to node
i during a later dfs with a larger distance, there is no need to continue
searching (greedily). We will always have reached an equal/greater number of
nodes during the previous dfs.


## Complexity

Since each edge length is positive, and the safe distance is at most 100, the
minDist value for each node is updated at most 100 times. Thus, time complexity
is amortized O(100\*(n+m)).
