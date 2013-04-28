#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

// DATA STRUCTURES
struct FibNode
{
	int key;
	//type data/payload

	bool mark;
	
	FibNode* p;
	FibNode* left;
	FibNode* right;
	FibNode* child;
	unsigned int degree;
};

struct FibHeap
{	
	unsigned int n;
	FibNode* min;
};

// OPERATIONS
// Mergeable heap
FibHeap* MakeHeap();
void Insert( FibHeap* H, FibNode* x );
FibNode* Minimum( FibHeap* H );
FibHeap* Union( FibHeap* H1, FibHeap* H2 );
FibNode* ExtractMin( FibHeap* H );
void Consolidate( FibHeap* H );
void FibHeapLink( FibHeap* H, FibNode* y, FibNode* x );
// Fibonacci heap
void DecreaseKey( FibHeap* H, FibNode* x, int k );
void Cut( FibHeap* H, FibNode* x, FibNode* y );
void CascadingCut( FibHeap* H, FibNode* y );
FibNode* Delete( FibHeap* H, FibNode* x );

#endif 
