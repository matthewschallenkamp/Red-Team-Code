# Connect the Dots
https://open.kattis.com/problems/knockout

Tags: Trees, Math

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
left child = 2*i+1
right child = 2*i+2
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
For each probability Pleft for player j in dp[2*i+1]:
We sum the probabilities that player j advances to node i by beating each player
in the other subtree:
For each probability, Pright, in dp[2*i+2]:
sum Pleft \* Pright \* rating(2\*i+1) / (rating(2\*i+1) + rating(2\*i+2)).

We do the same calculation for each player in the right subtree.















