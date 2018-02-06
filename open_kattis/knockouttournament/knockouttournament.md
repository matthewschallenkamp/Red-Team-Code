# Connect the Dots
https://open.kattis.com/problems/knockout

Tags: Trees, Math, Dynamic Programming

## Problem Summary

Given a tree denoting a tournament bracket, and an array of player ratings, one
of which is Dale's rating, find the maximum probability Dale will win. There is
one leaf node for each player. A player with rating A beats another player with
rating B with probability A/(A+B).

## Solution

Have Dale face opponents in order of non-decreasing rating, (face the easiest
players first).

First, build the tree.

For node i (0-based indexing):

left child = 2\*i+1

right child = 2\*i+2

parent = (i+1)/2-1 (integer division)

Place Dale on the right-most leaf node. This leaf node will always have minimum
height among leaf nodes.

Then place the other player's ratings, one per leaf node, sorted in
non-increasing order from left to right. This way, Dale faces the easiest
opponents first.

Now to calculate the probability that Dale wins using dynamic programming:

For each node i, let dp[i] = a list of probabilities that player j makes it to
node i, for each leaf node j in i's subtree.

Denote player's rating at leaf node i as rating(i).
Denote a list of probabilites using curly brackets, ex: {1, 0.5, 0.333}.

Base case: leaf nodes: for each leaf node i, dp[i] = {1}.

For each non-leaf node i:
For each probability Pleft for player j in dp[2\*i+1]:
We sum the probabilities that player j advances to node i by beating each player
in the other subtree:
For each probability, Pright, in dp[2\*i+2]:
sum Pleft \* Pright \* rating(2\*i+1) / (rating(2\*i+1) + rating(2\*i+2)).

We do the same calculation for each player in the right subtree.

The probability Dale wins the tournament is the product of the probabilities
that he wins at each node. The probability Dale wins at a certain node i is the
sum of the probabilities he beats each player who has reached the other child of
the parent of node i.

## Complexity

There are n leaf nodes in the tree, n <= 2^12. The complexity of calculating
dp[i] is O(k^2) where k = number leaf nodes in subtree i.

You might think this gives O(n^3) to calculate dp[i] for each node. But remember
the binary tree is balanced. Thus, consider calculating dp[i] every node i such
that the depth (distance to root) of i is D: There are O(2^D) of these nodes,
each taking O(2^(2\*(log(n)-D))) giving O(2^(2\*log(n)-D)) = O(n^2). Doing this
for each depth gives O(n^2\*log(n)) overall.
















