# YATP

https://open.kattis.com/problems/yatp

https://www.geeksforgeeks.org/centroid-decomposition-of-tree/

https://sites.google.com/site/indy256/algo/convex_hull_optimization

https://www.youtube.com/watch?v=kOfa6t8WnbI

Tags: Centroid Decomposition, Least Common Ancestor (LCA), Convex Hull, Dynamic
Programming

## Problem Statement

You are given a weighted tree, with each node having a value. The length of a
path between nodes i,j is the sum of the weights of edges on the simple path
from i to j plus value[i]\*value[j]. For each node, find the minimum path length
from it to all other nodes. Then the answer is the sum these min lengths for
each node.

## Solution

First, calculate the centroid decomposition tree. From now on, I'll be refering
only to the centroid decomposition tree.

Consider the path between nodes i,j. There are 2 cases:

1) LCA(i,j) == i || LCA(i,j) == j

2) LCA(i,j) != i && LCA(i,j) != j

Case 1) For each node i, we can update each answer with the path length from i
to every ancestor of i. This can be done naively, and takes O(n\*(log(n))^2)
because the max height of the tree is O(log(n)), and calculating path distances
takes O(log(n)).

Case 2) For each node i, consider only the paths from node i to every node j in
i's subtree. These path lengths take the form value[i]\*value[j] + dist(i,j).
Notice, value[i] is constant in all of these path lengths. Thus, we can consider
path lengths as lines of the form y=m\*x+b where y is the path length, m (slope)
is value[j], x is value[i], and b = dist(i,j). We can use the convex hull trick
(linked above) to calculate the minimum y-value (path distance) over all lines
for a certain x value. Then we can iterate (again) over all nodes j in i's
subtree. The min length path from node j to all other nodes in i's subtree is
the min value over all lines while plugging in value[j] for x. we add dist(j,i)
to the value to retrieve the path length.

For all pairs of nodes (j,k), we will consider the case where LCA(j,k) = i
because we are iterating over all nodes i.

## Complexity

LCA: Time: O(log(n)) to find a path length. Memory: O(n\*log(n))

Centroid Decomposition: Time: O(n\*log(n)) Memory: O(n)

Calculating for each node i, all the nodes in i's subtree of the Centroid
Decomposition Tree: Time: O(n\*(log(n))^2). We calculate one LCA for each of the
O(n\*log(n)) children. Note: this bound can be lowered to O(n\*log(n)) if O(1)
LCA algorithm is used. Memory: O(n\*log(n))

Convex Hull Optimization: Time: O(n\*log(n) + Q) to calculate Q queries from n
lines. Here, the log(n) comes only from the sort. Memory: O(n)

Total: Time: O(n\*(log(n))^2) Memory: O(n\*log(n))
