#include "huffman.h"
#include <iostream>
#include <unordered_map>
#include <stack>
#include <stdlib.h>
#include <string>


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
    //create map of char and frequency 
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
    //transfer data to a node Heap 
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
    //Construct the huffman tree 
    Node rootNode;
    int len = 2*nodeHeap.map.size()-1;
    Node* huffmanTree = (Node*) malloc(len*sizeof(Node));
    constructTree(nodeHeap, &(huffmanTree[0]));
    rootNode = huffmanTree[len-1];
    //Traverse Tree and Generate Huffman Code Table
    std::unordered_map<char,std::string> codeTable;
    std::string code = "";
    createCodeTable(codeTable, &huffmanTree[len-1], code);
    //Make a string of 1's and zeros based on huffman Table
    std::string codedMessage;
    for(int i = 0; i < bufinlen-1; i++)
    {
        char c = *(bufin + i);
        codedMessage += codeTable.at(c);
    }
    //Make a string to represent the codeTable
    std::string codedCodeTable="";
    for(auto itr = codeTable.begin(); itr!=codeTable.end(); ++itr)
    {
        codedCodeTable += ",";
        codedCodeTable += itr->first;
        codedCodeTable += itr->second;
    }
    codedCodeTable += '\n';
    int codedCodeTableSize = codedCodeTable.size();
    //Make a string for the code header
    int codeHeaderSie = 2;
    unsigned char codeHeader[2];
    codeHeader[0] = (unsigned char) (8-codedMessage.size()%8);
    codeHeader[1] = '\n';
    //Compress the coded Message to byte array
    int compressedMessageSize = (int) codedMessage.size()/8;
    compressedMessageSize += 1;
    unsigned char compressedMessage[compressedMessageSize];
//    std::cout << "\n********\n";
    std::string temp = "";
    int k = 0;
    for(int i=0; i<8*compressedMessageSize; i++)
    {
        if(i < codedMessage.size())
        {
            temp += codedMessage[i];
        }
        else
        {
//            std::cout << "zero padding\n";
            temp += '0';
        }
        if(temp.size()%8 == 0)
        {
//            std::cout << temp << '\n';
            unsigned char curByte = 0x00; 
            for(int j = 0; j < 8; j++)
            {
                char bit = temp[j];
                curByte <<= 1;
                if (bit=='1'){
                    curByte |= 0x01;
                }
            }
            temp = "";
            compressedMessage[k] = curByte;
            k+=1;
        }
    }
     
    std::cout << codedCodeTableSize << ", " << codeHeaderSie << ", " << compressedMessageSize << '\n';
    std::cout << codedCodeTableSize + codeHeaderSie + compressedMessageSize;
//    std::cout << "\n********\n";
//    std::cout << input_string << '\n';
//    std::cout << codedCodeTable;
//    std::cout << codedMessage << "("<< codedMessage.size()<< ")\n";
//    for(int d = 0; d<compressedMessageSize; d++)
//    {
//        std::cout << std::hex << (int)compressedMessage[d] << '\n';
//    }
//    std::cout << "\n********\n";
    //Allocate memory for pbufout to hold table, code header, and compressedMessage 
    int outStreamSize = codedCodeTableSize + codeHeaderSie + compressedMessageSize;
    unsigned char* outStream = (unsigned char*) malloc(outStreamSize*sizeof(unsigned char));
    //fill the outStream with unsigned chars
    for(int i = 0; i < outStreamSize; i++)
    {
        if (i < codedCodeTableSize)
        {
            outStream[i] = (unsigned char) codedCodeTable[i];
        }
        else if(i < codedCodeTableSize + codeHeaderSie)
        {
            outStream[i] = codeHeader[i-(codedCodeTableSize)];
        }
        else
        {
            outStream[i] = compressedMessage[i-(codedCodeTableSize+codeHeaderSie)];
        }
    }
    //set pbufout and pbufoutlen
    *pbufoutlen = outStreamSize;
    *pbufout = outStream;
    //free memory
    free(huffmanTree);
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
    std::cout << "\nDecoding\n";
    //convert first line of buf in to a hash map
    std::unordered_map<std::string,unsigned char> decodeMap;
    int i = 0;
    unsigned char c = *bufin;
    std::string key = "$";
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
    i+= 1;
    //convert the second line to an integer
    int pad = *(bufin+i);
    i+=2;
    //convert the third line to a char array of 1s and 0s
    std::string bitString = "";
    while(i < bufinlen)
    {
        unsigned char byte = *(bufin +i);
      //  std::cout << '\n';
//        std::cout << std::hex << (int) byte << '\n';
        for(int k=0; k<8; k++)
        {
            bool isOne = ((byte >> (7-k)) & 1);
            if(isOne)
            {
//                std::cout << '1';
                bitString += '1';
            }
            else
            {
//               std::cout << '0';
                bitString += '0';
            }
        }
//        std::cout << '\n';
        i+=1;
    }
    //loop through char array and crate output char array
    std::string temp = "";
    std::string decodedString = "";
    int len = bitString.size()-(int)pad;
//    std::cout << '\n' << bitString << '(' << std::dec <<bitString.size() << ',' << len << ')' << '\n';
    for(int i; i < len; i++)
    {
        
        temp += bitString[i];
        for(auto itr = decodeMap.begin(); itr != decodeMap.end(); ++itr)
        {
            if(temp == itr->first)
            {
                temp = "";
                decodedString += itr->second;
            }
        }
    }
//    std::cout << '\n' << decodedString << '(' << std::dec <<decodedString.size() << ')'<< '\n';
    //allocate memory for the ouput buffer
    int outStreamSize = decodedString.size()+1; 
    unsigned char* outStream = (unsigned char*) malloc(outStreamSize*sizeof(unsigned char));
    //convert the decoded string to character array
    for(int i = 0; i < outStreamSize; i++)
    {
        outStream[i] = decodedString[i];
    }
    *pbufoutlen = outStreamSize;
    *pbufout = outStream;
    std::cout << "\nDone Decoding\n";
	return 0;
}
