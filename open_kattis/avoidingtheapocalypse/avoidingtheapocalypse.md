# Avoiding The Apocalypse
https://open.kattis.com/problems/avoidingtheapocalypse

Tags: Flow

## Problem Summary
A group of people needs to get through a city of zombies to the medic stations. Each road can only be crossed a specific direction, takes a certain amount of time to cross, and limits how many people can start crossing each time step.

Find the max number of people that can safely get to medic stations within a set time limit.

## Discussion
This is a flow problem; and if we structure our graph specifically, then no modifications to the Ford-Fulkerson method are required. I used a DFS for the 'find a path' step of that method, but I believe a queue would work just as well.

## Solution
There are two things we do to build our graph for the Ford-Fulkerson method. 

First, we expand each location into a node at each time point; so if the time limit is 3 seconds, we will have a node for location 0 at time 0, location 0 at time 1, location 0 at time 2, and location 0 at time 3. We connect every node to the same node at time t+1, using an infinite capacity edge; this allows people to wait at a node instead of being forced to move during a time step. Then, when we read the edges given in the problem, we create a version of each edge for each time point that it will be valid (it can be crossed within the time limit).

Second, since there are multiple end locations, we create a super-sink location that is the 'true end' of the graph. We connect each node which is a medic station at the final time point to this super-sink using a road with infinite capacity. This allows people to travel from a medic to the end point for free. If the reach a medic station early, they can just wait there for the final time point, and then jump to the end node.

Now, we can run the Ford-Fulkerson method to find the maximum flow through the graph. By time-layering the nodes, we reduced all edge distances to 1 unit of time, and removed any cycles that might have existed; so no modifications to the method are required.

## Notes
Care should be taken that the Ford-Fulkerson solver doesn't return an answer with more people than exist. Two ways of solving this are
* Create a source node and connect it only to the 'start' location at time 0 with a road having the same capacity as there are people
* Check after ever iteration of the Ford-Fulkerson method if enough flow has been found to save all the people.

It is not sufficient to just do min(ford_fulkerson(), people) at the end, because there are test cases that will have a VERY LARGE max flow, which results in the test case taking too long to solve. (This took me a long time to figure out).