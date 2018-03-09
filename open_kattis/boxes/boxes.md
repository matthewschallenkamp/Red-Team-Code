# Boxes

https://open.kattis.com/problems/boxes

Tags: Binary Lifting, Trees

## Problem Summary

Given a tree with 200,000 nodes, and up to 100,000 querys, answer the queries.
The queries are as follows: given a set of <= 20 nodes, output the number of
nodes in each of the subtrees of each node in the set. Don't double count any
nodes.

## Solution

First, calculate the following using DFS or BFS:

* depth[i] - distance from i to root

* numNodesSubtree[i] - number of nodes in i's subtree (including node i)

Note, we can use binary lifting to find the k-th ancestor of any node in
O(log(n)). https://www.youtube.com/watch?v=kOfa6t8WnbI

For each query, loop though each distinct pair of nodes in that query
(nodes i,j). WLOG assume depth[j] > depth[i], we need to check if j is in i's
subtree. If j is in i's subtree, we only count the number of nodes in i's
subtree. To check for this, we check if the (depth[j]-depth[i])-th ancestor of j
is equal to i. Intuition: j is in i's subtree iff we perform j = parent[j] until
depth[j] == depth[i], and once depth[j] == depth[i], j and i are now the same
node.

Now the answer for each query is the number of nodes in the subtrees of nodes
which aren't in other node's subtrees.

## Complexity

Precalculation DFS/BFS: O(n)

Individual queries: O(20^2\*log(n))

Total time: O(n+q\*20^2\*log(n))









