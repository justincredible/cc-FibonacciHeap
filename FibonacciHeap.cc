/*
 * Algorithm from: Cormen et al. (2009) "Fibonacci Heaps," in Introduction to Algorithms, 3rd ed. Cambridge: MIT Press, pp. 505-530.
 * consider all references as paraphrases
 *
 * THIS SHOULD NOT BE USED AS IS: potential memory leaks
 *								  no error checking on memory allocation
 *								  direct access to data structures
 *
 */

#include <cmath> // I still can't write an accurate logarithmic function.

#include "FibonacciHeap.h"
// Peripherals.h can be replaced by a standard library: include climits and changing delete to decrease the key to INT_MIN
//														include cstdlib and changing NILs to NULLs
#include "Peripherals.h"

using namespace std;

/*
 * . . . allocates and returns the Fibonacci heap object H, where 
 * H.n = 0 and H.min = NIL;
 */
FibHeap* MakeHeap()
{
	FibHeap* H;
	
	H = new FibHeap;
	H->n = 0;
	H->min = NIL;

	return H;
}

/*
 * Insert(H,x)
 * 1. x.degree = 0
 * 2. x.p = NIL
 * 3. x.child = NIL
 * 4. x.mark = FALSE
 * 5. if H.min == NIL
 * 6. 	create a root list for H containing just x
 * 7. 	H.min = x
 * 8. else insert x into H's root list
 * 9. 	if x.key < H.min.key
 *10. 		H.min = x
 *11. H.n = H.n + 1
 */
void Insert( FibHeap* H, FibNode* x )
{
	// 1
	x->degree = 0;
	// 2
	x->p = NIL;
	// 3
	x->child = NIL;
	// 4
	x->mark = false;
	// 5
	if ( H->min == NIL )
	{
		// 6, 7
		H->min = x->left = x->right = x;
	}
	else
	{
		// 8
		H->min->left->right = x;
		x->left = H->min->left;
		H->min->left = x;
		x->right = H->min;
		// 9
		if ( x->key < H->min->key )
		{
			// 10
			H->min = x;
		}
	}
	// 11
	H->n++;
}

/*
 * The minimum node of a Fibonacci Heap H is given by the pointer H.min
 * . . .
 */
FibNode* Minimum( FibHeap* H )
{
	return H->min;
}

/*
 * Union(H1,H2)
 * 1. H = MAKE-FIB-HEAP()
 * 2. H.min = H1.min
 * 3. concatenate the root list of H2 with the root list of H
 * 4. if (H1.min == NIL) or (H2.min != NIL and H2.min.key < H1.min.key)
 * 5. 	H.min = H2.min
 * 6. H.n = H1.n + H2.n
 * 7. return H
 */
FibHeap* Union( FibHeap* H1, FibHeap* H2 )
{
	FibHeap* H;
	
	// 1
	H = MakeHeap();
	// 2
	H->min = H1->min;
	// 3
	if ( H->min != NIL && H2->min != NIL )
	{
		H->min->right->left = H2->min->left;
		H2->min->left->right = H->min->right;
		H->min->right = H2->min;
		H2->min->left = H->min;
	}
	// 4
	if ( H1->min == NIL || ( H2->min != NIL && H2->min->key < H1->min->key ) )
	{
		// 5
		H->min = H2->min;
	}
	// 6
	H->n = H1->n + H2->n;
	// 7
	delete H1;
	delete H2;

	return H;
}

/*
 * ExtractMin(H)
 * 1. z = H.min
 * 2. if z != NIL
 * 3. 	for each child x of z
 * 4. 		add x to the root list of H
 * 5. 		x.p = NIL
 * 6. 	remove z from the root list of H
 * 7.		if z == z.right
 * 8. 		H.min = NIL
 * 9. 	else H.min = z.right
 *10. 		CONSOLIDATE(H)
 *11. 	H.n = H.n - 1
 *12. return z
 */
FibNode* ExtractMin( FibHeap* H )
{
	FibNode* z, * x, * next;
	FibNode** childList;
	
	// 1
	z = H->min;
	// 2
	if ( z != NIL )
	{
		// 3
		x = z->child;
		if ( x != NIL )
		{
			childList = new FibNode*[z->degree];
			next = x;
			for ( int i = 0; i < (int)z->degree; i++ )
			{
				childList[i] = next;
				next = next->right;
			}
			for ( int i = 0; i < (int)z->degree; i++ )
			{
				x = childList[i];
				// 4
				H->min->left->right = x;
				x->left = H->min->left;
				H->min->left = x;
				x->right = H->min;
				// 5
				x->p = NIL;
			}
			delete [] childList;
		}
		// 6
		z->left->right = z->right;
		z->right->left = z->left;
		// 7
		if ( z == z->right )
		{
			// 8
			H->min = NIL;
		}
		else
		{
			// 9
			H->min = z->right;
			// 10
			Consolidate(H);
		}
		// 11
		H->n--;
	}
	// 12
	return z;
}

/*
 * Consolidate(H)
 * 1. let A[0 . . D(H.n)] be a new array
 * 2. for i = 0 to D(H.n)
 * 3. 	A[i] = NIL
 * 4. for each node w in the root list of H
 * 5. 	x = w
 * 6. 	d = x.degree
 * 7. 	while A[d] != NIL
 * 8. 		y = A[d]
 * 9. 		if x.key > y.key
 *10.			exchange x with y
 *11. 		FIB-HEAP-LINK(H,y,x)
 *12. 		A[d] = NIL
 *13. 		d = d + 1
 *14. 	A[d] = x
 *15. H.min = NIL
 *16. for i = 0 to D(H.n)
 *17. 	if A[i] != NIL
 *18. 		if H.min == NIL
 *19. 			create a root list for H containing just A[i]
 *20. 			H.min = A[i]
 *21. 		else insert A[i] into H's root list
 *22. 			if A[i].key < H.min.key
 *23. 				H.min = A[i]
 */
void Consolidate( FibHeap* H )
{
	FibNode* w, * next, * x, * y, * temp;
	FibNode** A, ** rootList;
	// Max degree <= log base golden ratio of n
	unsigned int d, rootSize;
	int max_degree = floor(log(double(H->n))/log(double(1 + sqrt(double(5)))/2));
	
	// 1
	A = new FibNode*[max_degree+2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
	// 2
	for ( int i = 0; i < max_degree+2; i++ )
	{
		// 3
		A[i] = NIL;
	}
	// 4
	w = H->min;
	rootSize = 0;
	next = w;
	do
	{
		rootSize++;
		next = next->right;
	} while ( next != w );
	rootList = new FibNode*[rootSize];
	for ( int i = 0; i < (int)rootSize; i++ )
	{
		rootList[i] = next;
		next = next->right;
	}
	for ( int i = 0; i < (int)rootSize; i++ )
	{
		w = rootList[i];
		// 5
		x = w;
		// 6
		d = x->degree;
		// 7
		while ( A[d] != NIL )
		{
			// 8
			y = A[d];
			// 9
			if ( x->key > y->key )
			{
				// 10
				temp = x;
				x = y;
				y = temp;
			}
			// 11
			FibHeapLink(H,y,x);
			// 12
			A[d] = NIL;
			// 13
			d++;
		}
		// 14
		A[d] = x;
	}
	delete [] rootList;
	// 15
	H->min = NIL;
	// 16
	for ( int i = 0; i < max_degree+2; i++ )
	{
		// 17
		if ( A[i] != NIL )
		{
			// 18
			if ( H->min == NIL )
			{
				// 19, 20
				H->min = A[i]->left = A[i]->right = A[i];
			}
			else
			{
				// 21
				H->min->left->right = A[i];
				A[i]->left = H->min->left;
				H->min->left = A[i];
				A[i]->right = H->min;
				// 22
				if ( A[i]->key < H->min->key )
				{
					// 23
					H->min = A[i];
				}
			}
		}
	}
	delete [] A;
}

/*
 * FibHeapLink(H,y,x)
 * 1. remove y from the root list of H
 * 2. make y a child of x, incrementing x.degree
 * 3. y.mark = FALSE
 */
void FibHeapLink( FibHeap* H, FibNode* y, FibNode* x )
{
	// 1
	y->left->right = y->right;
	y->right->left = y->left;
	// 2
	if ( x->child != NIL )
	{
		x->child->left->right = y;
		y->left = x->child->left;
		x->child->left = y;
		y->right = x->child;
	}
	else
	{
		x->child = y;
		y->right = y;
		y->left = y;
	}
	y->p = x;
	x->degree++;
	// 3
	y->mark = false;
}

/*
 * DecreaseKey(H,x,k)
 * 1. if k > x.key
 * 2. 	error "new key is greater than current key"
 * 3. x.key = k
 * 4. y = x.p
 * 5. if y != NIL and x.key < y.key
 * 6. 	CUT(H,x,y)
 * 7. 	CASCADING-CUT(H,y)
 * 8. if x.key < H.min.key
 * 9. 	H.min = x
 */
void DecreaseKey( FibHeap* H, FibNode* x, int k )
{
	FibNode* y;
	
	// 1
	if ( k > x->key )
	{
		// 2
		// error( "new key is greater than current key" );
		return;
	}
	// 3
	x->key = k;
	// 4
	y = x->p;
	// 5
	if ( y != NIL && x->key < y->key )
	{
		// 6
		Cut(H,x,y);
		// 7
		CascadingCut(H,y);
	}
	// 8
	if ( x->key < H->min->key )
	{
		// 9
		H->min = x;
	}
}

/*
 * Cut(H,x,y)
 * 1. remove x from the child list of y, decrementing y.degree
 * 2. add x to the root list of H
 * 3. x.p = NIL
 * 4. x.mark = FALSE
 */
void Cut( FibHeap* H, FibNode* x, FibNode* y )
{
	// 1
	if ( x->right == x )
	{
		y->child = NIL;
		y->degree--;
	}
	else
	{
		x->right->left = x->left;
		x->left->right = x->right;
		if ( y->child == x )
			y->child = x->right;
		y->degree--;
	}
	// 2
	H->min->right->left = x;
	x->right = H->min->right;
	H->min->right = x;
	x->left = H->min;
	// 3
	x->p = NIL;
	// 4
	x->mark = false;
}
 /*
  * CascadingCut(H,y)
  * 1. z = y.p
  * 2. if z != NIL
  * 3. 	if y.mark == FALSE
  * 4. 		y.mark = TRUE
  * 5. 	else CUT(H,y,z)
  * 6. 		CASCADING-CUT(H,z)
  */
void CascadingCut( FibHeap* H, FibNode* y )
{
	FibNode* z;
	
	// 1
	z = y->p;
	// 2
	if ( z != NIL )
	{
		// 3
		if ( y->mark == false )
		{
			// 4
			y->mark = true;
		}
		else
		{
			// 5
			Cut(H,y,z);
			// 6
			CascadingCut(H,z);
		}
	}
}

/*
 * Trivial to understand
 * edited to prevent inherent memory leak in non-garbage collected languages
 */
FibNode* Delete( FibHeap* H, FibNode* x )
{
	DecreaseKey(H,x,negInfinity());
	return ExtractMin(H);
}