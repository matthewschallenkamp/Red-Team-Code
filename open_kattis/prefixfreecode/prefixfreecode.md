# Prefix Free Code
Taken from https://open.kattis.com/problems/prefixfreecode

Tags: Counting, Trie, Segment Tree

## Problem Summary
You are given a list of words, and then a string which is formed by concatenating some k unique words from that list. The words are guaranteed not to be prefixes of other words in the list. If you were to form all such strings by concatenating k of the words, where would the given one lie in that list sorted alphabetically?

## Solution
We used a number of different tools to solve this problem; there's really 3 parts to solving each test case
* Read the given words, and split the final string
* Step through the words used to form the final string in order, being able to determine at all times how many unused words are before and after the current one (alphabetically)
* For each word in the above step, sum number of possible concatenated strings which come before the current one

So let's break this down into how we did each part

### Step 1
Step 1 was completed using a trie data structure. Since the input contains up to 10^6 input, we can't just construct a full trie and fill it in as we go; we have to use maps in each node to keep track of which child nodes it has. Since each word is guaranteed not a prefix of another word, we stored each of the input words in its final node of the trie.

Using the trie, it is trivial to split the concatenated string into its component words

### Step 2
For step 3, we need to know at any given point in time how many of the input words are unused before and after the current one, when the list is sorted alphabetically. For this, we used a segment tree (A Fenwick tree would have also worked). We inserted 1 for every word in the sorted list, and then as we encountered words in the concatenated string, we could query to find the number of 1's before and after it. Then we could mark it used (0).

### Step 3
The final part is the math required to find the number of new words before the current one in the list. As we walked through the words concatenated to form the target string, we sum the following equation, and then mod by 10^9+7, as per the problem specification.

wordsBefore * nchoosek(wordsBefore + wordsAfter, left)

Where wordsBefore and wordsAfter are the number of thus far unused words before and after the current one in the alphabetically sorted set.

## Notes
Looking at the statistics on Kattis after the fact, it appears to be a good thing that we approached this the way we did. It seems like the majority of incorrect submissions received on this problem are (at the time of this writing) due to the algorithm taking too long to execute.
