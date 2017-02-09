# 21499

pennyTable.h contains the data structures and their implementations.
pennyTable.c contains the program.

Here is the main algorithm behind my program:

Create a Hash Table to store solved boards in to reference later.
Do the following for every board from 0x0 to a preset size.

Go through each possible move to a piece
If a move is valid, perform the move.
Then isolate any boards that may result from disconnecting the piece, and add them to a linked list of potential positions which can be acheived through one move on the original board.
Undo the move.

After this has happened for every move, go through the linked list of positions and get their Nimbers (either by going back to calculate them, or the Nimbers have already been calculated and the boards have been placed in the Hash Table to lookup). Add the Nimbers to a list.
Sort the list of Nimbers and find the MEX. Set the original board's Nimber to the MEX and add it to the dictionary. 
Move on to next board.






Here are some issues that I'm experiencing:
Non-Full boards are not being evaluated in calculateNimber even though they are apparently correctly identified and added to the linked list in isolate. (By non-full board I mean any AxB sized piece which doesn't not have A times B area). Take a look at what gets printed to see what I mean.

What you see if it prints:
Table Index is printed when the Hash Table gets printed at the end of the program.
NOTICE prints each matrix in the linked list in calculateNimber
Any Case statement gets printed in isolate.

