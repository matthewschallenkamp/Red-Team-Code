# Mario

https://open.kattis.com/problems/mario

Tags: Simulation

## Problem Summary

The problem states that there are up to 100 boats, moving back and forth
between 2 end points:[l,r]. Mario starts at position 0, and needs to get to
position W along the x axis, W<=500. Mario can jump between 2 boats iff their
x-coordinate is equal.

## Solution

If mario doesn't reach position W in W^2 steps, then it will be impossible for
him to reach the end of the river. Because of this bound, we can simulate the
boats and mario's position. Store a bool value for each boat designating that
mario can reach that boat at time t. At time=0, mario can only reach the boats
at position 0, thus mark the boolean for these boats as true. After each second,
move the boats to their next position. Do this by storing the x-coordinate of
each boat during the current time. During this movement, if a boat which mario
can't get to crosses a boat which mario can get to, then mario can get to both
boats(mark their booleans as both true).

Be careful: If one boat is at position 3 going to position 4, and another boat is
at position 4 going to position 3, mario can go from the first boat to the second
boat and vice-versa when the boats are at position 3.5.

### Discussion

Why can we bound the simulation by W^2 steps? The worst case for Mario to reach
position W is if there are 2 boats with ranges: [0,x], and [x,W], which
intersect at position x, 0 < x < W.
The first boat is at location 0 at times 0, 2x, 4x, ...
The second boat is at position x at times 0, 2 * (W-x), 4 * (W-x), ...

The first time both boats are in their starting positions after t=0 is
t=lcm(2 * x,2 * (W-x)).
This is because it takes 2 * x time for the first boat to return to position 0.
It takes 2 * (W-x) time for the second boat to return to position x.

lcm(2 * x, 2 * (W-x)) <= 2 * x * (W-x).

Since either x <= W/2 or (W-x) <= W/2,  2 * x * (W-x) <= W^2.

These boats either intersect, or don't intersect before time t=W^2.

## Complexity

O(W * W * n) = 500 * 500 * 100 ~ 10^8 per test case.
