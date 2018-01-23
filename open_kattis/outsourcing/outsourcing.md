# Outsourcing
Taken from https://open.kattis.com/problems/outsourcing

Tags: DFA, DFA-Equivalence, BFS, Union, Hopcroft-Karp

## Problem Summary
Given two Deterministic Finite Automata each having one start and one end are equivalent

## Discussion
The problem statement does a good job of hiding the real problem in a lot of extra information. The important things to
note are that
* Each factory has a single in node
* Each factory has a single out node
* Each node can have up to 1 outgoing nodes for each operation
These constraints satisfy the requirements of a Deterministic Finite Automata. 

Given the context of the problem, it
seems kind of silly that the second example case should fail (The second factory can produce more different strings
than the first one), but the statement says that they should be able to produce exactly the same products.

## Solution
I chose to use the Hopcroft-Karp DFA Equivalence algorithm to solve this problem. Given to arbitrary DFAs, it can determine if they are equivalent. Since there's only one in node and one out node for each factory, the original
simplification step is easier than it might be otherwise. Details of the algorithm can be found here: https://drona.csa.iisc.ernet.in/~deepakd/atc-2009/Vijeth-Chintan-Seminar.pdf.

Solving each case has three steps:
* Read the factories
* Minimize the factories
* Check the factories for equivalence

### Reading the input
The important things to note here is that we doubly link the nodes so that we can go from child to parent, and that
we map each child node by it's operation and each operation by the child node it connects to. These are used
for quick lookups later on. We also initialize the boolean values in the Union type for the last node in the factory
to indicate that the Union contains an ending node and not an internal node.

### Minimize the factories
Since there is only one start and end node, we don't need to use a particularly complex algorithm to minimize the DFAs.
In our case, minimizing means we find all nodes which can be used in a path from start to end, and remove all of those nodes' children which cannot be used in such a path. To do this, we run a Breadth-First-Search from both ends of the graph and make lists of the nodes visited. Then, for each node, we check if it's in both lists. If not, we remove all links to it from its parents.

### Check for equivalence
Finally, we run the Hopcroft-Karp algorithm to determine if the graphs are equivalent. We start by unioning the two start nodes and pushing them as a pair onto a stack. While the stack has data, we take the top pair from the stack, and look at that pairs' children. If the nodes in that pair have different numbers of children, they are not equivalent. If the operations used to link the nodes their children are different, then they are not equivalent. If they are equivalent, we find the pairs of children which are accessed by the same operation. Those nodes are unioned, and pushed onto the stack as a pair. If unioning them results in a group with both an endpoint and a non-endpoint, the graphs are not equivalent.

## Notes
I'm unsure why, but my code faults when run on the kattis server. I was, however, able to download the original secret test data and run my program against it in order to verify that it works.