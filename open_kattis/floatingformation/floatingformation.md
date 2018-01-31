# Floating Formation
Taken from https://open.kattis.com/problems/floatingformation

Tags: Graph, Tree, Priority-Queue

## Problem Summary
A bunch of boats are on a lake holding art designs afloat. Each boat has at most two art designs tied to it. Designs can be tied to any number of boats. If a design is tied to only one boat, it will sink and that boat will detatch from any other designs to take the sunken design to shore. This may cause other designs to sink, and then their boats will detatch, possibly causing even more to sink and so on.

Given a configuration of designs and boats, and a number of boats that can each be attached to any one design on the water, what is the minimum number of designs that will sink.

## Discussion
There are a couple important points to note
* Each of the boats we can place can only be attached to one design (not two, like the rest of the exhibit)
* It is guaranteed that, if no boats are placed, at least one design will not sink
* There are no cycles consisting of exactly two boats and two exhibits
* There are no cycles consisting of exactly one boat and one exhibit
* All boats that are pre-placed are attached to exactly two designs

To solve this problem, we will treat this exhibit as an undirected graph with the exhibits being nodes. Any node with degree < 2 is unsafe.
Given all these constraints, we can make a couple statements about the pre-placed exhibit. Specifically, the given graph is composed of a bunch of smaller pieces. Each piece is either
* A cycle, which means all nodes in that cycle are safe
* A tree, which means no nodes in the tree are safe

We are guaranteed there is at least one cycle, that's the only way the problem can guarantee that at least some boats will not sink.

## Solution
For the most part, we can disregard all nodes which are part of a cycle; they will not sink. Now, given a number of trees, we need to determine where to place boats. For a specific tree, the maximum number of nodes we can save with a single new boat is the height of the tree; so if we pick the tallest tree and place one boat at the very bottom, we have achieved the maximum value for placing that one boat. This turns that tree into a line of safe nodes which may have smaller trees branching off.

So, to place boats in this manner, we first differentiate between nodes in cycles and nodes in trees. Then, we go from the leaves of the trees up to the roots, weighting each node as we go. We weight the leaves 1, and we weight each other node the max of its childrens' weights + 1.

Now, we can put each weighted node (not just the roots) into a priority queue. Until we run out of boats, we take the highest weighted node, and, if it not yet saved, walk down its tree, following the highest weighted children all the way. We can mark each of these children 'safe' so they don't get touched later when they are pulled from the queue.

If we keep a counter of the number of safe or unsafe nodes as we go, it is trivial to output the minimum sinking boats at the end.

## Notes
Rather than run a cyle detection algorithm, I chose to find the trees/cycles during the weighting process. I assume that every edge attaches a node to its parent (this is meaningless for the cycles, but not the trees). Each node that has only one edge is a leaf node of some tree. So, as we walk upward weighting the tree nodes, we can correct some of the 'parent' edges to be 'child' edges. Each leaf can mark itself as a child of it's 1 parent. When we step to a parent node, it could still have one of its children marked as a parent, so when a node is stepped to with > 1 parent edges, we break out and move to the next leaf. After all child nodes of a tree node have been stepped on, that node's parent list will be correct, and contain only one link; and then it can be weighted and processed.