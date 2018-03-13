# Intergalactic Mortgage

Taken from: https://open.kattis.com/contests/naipc18-p10/problems/mortgage

Tags: Math, Fast-Exponentiation, Compound-Interest

## Problem Summary
Given the principal amount and some interest rate for a loan compounded 12 times annually, determine if the recipient can pay off the loan within some number of years if they pay a given amount after each compounding.

## Solution
There are two things needed for this solution. First of all, you have to know the closed-form equations for calculating compound interest and how much the recipient will end up paying over time. Then you can just check if they pay more or less than the loan is worth in the time.

The equations we used:
* X = principal
* Y = debt payed after each compounding
* R = yearly rate
* N = term of loan (number of years)
### Total Debt Owed
X * (1 + r/12.0)^(N * 12)

This is the standard compound interest formula

### Total Debt Paid
Y * ((1.0 + (r/12.0)) ^ (N * 12.0) - 1.0) / ((1.0 + (r/12.0)) - 1.0)

This is a geometric sum across the entire term of the loan
to find the amount of money paid with some interest included

The second part is recognizing that you need to use fast exponentiation. The powers taken can get as big as 120,000, so using the std::pow function in the C++ standard will result in a Time Limit Exceeded error -- you must write a custom pow; this works because all powers taken are integers so log(n) pow can be used.