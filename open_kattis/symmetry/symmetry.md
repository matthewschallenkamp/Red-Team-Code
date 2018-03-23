# Symmetry
Taken from https://open.kattis.com/problems/symmetry

Tags: Geometry

## Problem Summary
Given a set of points, determine the minimum number of points that need to be added in order to make the entire set either symmetric across some line or symmetric around some point.

## Solution
We deemed this problem too big for a O(n^3) solution. One of the judges solutions is O(n^3); but we believe it does not run fast enough to be valid. However, that solution can be used as a starting point for a O(n^2?) solution. Runtimes given on the Kattis statistics page imply that there is a much faster solution, but we did not find it.

First of all, we can define a couple of common items to the solutions we've seen.
* A point type, consisting of an integer x and y
* A line type, consisting of integer a, b, and c

We can guarantee that all values stay integers by multiplying all points read by 2.

When looking at a point we know a couple of things:
* It is symmetric about itself
* For every other point
    * The two are symmetric about the point between them
    * The two are symmetric across their perpendicular bisector
    * The two are symmetric across the line through them both

We can keep track of how many points are covered for any given solution by mapping distinct lines and
points to counts.

### O(n^3)
* For each point, add 1 to the count mapped by that point
* For each distinct pair of points (O(n^2))
    * Add 2 to the count mapped by their perpendicular bisector
    * Add 0 to the count mapped by the line through them (Just make sure that line is in the set)
    * Add 2 to the count mapped by their midpoint

* For each line in the mapped set, test each point (O(n^3))
    * If the point is on the line, add 1 to its mapped count

* Find the mapped line or point with the highest count
* That many points are already covered by the best solution; output N - that count

### O(n^2?)
The problem with the previous solution is that we have to test every point against every line to see which ones are covered by being on a line. We can get rid of most of these checks by changing the step 
* Add 0 to the count mapped by the line through them (Just make sure that line is in the set)

At first, it seems like we'd just want to change it to 'add 2', but that would multi-count all of the points which are colinear on any given line if there are more than 2. Instead, we start a second list keeping track of the number of line
segments between given points that are colinear with one of the lines we care about. After all these segments are found, we can solve the inverse of N choose 2 to find the number of points that are on that line.

However, this does not handle the case of a line with a single point in the data set on it; so we need to find a fast way to find all such points. We can do this by forming isosceles triangles. For every two points in the set, if a third point is the same distance from both, then it lies on their perpendicular bisector. At first, this seems like O(n^3), but if we are clever about it, we can get the same information with less complexity.

For each point, we look at all the other points and map them by their distance from the first point. We can sort that set in order to get the groups of points that are all the same distance. Then for each distinct distance away from the first point, we can look at pairs of points; for each of those pairs, we can add to their perpendicular bisector.

Now, this part seems like it breaks the O(n^2) complexity.
* Mapping the points by distance: O(n^2)
* Sorting the distances: O(nlgn)
* Taking the pairwise points the same distance from other points: O(n^2)
But, we're pretty sure that it's not possible for all of these steps to be worst case for all of the points considered; if one of the points is close to worst case, it is unlikely that many other points will be close to worst case. We haven't proven what the complexity ends up as, but in any case it's fast enough.

So, the final solution is as follows 
* For each point, add 1 to the count mapped by that point
* For each distinct pair of points O(n^2)
    * Add 2 to the count mapped by their perpendicular bisector
    * Add 1 to the colinear count mapped by the line through them
    * Add 2 to the count mapped by their midpoint

* For each line O(n^2)
    * Do the inverse of N choose 2 to find how many points lie on that line
    * Add that amount to the count for the line

* Find the mapped line or point with the highest count

* For each point... O(n^2?)
    * Map all other points by their distance away from the point
    * Sort the set of other points
    * For all pairs of points that are the same distance from the first point
        * Find their perpendicular bisector
        * If the colinear count for that bisector is 0, check if it's count + 1 is the new best case (but don't add 1 permanantly)
        * If that's the new best case, we are done; that is guaranteed the final answer

* That many points are already covered by the best solution; output N - highest count

## Notes
We went through a number of rounds of optimization and bugfixes. It's unknown which ones were necessary, but I'll talk about them
* Originally, we were storing lines as slope, intercept doubles instead of A, B, C integers. These were mapped as pair<double, double> using a regular map. There was concern that this was causing precision issues, so we switched to rational math
* The switch to rational math did not yield the correct solution. Originally, we were using unordered_map types. Points were hashed using the macro #define mapp(point) ((uint64_t)(point.x) | ((uint64_t)(point.y) << 32)). This was breaking when casting from signed ints with sign-bit extension and causing points to have the same hash, so we switched to using a std::hash type templated on point
* At this point, we could verify the solution was correct against the published judges data; but the solution was taking too long. I thought the issue might be the time it took to index into map<pair<rational, rational>> and switched back to unordered_maps.
* That was not fast enough. I combined a number of steps to cut down the upfront constant on the runtime, but that was not fast enough
* At this point, I switched to the A, B, C representation of lines instead of slope, intercept
* The change that finally made our solution fast enough was switching from unordered_map to map. We're not sure why this yielded a more performant answer, since map uses a red-black tree and should be O(lgn) accesses instead of the O(1) of unordered_map
* It was speculated that the problem was that unordered_map was having to reallocate memory too often; but using unordered_map::reserve to pre-allocate the space did not solve the issue