# Assignment 1: Huffman Encoding and Decoding
# 07-30-2022

## Encoding
### Inputs
* Text file 
### Outputs
* Huffman tree
* Huffman/Prefix Codes
### Logic
1. Loop through text and create a Char-Freq hash table
    * Keys = each unique character
    * Values = # of occurrence of that character
1. Create Custom Struct to hold Char-Freq pair in a single element.
    * char character -> the character 
    * uint freq -> the # of times that character occured
    * bool isSym -> flag to define if struct is a "true" char
1. Create custom operations for the struct
    * compare(a,b) -> returns true if freq of a<b
1. Loop through hash table and place all pair structs into a min-priority queue
    * set the priority queue to use the custom compare function.
1. Generate the Huffman Tree
    * pop the top two items out of queue.
    * add their frequencies
    * create a new struct instance with the summed freq and isSym=false, char does not matter
    * add the new struct back into the queue
## Decoding
### Inputs
* Huffman tree e
* Huffman/Prefix Codes
### Outputs
* Text file 

## References 
https://en.wikipedia.org/wiki/Huffman_coding
