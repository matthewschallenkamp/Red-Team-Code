# Joining Networks

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

Lets focus on the first tree (sumA, sumASqr). We can calculate sumASqr in terms
of itself and sumA. So we calculate sumA first.

First, we think of the tree as a rooted tree with root=1. Let sumSubtree[i] = 
the sum over all nodes j in i's subtree: distance(i,j). We calculate (using DFS)
sumSubtree[i] in terms of i's children. We have: sumSubtree[i] = sum over all
nodes j which are children of node i: sumSubtree[j] + sizeOfSubtree[j]. Each
path ending in node j becomes 1 longer to end in node i. Since there are
sizeOfSubtree[j] paths which end in node j, we add 1*sizeOfSubtree[j] to the
sum.

Notice sumSubtree[1] = sumA[1]. Since 1 is the root, every node is in its
subtree. We calculate sumA[i] (using either DFS or BFS) in terms of i's parent.
If par is i's parent, we have: sumA[i] = sumSubtree[i] + (sumA[par] -
sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i]).

Intuition: sumSubtree[i] will count distances for all nodes in i's subtree.

(sumA[par] - sumSubtree[i] - sizeOfSubtree[i]) will find the sum of distances
from node par to all nodes not in i's subtree.

(n - sizeOfSubtree[i]) counts the number of times the path from i to par is
included.

Now we can calculate sumASqr[i] by first calculating sumSubtreeSQR[i]. Let
sumSubtreeSQR[i] = the sum over all nodes j in i's subtree: distance(i,j)^2. We
calculate sumSubtreeSQR[i] based on i's children: sumSubtreeSQR[i] = sum over
all children j of i: sumSubtreeSQR[j] + 2\*sumSubtree[j] + sizeOfSubtree[j].

Intuition: sumSubtreeSQR[j] = sum for all nodes k in j's subtree:
distance(j,k)^2. For each node k, we want lengthen the path from j to k by one
to get the path from i to k. Thus, sumSubtreeSQR[i] = sum for all nodes k in j's
subtree: (distance(j,k)+1)^2. This term equals: distance(j,k)^2 +
2\*distance(j,k) + 1.

Notice sumSubtreeSqr[1] = sumASqr[1] for the same reason given above. We
calculate sumASqr[i] (using either DFS or BFS) in terms of i's parent. If par is
i's parent, we have:

sumASqr[i] = sumSubtreeSqr[i] + 

sumASqr[par] - (sumSubtreeSqr[i] + 2\*sumSubtree[i] + sizeOfSubtree[i]) + 

2\*(sumASqr[par] - sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i]).

Final Intuition: sumSubtreeSqr[i] counts squared distances to all nodes in i's
subtree.

sumASqr[par] - (sumSubtreeSqr[i] + 2\*sumSubtree[i] + sizeOfSubtree[i]) counts
squared distances from par to all nodes not in i's subtree.

2\*(sumASqr[par] - sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i])
modifies the previous sum to count the squared distances from i to each node not
in i's subtree after adding the edge from i to par.

## Complexity

Computing arrays sumA, sumB, sumASqr, and sumBSqr: O(n)


























