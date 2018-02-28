# Cordon Bleu
https://open.kattis.com/problems/cordonbleu

Tags: Hungarian Algorithm

## Problem Summary
You own a restaurant. There are locations around the city from which you want to purchase wine. When you purchase a bottle of wine,
one of the couriers scattered around the city will drive to the wine bottle, then to the restaurant. If you purchase another wine bottle, you have the option of having that same courier continue from the restaurant. Couriers are paid for the distance traveled to the wine bottle, and then for the distance to the restaurant (manhattan distance). Determine which couriers should be used to minimize the cost of buying one wine bottle at each location.

## Solution
This is a simple variation on the job assignment problem. Instead of having n workers and n jobs, we have n workers and m jobs; but also, workers can complete extra jobs after they complete one. To account for this difference, we add m-1 phantom workers at the location of the restaurant. This allows for a single courier to bring a bottle of wine to the restaurant and then go back and forth to get all the rest of them.

Once you've made this minor change and built a cost table, the Hungarian Algorithm can be used to calculate the minimum cost. Note, the 'easy' version of the algorithm has runtime O(n^4), which is too slow; the O(n^3) algorithm is required.