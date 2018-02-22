# Fractal Tree

https://open.kattis.com/problems/fractaltree

Tags: Trees, Lowest Common Ancestor (LCA), Binary Search

## Problem Summary

Answer queries for the length of the shortest simple path between 2 nodes in a
rooted tree having potentially 100000^(2^(30)) nodes.

## Solution

First, special case linked lists: distance is the difference between the labels.

Now, assuming the number of leaf nodes >= 2, every tree with k >= 30 will
have >= 2^30 nodes. Since the labels in the queries are bounded by 2^30, if k >=
30, we only consider the bottom-left copy of F30, and the path to this subtree.

For each node i in each query, we represent i as an array of leaf nodes in the
origional tree. Each leaf node represents the node we traverse down to get to
node i (see the solution slides linked below for clarification).

We can find the array of leaf nodes by binary searching on the leaf nodes:
First, precompute the size of each subtree. Also, store the indexes of all the
leaf nodes in an array. For subtree indexed i (F sub i), we know that the number
of nodes in the subtree with a root being any leaf node of the origional tree
has size equal to subtree indexed (i-1) (F sub (i-1)). Thus we can calculate the
number of nodes between leaves in the origional tree. We repeatedly use binary
search on leaf nodes until we have found a complete path to the node. Combining
this with the way the labels are assigned, we can calculate for every node, the
sequence of leaf nodes in the origional tree traversed to get to that node.

Now to find the distance between nodes in this representation: First, we remove
the common prefix, as moving into the same subtree does not affect distance.

Now, assume the first leaf node is different for both representations. We find
the distance between these nodes in O(log(n)) by first finding the LCA of these
2 nodes (binary lifting method). If the least common ancestor of nodes (i,j) is
LCA(i,j), then the distance from node i to node j is depth[i]-depth[LCA(i,j)] + 
depth[j]-depth[LCA(i,j)]. Here, depth[i] = the distance from node i to the root.
The shortest path from node i to node j must go through the least common
ancestor. For the remaining leaf nodes in each array, we add the distance of
those leaf nodes to the root to the final distance as they are in different
subtrees. We can precompute the distance from each node to the root in the
origional tree using DFS in O(n).

https://ncpc.idi.ntnu.no/ncpc2017/ncpc17slides.pdf

## Complexity

DFS to relabel nodes and calculate distance from each node to root: O(n)

Calculating binary lifting table: O(n\*log(n))

Time complexity per query: O(min(k,30)\*log(n))

Total time complexity: O(n\*log(n) + q\*min(k,30)\*log(n))
