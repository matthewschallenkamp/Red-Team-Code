# Classrooms

https://open.kattis.com/problems/classrooms

Tags: Greedy

## Problem Statement

Given the number of activities (and the start/end time for each activity), and
the number of classrooms, find the maximum number of activities you can schedule
given no 2 activities can be scheduled with overlapping times in the same
classroom.

## Solution

First we sort the activities by end time and process them in this order. The
idea here is that it is always optimal to schedule activities (within the
constrainsts) which end sooner than all other activities. Also, among the rooms
which an activity can be scheduled, it is always optimal to schedule the
activity in the room in which the previous activity ends the latest.

## Complexity

Sort + greedy algorithm: O(n\*log(n) + n\*log(k)) = O(n\*log(n))
