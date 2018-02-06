# Human Cannonball

https://open.kattis.com/problems/humancannonball

This problem uses a djikstras search with the cannons and destinations as nodes. The distance function is a bit complex: before 30m, it's faster to run, after it's faster to cannonball first in the direction, then run back. Regardless of the distance, you never want to cannon in any direction other than directly towards the next node in your path, as that will result in a worse total time taken.
