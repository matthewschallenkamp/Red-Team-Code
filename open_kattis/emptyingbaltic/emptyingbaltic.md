# Emptying Baltic
https://open.kattis.com/problems/emptyingbaltic

Tags: Graph, Dijkstra

## Problem Summary
You are given the altitudes of 1m squares of land. Some of them are underwater. A drain will be placed somewhere on the map; determine how much water will drain.

## Solution
The first step is to set up the land area as a connected graph. For each underwater land cell, link it to it's underwater neighbors. Next run a slightly modified Dijkstra's algorithm to determine which how far down cells drain. For each cell, keep track of how far it will drain; the cell with the drain will empty completely. When moving to adjacent cells, the cost is the minimum of the current amount drained and the depth of the new cell. At the end, the cost of all cells can be summed to find the total water drained.