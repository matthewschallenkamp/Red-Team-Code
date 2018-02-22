# Airport Coffee
https://open.kattis.com/problems/airportcoffee

Tags: Dynamic Programming

## Problem Summary
A determine the fastest way to travel from point A to point B with places to gain delayed speedups at the cost of any current speedup.

## Solution
At first glance, this just seems like your regular dynamic programming problem. For each location, the time to get to the end is the minimum of the of all t(i) + T for all following locations where t(i) is the time to get to the end from the other location and T is the time to travel to that location. Unfortunately, this is a O(n^2) solution, and with 500,000 locations, it is too slow. However, the properties of the problem mean that we only have to check three nodes following each instead of all of them. Specifically, there are three zones after each coffee stand:
* Waiting to drink coffee
* Drinking coffee
* Done drinking coffee

We need to check the first coffee spot in the first and third zones, and the last coffee spot in the middle zone. If we use a binary search to find these locations, the final runtime is reduced to O(nlgn)