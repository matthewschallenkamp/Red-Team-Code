#Museum
Taken from https://open.kattis.com/problems/museum

Tags: Geometry, Counting

##Problem Summary
Given an L x W rectangle (L, W positive integers) and some square within that rectangle, find the number of triangles that can be formed using the
points at integer coordinates around the rectangle which won't intersect the square.

##Discussion
The judges' slides from the 2015 BAPC where this problem was first introduced state that a brute-force algorithm will suffice. That is, the given algorithm is to try every group of 3 points and see if any of the three lines intersect the square.

Given that 1 < L, W < 201, and there are up to 31 test cases in a file, this is an inadequate solution. Program runtime must be O(n^2) or better.
I was unable to find a O(n) solution, though the judges' slides hint that one exists. I did however find a way to reduce enough of the problem to counting that it should run in O(n^2) time.

##Solution
In general, I worked the lower-left corner of the rectangle being point (0, 0) and traveled clockwise around the rectangle. Originally I had
planned to let indicies be strictly incrementing, so that if point 0 was referenced from point n, it would be point (2L)(2W), but this proved to be cumbersome, so I switched to all indices being in the range [0, (2L)(2W)).

There are two steps for solving each case.

First, for each point x, the range of points needs to be found such that a line from x to each of those points will intersect the square. Another way to say this is we want to find the first point to the left of the square from x's perspective and the first point to the right from x's perspective.
This can be done for all points in O(n) time. We can see that, for each point xi, the beginning of the range of points for
xi+1 will be the same as or after the beginning of the range of points for xi, and similarly the end of the range will be at or after the end of the range for xi. So, if we find the range for point 0, we can use that range to quickly find the range for point 1, and the range for point 1 can be used to quickly find the range for point 2, and so on.
To check if a point y is to the left of the square from the perspective of some point x, we check the cross product of the vectors xy and xti for all ti being the four corners of the square. If any of those 4 cross products is > 0, then y is either behind the square or to the right from x's perspective. If all 4 are > 0, then it is definitely to the right from x's perspective.

Once these ranges are calculated for all points, we can use them to determine how many triangles are disqualified by the square. The total number of possible triangles is (2L)(2W) choose 3 - 2 * ((L+1) choose 3) - 2 * ((W+1) choose 3). The total number of groups of three points, minus the groups which are all on the same wall.

Now we walk around the rectangle again, and for each point x, we look at each point y in it's range of intersection. This is better than O(n^2), but it's difficult to know exactly what the runtime is. For each x, y pair we look at their intersection ranges together and find 3 values.
* The number of points in both ranges of exclusion
* The number of points in neither range of exclusion
* The number of points in point y's range of exclusion
We need to take care not to include point x in any of these counts. These three values are summed over all x, y pairs and then divided to prevent duplicate counting to find three other values respectively:
* The number of triangles which intersect the square 3 times (Divide by 6)
* The number of triangles which intersect the square 1 time  (Divide by 2)
* The number of triangles which intersect the square 2 times (Divide by 2)

###Explanation
For any two points around the rectangle, we will have a line that may intersect the square. Due to the method of picking x, y pairs above, the line xy will always intersect the square.
From there, we can pick a third point z anywhere else around the rectangle. This will result in one of three cases
* If z is in both ranges of intersection, xz and yz intersect the square (The full triangle intersects 3 times)
* If z is in neither range of intersection, xz and yz don't intersect the square (The full triangle intersects only 1 time)
* If z is only in y's range of intersection, xz doesn't intersect the square, but yz does (The full triangle intersects 2 times)
We don't care about the case where xz intersects and yz doesn't because that is handled during the course of the iterations at the point where x and y are reversed.

The triple-intersecting triangles are counted twice at each point - so we divide by 6. The double-intersect triangles are counted once at each of the two points which don't create a line through the square - so we divide by 2. The single-intersect triangles are counted once on both ends of the line that intersects the square - so we divide by 2.

###Notes
My implementation is a bit of a mess due to the need to check which ranges overlap and how many points are in them. Checking which points are in the ranges requires subtraction of the ends of the ranges and when the one or both of the ranges crosses the boundry between the last point and the first, special consideration needs to be made. To resolve this issue, I tested every single possible situation; first, I check if neither, one, or both ranges crosses the boundary. Then I check for the first range being partially or fully contained by the second and vice-versa. Finally, if the start and end of the second range are both within the first range, a check needs to be done to see if the second range is fully contained in the first or if wraps around the other direction and contains all points outside the first range.
