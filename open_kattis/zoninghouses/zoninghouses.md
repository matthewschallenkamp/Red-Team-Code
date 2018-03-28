# Zoning Houses
Taken from https://open.kattis.com/problems/zoninghouses

Tags: Segment Tree

## Problem Summary
You are given a number of house locations on a grid. For each query, i, j determine the smallest box which can contain houses i-j, ignoring at most 1 of them.

## Solution
The key to this problem, is that if you know the bound for a set of houses [i, j], then the smallest bound containing all but 1 of the houses can be found by comparing the bounds found when 1 house on the bound is removed.

This works because each edge of the bound is 1 of 2 cases.
* There is only 1 house on that bound, it is the reason the bound is extended that far in its direction.
* There are multiple houses on that bound

In the first case, removing the house from the set has the potential to shrink the bound. In the second case, removing any 1 of the houses will not change the bound. Therefore, we know we only need to test removing 1 house from each of the four edges of the boundary to find the best case.

This problem is solved by using a segment tree with all of the information in it. For each node of the tree, we store one point on each of the boundaries. Then, for each query read, we can query the entire range to get the bounds, and for each direction remove the point from the segment tree that is causes the bounds to be extended. After this, it is a simple matter of querying the segment tree 5 times and comparing the results to find the best bound