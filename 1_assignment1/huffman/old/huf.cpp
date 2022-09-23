#include "huffman.h"
#include <iostream>
#include <unordered_map>
#include <stack>
#include <stdlib.h>
#include <string>


struct Node{
    bool isLeaf=false;
    bool traversed=false;
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
    minNode = heap.map.at(minKey);
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

int getAvailKey(NodeHeap& heap)
{
    int newKey;
    if(heap.availableKey.size() == 0){newKey = heap.map.size();}
    else{newKey = heap.availableKey.top();heap.availableKey.pop();}
    return newKey;
}
void addNode(NodeHeap& heap, Node newNode)
{
//    ppNodeHeap(heap);
//    std::cout << "\t...Adding: "; 
//    ppNode(newNode);
//   std::cout << newNode.frequency << ',' << newNode.isLeaf << '\n';
    int newKey = getAvailKey(heap);
    heap.map.insert({newKey, newNode});
//    ppNodeHeap(heap);
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
        int a = i-2;
        int b = i-1;
        Node n;
        n.frequency = (*(tree+a)).frequency + (*(tree+b)).frequency;
        n.isLeaf = false;
        n.character = '$';
        n.leftChild = tree+a;
        n.rightChild = tree+b;
//        ppNodeHeap(heap);
        addNode(heap, n);
//        std::cout << n.character << n.frequency << " left child: " << tree+a << ", ";
//        std::cout << "right child: " << tree+b << '\n';
        if(heap.map.size() == 1)
        {
            *(tree+i) = n;
            break;
        }
    }
}

void createCodeTable(std::unordered_map<char,std::string>& codeMap, Node* root, std::string code)
{
   Node& node = *root;
//   std::cout << "analyzing...";
//   ppNode(node);
   if(node.isLeaf) 
   {
       char key = node.character; 
       codeMap.insert({key, code}); 
//       std::cout << "FOUND LEAF: ";
//       std::cout << "Inserting " << key << ':' << code << '\n';
   }
   else
   {
      
      code += "!";
      code[code.size()-1] = '1';
//      std::cout << "searching left\n";
      createCodeTable(codeMap, node.leftChild, code); 
      code[code.size()-1] = '0';
//      std::cout << "searching right\n";
      createCodeTable(codeMap, node.rightChild, code); 
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
    std::cout << "\nEncoding Input File\n";
    std::string input_string = "";
    //1. create map of char and frequency 
    std::unordered_map<char, int> symFreqMap = {};
    for(int i = 0; i < bufinlen-1; i++)
    {
        char key = *(bufin + i);
        input_string += key;
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
    }
    NodeHeap nodeHeap;
    nodeHeap.map = nodeMap;
    //3. Construct the huffman tree 
    Node rootNode;
    int len = 2*nodeHeap.map.size()-1;
    Node* huffmanTree = (Node*) malloc(len*sizeof(Node));
    constructTree(nodeHeap, &(huffmanTree[0]));
    rootNode = huffmanTree[len-1];
    //4. Generate Huffman Code Table
    std::unordered_map<char,std::string> codeTable;
    std::string code = "";
    createCodeTable(codeTable, &huffmanTree[len-1], code);
    std::string codedMessage;
    for(int i = 0; i < bufinlen-1; i++)
    {
        char c = *(bufin + i);
        codedMessage += codeTable.at(c);
    }
    unsigned int bufLen = codedMessage.size();
    for(auto itr = codeTable.begin(); itr != codeTable.end(); ++itr)
    {
        bufLen += 2+itr->second.size(); 
    }
    unsigned char* compressedString = (unsigned char*) malloc(bufLen*sizeof(unsigned char));
    i = 0;
    for(auto itr = codeTable.begin(); itr != codeTable.end(); ++itr)
    {
        compressedString[i] = ',';
        i++;
        compressedString[i] = itr->first;
        i++;
        std::string& s = itr->second;
        for(int j=0; j < s.size(); j++)
        {
            compressedString[i] = s[j];
            i++;
        }
    }
    compressedString[i] = '\n';
    i++;
    for(int k = 0; k < codedMessage.size(); k++)
    {
        compressedString[i] = codedMessage[k];
        i++;
    }
    *pbufout = compressedString; 
    bufLen += 1;
    *pbufoutlen = bufLen;
    free(huffmanTree);
    std::cout << "input str..." << input_string << '(' << input_string.size() << ")\n";
    std::cout << "Finished Encoding Input File\n\n";
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
    bool readingHeader = true;
    std::unordered_map<std::string,unsigned char> decodeMap;
    int i = 0;
    unsigned char c = *bufin;
    std::string key = "$";
    std::string decodedString ="";
    unsigned char value = '-';
    while(c != '\n')
    {
        c = *(bufin+i);
        if(c==',')
        {
            i += 1;
            c = *(bufin+i);
            value = c;
            key = "";
            i += 1;
            c = *(bufin+i);
        }
        else
        {
            while(c != ',' and c != '\n')
            {
                key += c; 
                i += 1;
                c = *(bufin+i);
            }
            decodeMap.insert({key, value});
        }
    }
    i+=1;
    std::string temp = "";
    for(i; i < bufinlen; i++)
    {
        char c = *(bufin+i);
        temp += c;
        for(auto itr = decodeMap.begin(); itr != decodeMap.end(); ++itr)
        {
            if(temp == itr->first)
            {
                temp = "";
                decodedString += itr->second;
            }
        }
    }
//    decodedString += '\n';
    int bufLen = decodedString.size();
    int idx = 0;
    unsigned char* outputString = (unsigned char*) malloc(bufLen*sizeof(unsigned char));
    int x;
    for(int i = 0; i < bufLen; i++)
    {
        *(outputString+i) = (unsigned char) decodedString[i];
        x=i;
    }
    *pbufout = outputString;
    *pbufoutlen = bufLen;
    std::cout << "output str.." << decodedString << '(' << bufLen <<")\n";
    std::cout << "output buf.." << outputString << '(' << x << ")\n";
    std::cout << "\nDONE DECODEING!\n";
	return 0;
}
