# Citadel Construction
https://open.kattis.com/problems/citadelconstruction

Tags: binary search, convex hull, geometry

## Problem Statement

Given n points, n<=1000, find the maximum area of a polygon formed by at most
4 of these points.

## Solution

The naive brute force solution involves taking the max area for all the polygons
formed by the (n choose 4) 4-tuplets of points and will take O(n^4).

How to improve?

First we greedly consider only the points on the convex hull. Intuition for this
is as follows: If we fix 3 of the points, and choose the 4th point not on the
convex hull, we will always get a larger area moving this 4th point to any point
on the convex hull which keeps our quadrilateral non-intersecting.

If our convex hull consists of 2 or 3 points, then the answer is the area of the
polygon formed by the convex hull. Now consider when the convex hull has more
than 3 points.

We can iterate over all possible segments of the convex hull. For each
segment, we can form a quadrilateral with the two points of the segment, and one
point on either side of the segment. The points on either side of this segment
form 2 triangles. We can use binary search/ternary search to find the max area
for both of these triangles independently. The max area of the quadrilateral
formed with this segment as a diameter is the sum of the areas of these 2
triangles both with max area themselves.

The maximum area for any quadrilateral can be found by taking the max of each
quadrilateral formed with a diameter as each possible segment.

## Discussion

How can you use binary search to find the minimum/maximum value on a function?

Short answer: binary search on the first derivative of the function for the
value zero.

You can also use ternary search.

Explanation on convex hull algorithm used (Jarvis's Algo.):
https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/

Explanation for finding the area of a triangle given 3 points:
https://en.wikipedia.org/wiki/Shoelace_formula

## Complexity

Finding the convex hull: O(n^2).

Iterating over all possible segments takes O(n^2). Finding the maximum area of
the triangles on each side of a particular segment takes O(2*log(n)) = O(log(n))
Total time complexity (per test case): O(n^2*log(n)).
