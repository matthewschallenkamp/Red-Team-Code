# Within Arm's Reach
Taken from https://open.kattis.com/problems/withinarmsreach

Tags: Robotics, Kinematics, Geometry

## Problem Summary
You have a serial manipulator with N links. That is, you have a robotic arm built from N sections connected consecutively. There is some point that you would like the arm to reach. Find the sequence of points that the sections should end at in order for the final section to be positioned over that target point.

In roboticist's terms: Solve the Inverse Positional Kinematics for an N-link serial manipulator.

## Discussion
There are a number of different approaches to take to this problem; and in the context given, none is more right than another. The judge's slide deck suggests a couple options, such as attempting to inscribe a circle that passes through the origin and target point and binary searching on the radius.

## Solution
We took the approach of finding the 'reachable area' for each segment. Thinking about this in a linear fashion, we realized that each segment could reach it's own length beyond where the previous segment could reach, and it's own length towards the origin from where the previous link could reach. Care had to be taken with links that could potentially reach back to the origin; but once that was done, we had the inner and outer radii of a washer shape representing the area each link could reach. In some cases, the inner radius was 0; the link could reach back to the origin.

After these ranges were found, we could check if the target point was within any of the ranges. If it wasn't, we moved the target point towards or away from the origin to the closest edge of a reachable section.

Once the target point was guaranteed reachable, the algorithm for laying out the sections was fairly simply.
In reverse order of the segments:
* Place the end of the current section on the target.
* If the target point is within the link's size range of the edge of the reachable section
    * Find a location on the edge of the reachable section that the link can reach to; move the target point there
* Otherwise
    * Extend the segment directly away from the origin, move the target point to the point it reaches

At some point, the segments would walk outwards from the origin and hit the boundary of a reachable section. At that point, we could just hop from reachable boundary to reachable boundary all the way back to the origin.