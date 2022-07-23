#include "huffman.h"
#include <iostream>
#include <unordered_map>
#include <stack>
#include <stdlib.h>


struct Node{
    bool isLeaf=false;
    char character='*';
    int frequency=0;
    Node* parent=nullptr;
    Node* leftChild=nullptr;
    Node* rightChild=nullptr;
};
void ppNode(Node n)
{
    std::cout << "char:" << n.character << ", ";
    std::cout << "freq:" << n.frequency << ", ";
    std::cout << "ilef:" << n.isLeaf << ", ";
    std::cout << "rcld:" << n.rightChild << ", ";
    std::cout << "lcld:" << n.leftChild << ", ";
    std::cout << "prnt:" << n.parent<< '\n';
}
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
//    std::cout << "...Popping:" << minKey << ", " << min.character << ", " << min.frequency << std::endl;
    heap.map.erase(minKey);
    heap.availableKey.push(minKey);
}



//helper print functions
void ppNodeHeap(NodeHeap h)
{
    std::cout << h.map.size() << std::endl;
    int len = h.map.size();
    for(int i = 0; i< len; i++)
    {
        Node x;
        x.character = '^';
        x.frequency = -10;
        popMin(h, x);
        ppNode(x);
    }
}

void addNode(NodeHeap& heap, Node newNode)
{
    int newKey;
    if(heap.availableKey.size() == 0){newKey = heap.map.size();}
    else{newKey = heap.availableKey.top();heap.availableKey.pop();}
    ppNodeHeap(heap);
    std::cout << "\t...Adding: "; 
    ppNode(newNode);
 //   std::cout << newNode.frequency << ',' << newNode.isLeaf << '\n';
    heap.map.insert({newKey, newNode});
    ppNodeHeap(heap);
}

void constructTree(NodeHeap& heap, Node *tree)
{
    int i=0;
    while(true)
    {
        popMin(heap, *(tree+i));
        i+=1;
        popMin(heap, *(tree+i));
        i+=1;
        Node& n = *(tree+i);
        int a = i-2;
        int b = i-1;
        n.frequency = (*(tree+a)).frequency + (*(tree+b)).frequency;
        n.isLeaf = false;
        n.character = '$';
        n.leftChild = tree+a;
        n.rightChild = tree+b;
//        std::cout << n.character << n.frequency << " left child: " << tree+a << ", ";
//        std::cout << "right child: " << tree+b << '\n';
        heap.map.insert({100+i,n});
//        ppNodeHeap(heap);
//        addNode(heap, n);
        if(heap.map.size() == 1)
        {
            *(tree+i) = n;
            break;
        }
    }
}

void createCodeTable(std::unordered_map<char,int>& codeMap, Node* tree, int treeSize)
{
   std::cout << "MAKING CODE TABLE\n";
   Node* x = (tree+treeSize-1); 
   bool done = false;
   int i = 0;
   while (!done)
   {
      Node temp = *x;
      x = temp.rightChild;
      std::cout << temp.character << ',' << temp.frequency << std::endl; 
      if(temp.isLeaf)
      {
        done = true;
      }
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
 //       std::cout <<  n.isLeaf << ", " << n.character  << ", " << n.frequency << '\n';
    }
    NodeHeap nodeHeap;
    nodeHeap.map = nodeMap;
//    ppNodeHeap(nodeHeap);
    //3. Construct the huffman tree 
    Node rootNode;
    int len = 2*nodeHeap.map.size()-1;
    Node* huffmanTree = (Node*) malloc(len*sizeof(Node));
    constructTree(nodeHeap, huffmanTree);
    rootNode = huffmanTree[len-1];
    for(int i = 0; i < len; i++)
    {
        ppNode(huffmanTree[i]);
    }
    //4.
    std::unordered_map<char,int> codeTable;
//    createCodeTable(codeTable, &huffmanTree[0], len);

    std::cout << "Finished Encoding Input File\n";
    free(huffmanTree);
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
