# Dominos

https://open.kattis.com/problems/dominos

Tags: Graphs, dfs, SCC

## Problem Summary

Given the layout of n dominos, n <= 10^5, and given which dominos will knock
over other dominos, find the minimum number of dominos needed to be knocked
over by hand to make every domino fall.

## Solution

First build a directed graph with dominos as nodes, and edges representing
dominos knocking over other dominos.

Define a Strongly Connected Component (SCC) as a set of nodes S with this
property: for each node s[i] in S, you can get to each other node s[j] in S by
traversing a path of edges.

Now consider a new graph with nodes representing SCC's in the origional graph.
If there is an edge from node i to node j, and (i,j) are in different SCC's,
then make an edge from the SCC containing node i to the SCC containing node j in
the new graph.

This new graph is generally refered to as the condensation of a graph.

First, the condensation of the graph has no cycles. If there are any cycles in
the origional graph, all nodes of that cycle are represented in a single node in
the condensation.

Second, knocking over any domino i in the origional graph will cause all the
dominos in the SCC containing node i to also fall.

Because of these properties, our answer is the number of nodes in the
condensation of the origional graph with indegree=0. Any node in the
condensation with indegree>0 will have all dominos knocked over by a domino in
the parent SCC.

More on SCC's:

https://en.wikipedia.org/wiki/Strongly_connected_component

https://www.youtube.com/watch?v=RpgcYiky7uw

## Complexity

Finding all SCC's takes 2 dfs's and is O(n+m). Counting the number of nodes in
the condensation with indegree = 0 takes O(n). O(n+m) total.
