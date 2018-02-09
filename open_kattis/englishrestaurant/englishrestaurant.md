# English Restaurant
Taken from https://open.kattis.com/problems/englishrestaurant

Tags: Probability, Dynamic Programming

## Problem Summary
You are building a restaurant; at this restaurant are tables which can fit various sizes of groups of people. When a group enters,
they will be seated at the smallest table which is large enough to fit them. Every hour a group arrives; group sizes are uniformly
distributed. Given a max group size, a number of hours the restaurant is open, and a set of table sizes, determine the expected occupancy
at the end of the day.

## Discussion
This is a VERY difficult problem. We don't really understand how the solution works.
A description of how to solve the problem can be found in this pdf: http://www.nwerc.eu/news/nwerc2017slides.pdf. (Note, we're pretty
sure that pdf is sorted in order of difficulty, and this problem is last.)

The code we have with this file is basically a copy of one of the judges solution files released after the original
contest; I've expanded some of the code and attempted to add whitespace and comments to make it easier to read.

This is what we were able to determine about the solution:
* We're pretty sure that the solution is to count all possible results
* We're pretty sure the extra tables added to accomodate people leaving are so that possible groups of guests don't get discounted

We don't understand:
* Why the binomal coefficient used is (i-j+1 / k-1); it's taking the size of the set considered choose all tables smaller than one in the middle of that set
* How to generalize the first part of the algorithm to account for partially filled ranges
* Why the bounded average is used
* Most of the solution, let's be honest

Other notes:
* Long Doubles are used everywhere because this solution deals with VERY BIG numbers. Using uint64_t numbers will result in overflow errors, as some of the values I printed during testing were in the range of 10^100
