# Blowing Candles
https://open.kattis.com/problems/blowingcandles

Tags: Convex Hull, Geometry

## Problem Summary
You are given a number of points on the 2D plane. Determine the width of the smallest rectangle (in any orientation) with infinite length, which contains them all.

## Solution
Step 1: Find the convex hull of the points.
Step 2: For each edge on the hull, find the line-point distance to the furthest point on the hull. Take the min of these maxes over the whole shape.

 Since there are up to 200000 points, you need better than O(n^2) runtime for the convex hull. We used Graham's Scan algorithm.
 Likewise, you need better than O(n^2) runtime when finding the min of the max widths. There are two options for doing this:
    * For each edge, binary search around the shape to find the furthest point
    * For each edge, use the furthest point from the previous edge, and walk forward until you find a point where the distance decreases

The first method is O(nlgn), and the second is O(n); but some special cases need to be considered. We used the linear method.

Finally, be sure to handle the case in which all points are colinear; we special-cased it, but that may not be necessary depending on how you write your solution.