# Battleship

https://open.kattis.com/problems/joiningnetwork

Tags: Trees, Dynamic Programming, DFS, BFS

## Problem Summary

Given 2 trees, add 1 edge joining the trees, to create a new tree with minimum
transmission cost. Transmission cost for a tree is defined as the sum over all
pairs of nodes (i,j) : distance(i,j)^2.

## Solution

Let sumA[i] = the sum for each node k in the first tree: distance(i,k).
Let sumB[j] = the sum for each node k in the second tree: distance(j,k).
Let sumASqr[i] = the sum for each node k in the first tree: distance(i,k)^2.
Let sumBSqr[j] = the sum for each node k in the second tree: distance(j,k)^2.

We can calculate sumA[i], sumB[j], sumASqr[i], and sumBSqr[j] for each node i in
the first tree (or node j in the second tree) in O(n) using dynamic programming
(see discussion section).

The transmission cost for the first tree is half the sum for all i: sumASqr[i]
(each path is counted twice, once for each endpoint).

Let costA = the transmission cost for the first tree.
Let costB = the transmission cost for the second tree.

The transmission cost for the new tree = costA + costB + distance(x,y)^2 for
each node x in the first tree, and for each node y in the second tree. Here,
distance(x,y) will change depending on which edge you add to connect the trees.

Consider adding an edge from node i in the first tree to node j in the second
tree. Now, distance(x,y)^2 = (distance(x,i) + 1 + distance(y,j))^2. The extra
one represents the edge we've added.

This equals distance(x,i)^2 + 1^2 + distance(y,j)^2 + 2\*distance(x,i) +
2\*distance(y,j) + 2\*distance(x,i)\*distance(y,j).

Notice we have precalculated each of these values. If the first tree has n
nodes, and the second tree has m nodes, then the sum of distance(x,y)^2 for
each node x in the first tree, and for each node y in the second tree equals:

m\*sumASqr[i] + n\*sumBSqr[j] + 2\*sumA[i]\*sumB[j] + 2\*m\*sumA[i] + 2\*n\*sumB[j] + n\*m.

## Discussion

How to calculate sumA[i], sumB[j], sumASqr[i], and sumBSqr[j] for each node (i
or j) in O(n)?

## Complexity

Computing arrays sumA, sumB, sumASqr, and sumBSqr: O(n)
