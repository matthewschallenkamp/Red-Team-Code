open.kattis.com/problems/mario

The problem states that there are up to 100 boats, moving back and forth
between 2 end points:[l,r]. Mario starts at position 0, and needs to get to
position W along the x axis, W<=500

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
boat and vice-versa.

O(W*W*n) = 500*500*100 ~ 10^8 per test case.
