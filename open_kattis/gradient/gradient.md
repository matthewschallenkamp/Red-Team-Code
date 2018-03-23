# Mine the Gradient

https://open.kattis.com/problems/gradient

## Problem Summary
You are given a grayscale image and asked to find the largest occurence of a certain feature. This feature is defined as a square of pixels such that the pixel values are an arithmetic progression when considering them grouped by rows, columns, or diagonals. For example, 
```
1 2 3    2 3    2 2         1 1    1 2    4 6
2 3 4    2 3    3 3         2 3    3 4    5 7
3 4 5
Good features             Not features
```

## Solution
If you aren't familiar with the algorithm used for the problem tables you might want to go read that. It will be helpful to understanding the next part.

Let's consider column by column what the largest rectangles we could make with their left edge at this column might look like, as long as they follow the other feature rules. If we have some rectangle that follows the feature rules, then it must contain a square that follows the same feature rules, and we can get the size of that square by taking the smaller edge of the rectangle. Now, how can we find rectangular features? First, lets tag each spot with the length it can go rightwards while staying in the same arithmetic progression. By default any item can include itself and its right neighbor, so our base width is 2. If we then walk from right to left in each row and compare the differences between the item, the item to the right, and the item two to the right, we can see if these items are in the same progression and possibly increase the height of our item to 1 + the height of the item to the right. We know that if the differences here are similar then for each item forwards up to our height they must also have the same difference.

After this, we have a sort of typed histogram that we can begin building rectangles out of. The type is the difference between each item and its right side neighbor, paired with the difference between its down side neighbor. To find rectangular features now we want to look inside each histogram for groups of similarly typed features, check that their types are valid, and finally capture maximal valid rectangles. We will do so by using a modification of the maximal rectangles in histogram algorithm: keep a stack of opening points and heights, when encountering a similarly typed node pop down to the items height and retrieve the new start by the last popped item, when encountering a differently typed item pop everything off the stack and the new start is here, and when popping items consider this location as their end to find the maximal rectangle they are part of (end - start by height). For keeping track of types we have 3 valid cases: the horizontal and vertical differences have the same magnitude, the horizontal axis is 0, or the vertical axis is 0. When encountering a node that has an invalid type we can replace it with some sentinel type that compares different to everything including itself. This will prevent steep diagonals or other invalid types from creating any significant rectangles. 

## Time Complexity
The first pass to compute the histograms is clearly linear time. For computing maximal rectangles inside the histograms we might push each item on to the stack once and pop it off once, so that section is amortized linear. Finally we only need to return the size so we can keep our max as we go along, for total linear time. Based on the size of the problem (4m elements) this is probably the only doable complexity, and fast IO is recommended.
