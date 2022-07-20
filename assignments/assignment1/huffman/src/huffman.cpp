#include "huffman.h"
#include <iostream>
#include <map>
#include <queue>

void print_map(std::map<char, int> mp)
{
    std::cout << "KEY\tELEMENT\n";
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) 
    {
        std::cout << itr->first  << '\t' << itr->second << '\n';
    }
}
void print_queue(std::queue<char> q)
{
    int len = q.size();
    for(int i=0; i < len; i++)
    {
        std::cout << q.front() << std::endl;
        q.pop(); 
    }
}

void getMax(std::map<char, int>& mp, char& c, int& i)
{
    c = mp.begin()->first;
    i = mp.begin()->second;
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) 
    {
        if(itr->second > i)
        {
            c=itr->first;
            i=itr->second;
        }
    }
    mp.erase(c);
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
    //1. calculate the frequency of each char 
    std::map<char, int> symFreqMap = {};
    for(int i = 0; i < bufinlen-1; i++)
    {
        char key = *(bufin + i);
        if(symFreqMap.count(key) == 0) { symFreqMap.insert({key,1}); }
        else{ symFreqMap.at(key) += 1; }
    }
    print_map(symFreqMap); 
    //2.Sort the symbols from lease to most frequent aka min queue or minheap
    std::queue<char>sortedChars{};
    std::queue<int>sortedVals {};
    std::map<char,int> map = symFreqMap;
    for(int i = 0; i < symFreqMap.size(); i++)
    {
        char maxChar = 'm';
        int maxVal = 0;
        getMax(map, maxChar, maxVal);
        sortedChars.push(maxChar);
        sortedVals.push(maxVal);
    }
    std::cout << "Finished Encoding Input File\n";
    print_queue(sortedChars);
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
