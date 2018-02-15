# Land Inheritance
https://open.kattis.com/problems/landinheritance

Tags: Geometry

## Problem Summary
Given a simple polygon and a line which may or may not intersect it find the area of the polygon's intersection with itself when reflected
across the line

## Solution
Conceptually, this isn't too hard to figure out, but it ends up havinga lot of difficult things in it.

Here are the steps I took to solve this problem:
    - Read the polygon
    - The polygon will be given in either clockwise or counterclockwise order. If it is counterclockwise, reverse it
    - Read two points defining the relection line
    - Spread those lines apart Very Far so they are guaranteed outside the bounds of the polygon
    - Intersect the reflection line with the polygon and insert all the intersection points
    - Split the points of the polygon into those on the 'left' of the reflection and those on the 'right'
    - Reflect the points on the 'left' across the line
    - Intersect the two parts with each other and insert the intersection points into both of them
    - It's possible that either of the sides has been split into multiple pieces connected by edges along the reflection line, so split both sides into its separate parts
    - Map every intersection point on the left to its matching point on the right

At this point, we've processed the input enough that we can finally sum the area of the common intersection.
To find the common area, we look at all the points on one of sides in order. I picked the 'left' side. Each time we find an unused intersection point, it is potentially the start of an intersecting area. Until this point is hit again, do the following, adding points used to a buffer and marking them as used
    - Find the next point clockwise from the intersection on the left side and on the right
    - Compare the vectors from the intersecting point to each of the two next points
    - Pick the rightmost one and move to the corresponding point on whichever polygon it's part of
    - Continue along the points of the current polygon clockwise until another intersection is found

At this point, when you have found the intersection that started the inner loop, find the area of the points in the buffer and add it
to the total.

## Notes
I'm not sure why, but this solution exceeds the memory limit when run in Kattis. However, when run against what we understand to be the maximal test case, it is well within the bounds. My guess is that there is some valid test case that we didn't understand to be valid.