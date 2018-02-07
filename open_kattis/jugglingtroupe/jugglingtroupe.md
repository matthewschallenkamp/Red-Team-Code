# Juggling Troupe
Taken from https://open.kattis.com/problems/jugglingtroupe

Tags: Simulation

## Problem Summary
There is a group of jugglers who perform in a line. Unfortunately, they are all terrible at juggling. At any time step, each juggler holding two or more balls will toss a ball to each of his sides. If he is at the end of the line, that ball
disappears from the show.

Determine, which jugglers will be holding a single ball at the end of the show.

## Discussion
This looks like a fairly simple simulation until you see that there can be up to 10^6 jugglers. That is too many for a 
O(n^2) solution. However, given that the problem runtime is 5 seconds, the solution doesn't have to be overly optimized.

## Solution
We found a way to solve this problem on a single pass through the list. If you draw out a couple cases, you will notice that a juggler having 2 balls results in a cascading effect between him and the first juggler to his sides with no balls.

Example: 1 0 1 2 1 1 0 2
```
1 0 1 2 1 1 0 2
1 0 2 0 2 1 1 0
1 1 0 2 0 2 1 0
1 1 1 0 2 0 2 0
1 1 1 1 0 2 0 1
1 1 1 1 1 0 1 1
```

Our solution stores all the zero-ball locations to the left of the jugglers as it traverses the list. Each time a juggler is found with > 1 balls, we can do all of the steps of that cascade at once and find the location where the ball will settle.

When a juggler drops a ball on his left and right, they may immediately be tossed back to him; but at the same time, the adjacent jugglers will toss their balls to the sides, so when the first juggler tosses the balls again; his adjacent jugglers may just keep them. When traveling down the line from left to right, this behavior results a juggler being able to toss away at most 1 ball that won't return unless a juggler next to him has no balls (he would just hold the one tossed to him then).

For any case, when a juggler tosses a ball away, the closest zero location to him will end up filled, and the spot immediately to its right will become a zero location. So, using this idea the juggler can toss away 1 ball per time step until that zero location walks itself to be directly next to him - at that point he can toss away 2 balls.

If the juggler is still holding more than 2 balls, the process repeats with the next closest zero location on his left.
As we step through the list, the balls tossed to the right just accumulate, and will be handled similarly.