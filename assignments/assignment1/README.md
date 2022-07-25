# Assignment 1: Huffman Encoding and Decoding
# 07-24-2022

## Compile and Run Instructions
1. On a Linux Machine navigate to the huffman directory.
2. Compile the code using Make with the following terminal command:
    ```
    make
    ```
4. Run the code with this command: 
    ```
    ./CODEC input.txt code.txt output.txt  
    ```
Note I only tested the code on WSL2 Ubuntu from my windows 11 laptop. I don't see any reason why it would not work on any ubuntu machine. 

## Encoding
To Encode the input characters into binary symbols I followed the following steps.
### 0. Create Helpful Structures
I created custom Node and NodeHeap structures to store data in a useful way. The Node struct is used to make constructing the huffman tree easier. Each Node holds information about its character, frequency, and sibling Nodes. I also made a struct for a NodeHeap. The NodeHeap Struct along with the popMin() and addNode() functions form a makeshift minimum proiority queue that makes the huffman tree construction much smoother. The Node Heap just holds an unordered map of nodes and a stack of available unused keys for the map. The popMin() function will return the minium (based on frequency) Node in the NodeHeap map, then remove that Node from the map. The addNode() function will add a node to the NodeHeap's map.  
### 1. Generate a Frequency table 
To create the frequency table I simple looped over the characters in the input array and stored them into an unordered map structure where each key was a charatcer and each value was the number of times that character appeared in the input array. 
### 2. Create A min Heap to store nodes
To create the initial NodeHeap I just made a new Node for each key-value pair in the frequency map and stored the Node into a new unordered map with integer keys. Then I just Create a new NodeHeap and set its map to equal the newley Created map. Note at this point the Nodes are set to know that they must be a leaf in the Huffman tree, but their parent node will not be set until the step. 
### 3. Generate the Huffman Tree
Once the Initial NodeHeap was created it could be used to construct a huffman tree. The huffman tree was just represented as an array of Nodes, where the last element in the array is the root of the Tree. To Create the tree the logic was to pop the min two node out of the NodeHeap and create a new non-leaf Node with a frequency is the sum of the two popped Node's Frequency. Then set the two popped Nodes parent to be the newly created Node. Next set the new Nodes left child to be the smallest node and right child to be the second smallest node. Finally store the two popped Nodes into the tree array and add the third, new, node to the Heap. Repeat these steps until only one Node Remains in the Heap. When there is just one Node left you know its the root and you can add it to the end of the tree array;
### 4. Traverse Huffman Tree to get Huffman symbol table 
The symbol table was just another unordered map where the keys were each unique character from the input array and the values are the string (eg.`"1001"`i) representation of the binary symbol for that character. To Construct this table the huffman tree needed to be traversed. Starting at the root until a leaf was reached a `0` was added to the symbol for every left step and a `1` was added for every right step. The traversal was done recursively by starting at a passed node and code (initally the root with code=''), then if it is a leaf you can add its character and the code to the table. If its not a leaf node then you add a `1` to the code and call the function on the left child. Ten once that returns you add a 0 to the code and call the function of the right child. 
### 5. Use Huffman table to generate a string of 1's and 0's
Once the Huffman Table is created it can be used to convert the input character array into a string of 1's and 0's. I simply looped through the input array and matched the current character the corresponding key int he Huffman table. Then i just added the symbol string at the key to the new string of 1's and 0's.
### 6. Encode the Huffman table to a char array
Now all of the information is available and it just needs to be encoded and added to an output buffer. I stated by taking my huffman table and saving converting it to a character array. I simply iterated though the table and generated a char array where the pattern was `,` (delimiter) `key` `sym`.It is possible to make this encoding smaller, but for simplicity I kept the symbols as 8bytes per symbol just so it was easier to decode. Encoding like this makes it easy to reassemble the Huffman map when decoding.
### 7. Encode a message header to a char array
Becase the Output of the encode is an array of bytes its, the compressed message must be divisible by 8. The way I stored the string of ones and zeros the length did not need to be divisible by 8. As a result I zero padded the end of the output array. THe Decoder needed to know the exact bit length of the encoded message, so I added a header that is just an integer value where `msg_len*8-head_int=true_bit_size`. 
### 8. Encode the message string to a char array 
Finally the string of ones and zeros needs to be updated to be an actual byte array so that the code.txt file is compressed. To encode the message I looped through the string of 1's and 0's and converted every 8 charaters to a single unsiged char element in the output array.
### 9. combine all char arrays to one output array
Finally I could combine the huffman table code, header code, and message code into a single output array. I chose to add a `\n` character between each code so it would be easier to decode later on. 

## Decoding
To Decode the compressed byte array to get back the original file I folloed the following steps.
### 1. Reconstruct a Huffman table
To reconstruct the Huffman Table I just read through the first line of the input array. I used the `,` delimiters to make a new unordered map where the keys were stings of 1s and 0s and the value was the character associated with that symbol. Storing this table in the opposite way here makes decoding easier. 
### 2. Store Header into a variable
For this step I just read the byte stored on the second line and stored it as an integer.
### 3. Uncompress the message code into a string
For this step I looped throgh each char in the third line and then for each bit in that char I added a new 1 or 0 to a string.
### 4. Convert message string into output char array
Finally I looped over the string of 1's and 0's from step 3 to reconstruct the original message. For this I just kept a temp string and added the current char to it on each iteration. Then I checked if the temp string matched any of the keys from the table form step 1. If a key was match i reset the temp string and added the char at that key to the output char array.

