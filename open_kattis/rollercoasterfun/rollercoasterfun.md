# Roller Coaster Fun

https://open.kattis.com/problems/rollercoasterfun

This problem is very similar to the normal knapsack problem, and as such is solved with dynamic programming. Essentially we have two types of items, some that reduce in value the more times they are taken, and some that do not. Our initial solution was to create two reccurence relations for the different types of items. For normal items we will recurse based on each of the possible counts that we could take for that item, which is fairly limited at <32 because the reducing function (a - k^2*b) will always go negative after that. For non-reducing items we can choose to either take another of this item or stop taking them, resulting in only two recursions. This solution ends up with a complexity of 33 recurs per spot * 25000 possible time left * 100 roller coasters to consider ~= 80m im 3 seconds.

There is an important optimization to making the above calculation fast enough, and that is to build a table rather than using a memoized function. After rewriting my solution to iterative, it changed the speed enough to be accepted at 0.23s, rather than failing at >3s as we experienced during the contest.