#include "huffman.h"
#include <iostream>
#include <map>

void print_map(std::map<char, int> mp)
{
    std::cout << "KEY\tELEMENT\n";
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) 
    {
        std::cout <<  itr->first << '\t' << itr->second << '\n';
    }
}
void print_queue(char *head, char *tail, std::map<char,int> mp)
{
    int len = tail-head;
    std::cout << len << std::endl; 
    std::cout << "KEY\tELEMENT\n";
    for(int i = 0; i < (len-1); i++)
    {
        char key = *(head+i);
//        std::cout << key << '\t' << mp.at(key) << '\n';
        std::cout << key << '\n';
    }
}
*char quickSortSymMap(std::map<char,int> mp)
{
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
    char minSym = symFreqMap.begin()->first;
    int minOcc = symFreqMap.begin()->second;
    //head of queue should point to the least frequent sym 
    char *head = &minSym;
    char *tail = head;
    for (auto itr = symFreqMap.begin(); itr != symFreqMap.end(); ++itr) 
    {
        char curSym = itr->first;
        int curOcc = itr->second;
        if (curOcc < minOcc)
        {
            std::cout << "Least Frequent is: " << curOcc << ":" << curSym;
            std::cout << " Old was: " << symFreqMap.at(*head) << ":" << *head << std::endl;
            head -= 1; 
            *head = curSym;
            minOcc = curOcc;
        }
        else
        {
            std::cout << "Tail is: " << curOcc << ":" << curSym;
            tail += 1;
            *tail = curSym;
        }
    }
    std::cout << "\nTail - Head = " << tail-head << "\n";
//    print_queue(head, tail, symFreqMap);
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
