#include "huffman.h"
#include <iostream>
#include <map>

void print_map(std::map<char, int> mp)
{
    std::cout << "KEY\tELEMENT\n";
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) 
    {
        std::cout << itr->first << '\t' << itr->second << '\n';
    }
}
/**
 * TODO Complete this function
 **/
int huffman_encode(const unsigned char *bufin,
						  unsigned int bufinlen,
						  unsigned char **pbufout,
						  unsigned int *pbufoutlen)
{
std::cout << "Encoding Input File\n";
    //1. make a hash table, key=char value=#occur
    std::map<char, int> symbolTable = {};
    for(int i = 0; i < bufinlen; i++)
    {
        char key = *(bufin + i);
        if(symbolTable.count(key) == 0) { symbolTable.insert({key,1}); }
        else{ symbolTable.at(key) += 1; }
    }
    print_map(symbolTable);
    //2. initalize binary tree len = # of keys in has table
    char *hufTree{};
    //3. 
std::cout << "Decoding Input File\n";
	return 0;
}


/**
 * TODO Complete this function
 **/
int huffman_decode(const unsigned char *bufin,
						  unsigned int bufinlen,
						  unsigned char **pbufout,
						  unsigned int *pbufoutlen)
{
    std::cout << "Decoding Input File\n";
	return 0;
}
