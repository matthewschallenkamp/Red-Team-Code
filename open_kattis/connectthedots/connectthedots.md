# Connect the Dots
https://open.kattis.com/problems/connectdots

Tags: Geometry, Implementation

## Problem Summary

Given a permutation of (1-16) in a 4x4 grid, find the minimum number of segments
needed to connect these numbers in order. You have to be able to draw these
segments, one after another, without lifting your pencil.

## Solution

After each point, from 1 to 16, store a range of angles which can be obtained by
(possibly) extending the previous segment. There are 2 cases for transitioning
ranges of angles from one point to the next:

Case 1) The angle formed by point and the previous point is in the previous
range of angles:

 - Extend the previous segment
 - The new range of angles is a single angle formed by the current point, and
 previous point

Case 2) The angle formed by point and the previous point is NOT in the previous
range of angles:

 - Add 1 to the count of segments
 - Update the range of angles (see discussion)
 
## Discussion

How do you to calculate the range of angles?

Consider the 2nd test case:

1 2 3 4

8 9 10 11

7 15 16 12

6 14 13 5

Lets first define the angles to be:
Moving right (from 3 to 4) is 0 degrees.
Moving up (from 6 to 7) is 90 degrees.
Moving left (from 5 to 6) is 180 degrees.
Moving down (from 11 to 12) is 270 degrees.
All angles in between should be intuitive.

Denote the range of angles as Start-End.
Denote the angle formed by current point and previous point as Theta.
0 <= Start, End, Theta < 360.
All angles are in degrees.

Both endpoints for the new range are one of the 3 following values:
 - Theta, inclusive
 - (Start+180)%360, exclusive
 - (End+180)%360, exclusive
 
New range is the min of these values to the max of these values.

In the 2nd sample test, the range after point 4 is [0,0].

If we make a segment directly from point 4 to 5, it would have the angle 270,
inclusive. We can also extend the segment from 3 to 4 infinitly, and then draw a
segment to 5 with angle 180=(0+180)%360, exclusive. Thus the range from 4 to 5
is (180, 270]. We don't include 180 in our range, as we can only aproach 180
degrees the more we extent the previous segment.












