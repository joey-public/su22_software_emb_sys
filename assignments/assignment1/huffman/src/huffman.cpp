#include "huffman.h"
#include <iostream>
#include <unordered_map>
#include <stack>


struct Node{
    bool isLeaf;
    char character;
    int frequency;
    Node* parent;
};
struct NodeHeap{
    std::stack<int> availableKey;
    std::unordered_map<int, Node> map;
};

void popMin(NodeHeap& heap, Node& minNode)
{
    if(heap.map.size() == 0){return;} 
    Node min = heap.map.begin()->second;
    int minKey = heap.map.begin()->first;
    for (auto itr = heap.map.begin(); itr != heap.map.end(); itr++)
    {
        if(itr->second.frequency < min.frequency)
        {
            min = itr->second;
            minKey = itr->first;
        }
    }
    minNode.isLeaf = min.isLeaf;
    minNode.character = min.character;
    minNode.frequency = min.frequency;
//    std::cout << "\tPopping:" << minKey << ", " << min.character << ", " << min.frequency << std::endl;
    heap.map.erase(minKey);
    heap.availableKey.push(minKey);
}

void addNode(NodeHeap& heap, Node& newNode)
{
    int newKey;
    if(heap.availableKey.size() == 0){newKey = heap.map.size();}
    else{newKey = heap.availableKey.top();heap.availableKey.pop();}
    std::cout << "Adding: " << newKey << newNode.character << std::endl;
    heap.map.insert({newKey, newNode});
}

//helper print functions
void ppNodeHeap(NodeHeap h)
{
    std::cout << h.map.size() << std::endl;
    int len = h.map.size();
    for(int i = 0; i< len-1; i++)
    {
        Node x;
        x.character = '^';
        x.frequency = -10;
        popMin(h, x);
        std::cout << x.character << " , " << x.frequency << " , " << x.isLeaf << '\n';
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
    //1. create map of char and frequency 
    std::unordered_map<char, int> symFreqMap = {};
    for(int i = 0; i < bufinlen-1; i++)
    {
        char key = *(bufin + i);
        if(symFreqMap.count(key) == 0)
        {
            symFreqMap.insert({key,1}); 
        }
        else{ symFreqMap.at(key) += 1; }
    }
    //2. transfer data to a node Heap 
    std::unordered_map<int, Node> nodeMap= {};
    int i = 0;
    for(auto itr=symFreqMap.begin(); itr!=symFreqMap.end(); ++itr)
    {
        Node n;
        n.isLeaf = true;
        n.character = itr->first;
        n.frequency = itr->second;
        nodeMap.insert({i,n});
        i ++;
        std::cout <<  n.isLeaf << ", " << n.character  << ", " << n.frequency << '\n';
    }
    NodeHeap nodeHeap;
    nodeHeap.map = nodeMap;
    ppNodeHeap(nodeHeap);
    //3. pop the smallest two nodes;
    Node a, b;
    popMin(nodeHeap,a);
    popMin(nodeHeap,b);
    Node c;
    c.isLeaf = false;
    c.character = '$';
    c.frequency = a.frequency + b.frequency;
    a.parent = &c;
    b.parent = &c;
    addNode(nodeHeap, c);
    ppNodeHeap(nodeHeap);

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
