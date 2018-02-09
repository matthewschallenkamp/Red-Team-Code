# Tourists

https://open.kattis.com/problems/tourists

Tags: Trees, Binary Lifting, LCA (Least Common Ancestor)

## Problem Summary

Given a tree, find the sum of path lengths between nodes (x,y) such that y>x and
y%x==0.

## Solution

If we can find the length of the path between nodes (x,y) in O(log(n)), we can
sum these lengths to solve the problem.

First, convert the tree into a rooted tree, choosing an arbitrary node as the
root. Let parent[node] = the parent of the node in the rooted tree. We can
calculate the parent array using dfs. Let memo[node][k] = node after doing
node=parent[node] (2^k) times. Since k is at most O(log(n)), memo takes
O(n\*log(n)) memory.

Calculating memo table:

Base case: k=0: memo[node][0] = parent[node]. (2^0 = 1) so we do
node=parent[node] once.

We know that 2^(k-1) + 2^(k-1) = 2^k.

Thus we can calculate memo[node][k] in terms of memo[node][k-1]:

memo[node][k] = memo[memo[i][k-1]][k-1].

Define LCA(x,y) to be the lowest node i in the rooted tree such that x and y are
both in i's subtree. Define distance(x,y) to be the distance between nodes x,y.
Because the simple path between x,y goes through LCA(x,y), we have:
distance(x,y) = distance(x,LCA(x,y)) + distance(y,LCA(x,y)). 

Define depth(x) to be the distance from node x to the root. First we have to
move the lower node to the same level as the higher node. If difference = 
abs(depth(x) - depth(y)), then we jump the lower node up 2^k times in the tree
for each one-bit k in difference. For example, if abs(depth(x) - depth(y)) = 
10100 in binary, then we do node = memo[node][4], then node = memo[node][2].

Now that both nodes (x,y) have equal depth, we jump x and y up at the same time
until they are the same node which is the LCA(x,y).

If memo[x][k] == memo[y][k] and memo[x][k-1] != memo[y][k-1], then jumping 2^k
times leads the same node while jumping 2^(k-1) times doesn't. Thus the LCA(x,y)
is somewhere between these two depths. we move x and y up the tree 2^(k-1) times
and repeat.

We can keep track of the distance both nodes (x,y) travel during each jump.

Video explanation: https://www.youtube.com/watch?v=kOfa6t8WnbI&t

## Complexity

This nested for-loop is O(n\*log(n)):

for(i = 1; i <= n; i++) for(j = i; j <= n; j += i)

The inside loop runs floor(n/i) times during the i-th loop.

Total number of loops = n/1 + n/2 + n/3 + ... + n/n.

 = n\*(1/1 + 1/2 + 1/3 + ... + 1/n)
 
Since the harmonic series grows logarithmically, this equals O(n\*log(n)) total
loops.

Time complexity for calculating rooted tree & each node's parent is O(n).

Time & memory complexity for calculating binary-lifting table is O(n\*(log(n)).

Time complexity for finding the path between nodes x and y is O(log(n)).

There are O(n\*log(n)) paths to sum the lengths of, each taking O(log(n)) giving
O(n\*(log(n))^2) total.
