# Museum
https://open.kattis.com/problems/battleship

Tags: Simulation

## Problem Summary
Given two 'battleship' game boards and a sequence of shots, determine which player won the game. 

## Discussion
It's important to note the rules for the sequence of turns. If a player hits an opponent's ship then they may take another shot. This continues until they miss or all of the opponent's ships are destroyed. Additionally, both players get the same number of turns, so if player 1 destroys the last ship of player 2, player 2 gets one last turn.

## Solution
This is a fairly small problem. There are up to 2000 shots per game and 20 games per file; that is more than small enough to just run a simulation of the shots. 

I stored the game board as a vector of strings with a counter for the number of ship parts. When a shot is taken, the board can be changed if it's a hit (and the counter decreased). After each player's turn, the simulation checks if the game is over (if any player has no ships left). Special care is taken to make sure that player 2 gets the same number turns as player 1 and that if there are more shots than needed, they are ignored.