# Joining Networks

https://open.kattis.com/problems/joiningnetwork

Tags: Trees, Dynamic Programming, DFS, BFS

## Problem Summary

Given 2 trees, add 1 edge joining the trees, to create a new tree with minimum
transmission cost. Transmission cost for a tree is defined as the sum over all
pairs of nodes (i,j) : distance(i,j)^2.

## Solution

Let sumA[i] = the sum for each node k in the first tree: distance(i,k).

Let sumB[j] = the sum for each node k in the second tree: distance(j,k).

Let sumASqr[i] = the sum for each node k in the first tree: distance(i,k)^2.

Let sumBSqr[j] = the sum for each node k in the second tree: distance(j,k)^2.

We can calculate sumA[i], sumB[j], sumASqr[i], and sumBSqr[j] for each node i in
the first tree (or node j in the second tree) in O(n) using dynamic programming
(see discussion section).

The transmission cost for the first tree is half the sum for all i: sumASqr[i]
(each path is counted twice, once for each endpoint).

Let costA = the transmission cost for the first tree.

Let costB = the transmission cost for the second tree.

The transmission cost for the new tree = costA + costB + distance(x,y)^2 for
each node x in the first tree, and for each node y in the second tree. Here,
distance(x,y) will change depending on which edge you add to connect the trees.

Consider adding an edge from node i in the first tree to node j in the second
tree. Now, distance(x,y)^2 = (distance(x,i) + 1 + distance(y,j))^2. The extra
one represents the edge we've added.

This equals distance(x,i)^2 + 1^2 + distance(y,j)^2 + 2\*distance(x,i) +
2\*distance(y,j) + 2\*distance(x,i)\*distance(y,j).

Notice we have precalculated each of these values. If the first tree has n
nodes, and the second tree has m nodes, then the sum of distance(x,y)^2 for
each node x in the first tree, and for each node y in the second tree equals:

m\*sumASqr[i] + n\*sumBSqr[j] + 2\*sumA[i]\*sumB[j] + 2\*m\*sumA[i] + 2\*n\*sumB[j] + n\*m.

Finally, we are left with the problem of choosing the best nodes in A and B to combine based on the two scores at each point in the tree. Naively testing all pairs comes to O(size A * size B) and is clearly too slow. How can we optimize?

Lets consider two nodes in one of the trees, n1 and n2. If n1.sumSqr < n2.sumSqr and n1.sum < n2.sum, then for any node in the other tree n3, cost(n1, n3) < cost(n2, n3). This means that we don't even need to consider n2 at all anymore. It's easy to remove all such nodes with a sort and a linear pass over each tree so now the question is how many nodes are left?

Lets consider a few cases. First one is a rope or linked list. The middle node must have sumSqr and sum a and b. If we move outwards, we increase our sumSqr as well as our sum, because now we have more items further away. Clearly this type of tree will have very few remaining items.

Next, consider a full binary tree, starting at the root. In this we find a similar situation, moving in any direction causes both of our scores to increase, meaning very few remaining items.

Finally, lets consider the case where a=n/x nodes form a blob with a single node in the center, and the remaining b=(x-1)n/x nodes form a rope. We will start at the place where sum is minimal (half of the nodes on each side). Moving down the rope will initially decrease our sumSqr, because the total sum there with position j is = 1/6(j)(j+1)(2j+1), whereas the total sum for the blob side is ~= a\*(b-j)^2. changing j by one creates changes of j^2+j and ~= (2n\*(j+n))/x in our sumSqr. Moving down the rope will also increase our sum, because we now have more than half of the nodes behind us. Where will our sumSqr stop decreasing? a\*(b-i)^2 = i^3. For x = 2, this is at 0, but as x increases, our number changes. for x = 4, this is about 4/18\*n. With that, we have a total time estimate of 50000\*50000/25, which comes out to about 100m. This is around the edges of our time limit but just might be fast enough. This algorithm passes the kattis test cases, so either it's not being tested well enough or this gives a small enough constant multiple to pull us under time.


An alternate solution is to do a greedy walk in each tree. We will move to any vertex in one tree that decreases our score until we reach a local minima in one tree and then switch and repeat until no more movement is possible. We can do this because we know that any move one step away from the global minimum by one edge will change sum/sumsq in such a way that the total score will increase, and any further steps in the same direction must have a similar or more extreme change. Finally, the galloping is important because of the way we find our next step. There is no other way besides a linear search of the edges from the current node, but that could be n! If we don't gallop, we might need to search all the edges of A for each move we make in B. Galloping fixes that by proving that we will only ever consider and fail on an edge-direction twice: once before finding a min and starting a gallop in the other tree and once after. if we fail twice in a row the other tree must be at the min and we are finished.


## Discussion

How to calculate sumA[i], sumB[j], sumASqr[i], and sumBSqr[j] for each node (i
or j) in O(n)?

Lets focus on the first tree (sumA, sumASqr). We can calculate sumASqr in terms
of itself and sumA. So we calculate sumA first.

First, we think of the tree as a rooted tree with root=1. Let sumSubtree[i] = 
the sum over all nodes j in i's subtree: distance(i,j). We calculate (using DFS)
sumSubtree[i] in terms of i's children. We have: sumSubtree[i] = sum over all
nodes j which are children of node i: sumSubtree[j] + sizeOfSubtree[j]. Each
path ending in node j becomes 1 longer to end in node i. Since there are
sizeOfSubtree[j] paths which end in node j, we add 1\*sizeOfSubtree[j] to the
sum.

Notice sumSubtree[1] = sumA[1]. Since 1 is the root, every node is in its
subtree. We calculate sumA[i] (using either DFS or BFS) in terms of i's parent.
If par is i's parent, we have: sumA[i] = sumSubtree[i] + (sumA[par] -
sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i]).

Intuition: sumSubtree[i] will count distances for all nodes in i's subtree.

(sumA[par] - sumSubtree[i] - sizeOfSubtree[i]) will find the sum of distances
from node par to all nodes not in i's subtree.

(n - sizeOfSubtree[i]) counts the number of times the path from i to par is
included.

Now we can calculate sumASqr[i] by first calculating sumSubtreeSQR[i]. Let
sumSubtreeSQR[i] = the sum over all nodes j in i's subtree: distance(i,j)^2. We
calculate sumSubtreeSQR[i] based on i's children: sumSubtreeSQR[i] = sum over
all children j of i: sumSubtreeSQR[j] + 2\*sumSubtree[j] + sizeOfSubtree[j].

Intuition: sumSubtreeSQR[j] = sum for all nodes k in j's subtree:
distance(j,k)^2. For each node k, we want lengthen the path from j to k by one
to get the path from i to k. Thus, sumSubtreeSQR[i] = sum for all nodes k in j's
subtree: (distance(j,k)+1)^2. This term equals: distance(j,k)^2 +
2\*distance(j,k) + 1.

Notice sumSubtreeSqr[1] = sumASqr[1] for the same reason given above. We
calculate sumASqr[i] (using either DFS or BFS) in terms of i's parent. If par is
i's parent, we have:

sumASqr[i] = sumSubtreeSqr[i] + 

sumASqr[par] - (sumSubtreeSqr[i] + 2\*sumSubtree[i] + sizeOfSubtree[i]) + 

2\*(sumASqr[par] - sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i]).

Final Intuition: sumSubtreeSqr[i] counts squared distances to all nodes in i's
subtree.

sumASqr[par] - (sumSubtreeSqr[i] + 2\*sumSubtree[i] + sizeOfSubtree[i]) counts
squared distances from par to all nodes not in i's subtree.

2\*(sumASqr[par] - sumSubtree[i] - sizeOfSubtree[i]) + (n - sizeOfSubtree[i])
modifies the previous sum to count the squared distances from i to each node not
in i's subtree after adding the edge from i to par.

## Complexity

Computing arrays sumA, sumB, sumASqr, and sumBSqr: O(n)


























