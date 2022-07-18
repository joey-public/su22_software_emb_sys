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
    std::map<char, int> symbolTable = {};
    for(int i = 0; i < bufinlen; i++)
    {
        char key = *(bufin + i);
        if(symbolTable.count(key) == 0) { symbolTable.insert({key,1}); }
        else{ symbolTable.at(key) += 1; }
    }
std::cout << "Finished Encoding Input File\n";
    return 1; 
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
