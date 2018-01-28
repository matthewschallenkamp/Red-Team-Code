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


Explanation on convex hull algorithm used (Jarvis's Algo.):
https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/

Explanation for finding the area of a triangle given 3 points:
https://en.wikipedia.org/wiki/Shoelace_formula


## Discussion

How can you use binary search to find the minimum/maximum value on a function?

Denote our function as f(x). f(x) in our case is the area of a triangle formed
by 2 fixed points on a convex hull, and a third point at index x.

Short answer: binary search on f'(x) (the first derivative of f) for the value
zero.

We can only use binary search on a monotonic function. But f(x) isn't monotonic.

Let the bounds of our search be points indexed as 'start' and 'end'.

We know there exists a point at index k such that:
 - start <= k < end.
 - f(i) <= f(i+1) for all start <= i < k.
 - f(i) >= f(i+1) for all k <= i < end.

Note: these conditions alone allow us to use ternary search.

These conditions don't necessarily make f'(x) monotic. Since the points lie on a
convex hull, f'(x) is monotic because f''(x) < 0 for all x, start<=x<end. We can
binary search on f'(x) for 0; f(x) is maximal when f'(x)=0.

## Complexity

Finding the convex hull: O(n^2).

Iterating over all possible segments takes O(n^2). Finding the maximum area of
the triangles on each side of a particular segment takes O(2log(n)) = O(log(n))
Total time complexity (per test case): O(n^2log(n)).
