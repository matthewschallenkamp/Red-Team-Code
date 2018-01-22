# Museum
Taken from https://open.kattis.com/problems/museum

Tags: Geometry, Counting

## Problem Summary
Given an L x W rectangle (L, W positive integers) and some square within that rectangle, find the number of triangles that can be formed using the
points at integer coordinates around the rectangle which won't intersect the square.

## Discussion
The judges' slides from the 2015 BAPC where this problem was first introduced state that a brute-force algorithm (test every possible triangle to see if it intersects the square) will suffice.

Given that 1 < L, W < 201, and there are up to 31 test cases in a file, this is an inadequate solution. Program runtime must be O(n^2) or better to fit the parameters of the problem in a worst-case scenario.
We were unable to find a O(n) solution, though the judges' slides hint that one exists. We did however find a way to reduce enough of the problem to counting that it should run in O(n^2) time.

## Solution
There are two main steps for solving each case.

First, for each point x, the range of points needs to be found such that a line from x to each of those points will intersect the square. Another way to say this is we want to find the first point to the left of the square from x's perspective and the first point to the right from x's perspective.
This can be done for all points in O(n) time. We can see that, for each point xi, the beginning of the range of points for
xi+1 will be the same as or after the beginning of the range of points for xi, and similarly the end of the range will be at or after the end of the range for xi. So, if we find the range for point 0, we can use that range to quickly find the range for point 1, and the range for point 1 can be used to quickly find the range for point 2, and so on.
To check if a point y is to the left of the square from the perspective of some point x, we check the cross product of the vectors xy and xti for all ti being the four corners of the square. If any of those 4 cross products is > 0, then y is either behind the square or to the right from x's perspective. If all 4 are > 0, then it is definitely to the right from x's perspective.

Once these ranges are found, we iterate over all point pairs x, y such that a line from x to y does not intersect the square. For each pair, we find the size of the union of their ranges (and subtract two, so points x and y are not counted). This number is the number of other points that we can safely use as the third point of a triangle with points x and y. We sum the number of valid triangles over all pairs of x, y.

Finally, we make a small adjustment to the result. First, we divide by 6 because each triangle is counted twice for each of it's points. Next, we subtract 2*(W+1 choose 3) and 2*(L+1 choose 3) in order to remove all the 'triangles' that consist of three points on the same wall of the rectangle.

## Notes
In general, I worked the lower-left corner of the rectangle being point (0, 0) and traveled clockwise around the rectangle. Originally I had
planned to let indicies be strictly incrementing, so that if point 0 was referenced from point n, it would be point (2L)(2W), but this proved to be cumbersome, so I switched to all indices being in the range [0, (2L)(2W)).

The coordinate system used by the problem is not obvious without reading very carefully. The upper-left corner of the room is (0, 0), and the y-axis goes to the right from there, and the x-axis down. This can be seen because the limits given for the x, y locations of the treasure are (10e−1 <= X <= L−2 * 10e−1),and (10e−1 <= Y <= W−2 * 10e−1)