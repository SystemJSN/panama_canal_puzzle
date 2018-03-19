Panama Canal Puzzle

-------------
|C|A|N|A|M|A|
|P|A|N|A|L| |
-------------

How to change the position of "P" and "C" by only moving the blocks?

This code use Breadth First Tree to solve the Panama Canal Puzzle

1. Create initial position, hash table and the queue. Push the initial position to the queue, and add it to the hash table.
2. Start searching for the solution. If the final position is still not found, pop one position from the queue. If the popped position is the final position, then end the searching process and print the result. If the position is not the final position, try to move the blank character in four directions to get new positions. And mark the popped position as the last position of them. Check these new positions one by one, if they are not in the hash table, push the new position to the queue and add it to the hash table. 
Continue the process until the final position is found.
3. The "print_res" function will be passed two parameters, one of them is the position, another one is the output file stream pointer. If the position is the initial position, just print its content. If not, first recursively call "print_res" to print its last position, then print itself.
