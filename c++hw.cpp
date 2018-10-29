//
//  main.cpp
//  Huffmancode
//
//  Created by shuyu shen on 10/12/18.
//  Copyright © 2018 shuyu shen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>



//define a constant tree height
#define tree_height 100

/*******************************************Ndoe Class Decleration*************************************/
struct Node
{
    char data;  // One of the input characters
    unsigned freq;  // Frequency of the character
    Node *left, *right; // Left and right child of this node
    friend class MinHeap;// friend class  minheap
};
/*************************************MinHeap Class Decleration *********************************************************/
// a class to build a heap to store the characters
class MinHeap
{
//    friend class HuffmanTree;
    public :
        unsigned size;    // Current size of min heap
        unsigned capacity;   // capacity of min heap
        struct Node **array;  // Array of minheap node pointers
    
    //constructor
        MinHeap(){};
        MinHeap* createMinHeap(unsigned capacity);
    
    //insert function
        void insertion (MinHeap* minHeap, Node *minNode);
    //swap function
        void swapMinHeapNode(Node** a, Node** b);
    //heapfiy function to keep the heap properties
        void minHeapify(MinHeap* minHeap, int idx);
    //a function to test if the tree only has one node left. ending condition for while lop
        int onlyOneLeft(MinHeap* minHeap);
    //a function to extract the minimun node in the heap to build huffman tree
        Node* min_extract( MinHeap* minHeap);
    //function to build the heao
        void buildMinHeap( MinHeap* minHeap);
    // a function to build heap when passing the input char array, and int input array with frequency and the size
        MinHeap* minHeapConstructor(char data[], int freq[], int size);
    //assign binary code to an array
        void assignbinarytoArray(int arr[], int n);
//        int isLeaf(struct Node* root);
    
};
/***********************************************Hufffman class Decelration ***************************************/
struct HuffmanTree: public MinHeap
{
public:
    //constructor
    HuffmanTree(){
        
    };
    friend class MinHeap;
    Node* HuffmanTreeConstructor(char data[], int freq[], int size);
    
};//root node for the tree
Node *root;
/*******************************************Node Clas Implemenration ***************************************************************/
//Node constructor
 Node *createNode(char data, unsigned freq)
{
    Node *newNode = new Node;
    //assign pointer and data
    newNode->left = newNode->right = NULL;
    newNode->data = data;
//    std::cout << "input is "<< data;
    newNode->freq = freq;
//    std::cout << "input is "<< freq;
    return newNode;
}
//function create minheap
MinHeap* MinHeap::createMinHeap(unsigned capacity)
{
    
    MinHeap *minHeap = new MinHeap;
    //asiggn value to size, capcacity and array
    minHeap->size = 0;  // current size is 0
    minHeap->capacity = capacity;
    minHeap->array =
    (Node**)malloc(minHeap->capacity * sizeof(Node*));
    return minHeap;
}
//swap function
void MinHeap::swapMinHeapNode(Node** a,Node** b)
{
    Node* t = *a;
    *a = *b;
    *b = t;
}
//minheapify function keeps the heap proerties
void MinHeap::minHeapify( MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    //checking the value and update accordingly
    if (left < minHeap->size &&
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
     //checking the value and update accordingly
    if (right < minHeap->size &&
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    //making sure the top on the heap is the smallest
    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
//a helper function to check is the size equal to one
int MinHeap::onlyOneLeft(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
//a helper function to extract the min in the heap
Node* MinHeap::min_extract(struct MinHeap* minHeap)
{

    Node* result = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    
    --minHeap->size;
    minHeapify(minHeap, 0);
    return result;
}
//an insertion function to the heap
void MinHeap::insertion (MinHeap* minHeap, Node *minNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minNode->freq < minHeap->array[(i - 1)/2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    minHeap->array[i] = minNode;
}
//a function to build heap
void MinHeap::buildMinHeap(MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
//heap constructor
MinHeap* MinHeap::minHeapConstructor(char data[], int freq[], int size)
{
    //create heap base on the information pass in
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; i++)
    {
//        std::cout << "passing " << freq[i];
        minHeap->array[i] = createNode(data[i], freq[i]);
    }

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

/*************************************************Huffman Implementation **************************************/
//Huffman constructor
Node* HuffmanTree::HuffmanTreeConstructor(char data[], int freq[], int size)
{
    //pointers for the huffman tree
    Node *left, *right, *top;
//    std::cout << "value is "<< data[0] << "/n";
    //call the constructor for the heap
    MinHeap* minHeap = minHeapConstructor(data, freq, size);
    // while there are more than one node loops through the heap
    while (!onlyOneLeft(minHeap))
    {
        // extract two symbols with least frequencies.
        left = min_extract(minHeap);
        right = min_extract(minHeap);
        //create a node with the sum of the two frequencies  and insert it back to the heap and insert the node to the encoding tree
        //becoming child nodes for root node.
        top = createNode('*', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertion(minHeap, top);
    }
    // when there is only one node left, the reamining node is the root node and tree is complete
    return min_extract(minHeap);
}
/*****************************Function to Assign binary code and Other helper function *******************/
//function to assign the bianry code
void assignBinaryCode(Node* root, int arr[], int top);
//a function to test if the node is the leaf
int isLeaf(struct Node* root);
//function to assign binary code to array and print it out
void assignbinarytoArray(int arr[], int n);

/****************************************main function **********************************************************/
int main()
{
//    testing with different input size from n =6 to n= 60000;
    char data[6];
    int freq[6];
    //assign random characters to input array
    for (int i =0;i < 6;i++)
    {
        data[i]=rand() % 26 + 65;
    }
    //assign random frequencies to frequncey array
    for (int i =0;i < 6;i++)
    {
        freq[i]=rand();
    }

    int size = sizeof(data)/sizeof(data[0]);
    //initiliaze an empty tree;
    HuffmanTree mytree;
    //calculating runtime
    auto start = std::chrono::high_resolution_clock::now();
    //building a huffman tree
    Node * root= mytree.HuffmanTreeConstructor(data, freq, size);
    int arr[tree_height];
    int top=0;
    //assigning binary code
    assignBinaryCode(root,arr,top);
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
    return 0;
}
//isLeaf implementation
int isLeaf(struct Node* root)
{
    return !(root->left) && !(root->right);
}
//assigning binary code in array to each node function
void assignbinarytoArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        std::cout<<arr[i]<<std::endl;
        
    }
}
//assigning binary code to each node in the tree
void assignBinaryCode(Node* root, int input[], int top)
{
    // Assign 0 to left edge recurseivly
    if (root->left)
    {
        input[top] = 0;
        assignBinaryCode(root->left, input, top + 1);
    }
    // Assign 1 to right edge recurseivly
    if (root->right)
    {
        input[top] = 1;
        assignBinaryCode(root->right, input, top + 1);
    }
// If this is a leaf node, then it contain the input characters, print the character nad binary code out
    if (isLeaf(root))
    {
        //print out the data and the code
        std::cout << root->data << ":" << std::endl;
        //assign the binary code
        assignbinarytoArray(input, top);
    }
}
