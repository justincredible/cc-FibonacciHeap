#include "FibonacciHeap.h"
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

int main()
{
	int n, m;
	FibNode* temp, * first1, * first2;
	FibHeap* H1, * H2;
	srand(time(0));
	// input must be >= 0
	cin >> n >> m;
	if ( m < n )
	{
		m = n - m;
		n = n - m;
		m = n + m;
	}
	FibNode** Arr2;
	FibNode** Arr1 = new FibNode*[m];
	if ( n > 0 )
		Arr2 = new FibNode*[n];

	for ( int i = 0; i < m; i++ )
	{

		Arr1[i] = new FibNode;
		Arr1[i]->key = rand()%m;

		if ( i < n )
		{
			Arr2[i] = new FibNode;
			Arr2[i]->key = rand()%m;
		}
	}

	// TEST 0 make two Fibonacci heaps
	H1 = MakeHeap();
	H2 = MakeHeap();

	// TEST 1 insert nodes into two heaps
	for ( int i = 0; i < m; i++ )
	{
		Insert(H1,Arr1[i]);
		if ( i <  n )
			Insert(H2,Arr2[i]);
	}

	// TEST 2 initial extract min and successor extract min on two heaps
	if ( m > 0 )
	{
		first1 = temp = ExtractMin(H1);
		cout << "ExtractMin from H1: " << temp->key;
		temp = ExtractMin(H1);
		cout << " " << temp->key << endl;
		if ( n > 0 )
		{
			first2 = temp = ExtractMin(H2);
			cout << "ExtractMin from H2: " << temp->key;
			temp = ExtractMin(H2);
			cout << " " << temp->key << endl;
		}
	}

	// TEST 3 merge two heaps
	// H1 and H2 are deleted and H1 points to a new FibHeap
	if (rand()%2)
		H1 = Union(H1,H2);
	else
		H1 = Union(H2,H1);

	while ( H1->n > 0 )
	{
		temp = ExtractMin(H1);
		cout << temp->key << endl;
	}

	// TEST 4 reinitilialize heaps and compare first extractMins' keys
	// H1 and H2 are different heaps from before but will have the same nodes
	H2 = MakeHeap();
	for ( int i = 0; i < m; i++ )
	{
		Insert(H1,Arr1[i]);
		if ( i <  n )
			Insert(H2,Arr2[i]);
	}
	if ( m > 0 )
	{
		temp = ExtractMin(H1);
		cout << "from H1: " << temp->key << endl;
		if ( first1->key != temp->key )
			for ( int i = 1; i < m; i++ )
				cout << "H1 has different minimums\n";
		Insert(H1,temp);
		if ( n > 0 )
		{
			temp = ExtractMin(H2);
			cout << "from H2: " << temp->key << endl;
			if ( first2->key != temp->key )
				for ( int i = 1; i < n; i++ )
					cout << "H2 has different minimums\n";
			Insert(H2,temp);
		}
	}

	// TEST 4 delete nodes from two heaps
	if ( n > 0 )
	{
		cout << "Delete from H1: ";
		for ( int i = 0; i < m/n; i++ )
		{
			Delete(H1,Arr1[i]);
			if ( i == m/n-1 )
				cout << i+1 << " nodes";
		}
		cout << endl;
		cout << "Delete from H2: ";
		for ( int i = 0; i < n; i++ )
		{
			Delete(H2,Arr2[i]);
			if ( i == n-1 )
				cout << i+1 << " nodes";
		}
		cout << endl;
	}

	// TEST 5 decrease all remaining keys except the last to -1
	// also tests decrease key using a greater key than the node already has
	if ( n > 0 )
	{
		for ( int i = m/n; i < m-1; i++ )
		{
			DecreaseKey(H1,Arr1[i],-1);
			temp = ExtractMin(H1);
			cout << temp->key << endl;
		}
		cout << Arr1[m-1]->key << " ";
		DecreaseKey(H1,Arr1[m-1],2*Arr1[m-1]->key);
		temp = ExtractMin(H1);
		cout << temp->key << endl;
	}


	cout << "testing complete.";
	getchar(); getchar();
	temp = NULL;
	for ( int i = 0; i < m; i++ )
	{
		delete Arr1[i];
		if ( i < n )
			delete Arr2[i];
	}
	if ( m > 0 )
		delete [] Arr1;
	if ( n > 0 )
		delete [] Arr2;
	delete H1;
	return 0;
}