// Aaron Zeile
// azeile
// pa4
// Matrix.c

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "Matrix.h"
#include "List.h"

typedef struct EntryObj *Entry;

typedef struct EntryObj {
	int col;
	double val;
} EntryObj;

typedef struct MatrixObj {
	MatrixElement *row;
	Entry info;
	int size;
	int nnz;
} MatrixObj;

// constructors/ destructors ------------------------------------------

// returns a new matrix entry object
Entry newEntry(int c, double v) {
	Entry E = malloc(sizeof(EntryObj));
	assert (E != NULL);
	
	E->col = c;
	E->val = v;
	return E;
}

// frees allocated memory for entries
void freeEntry(Entry *pE) {
	if (*pE && pE) {
		free(*pE);
		*pE = NULL;
	}
}

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n) {
	// allocate space for matrix
	Matrix M = malloc(sizeof(MatrixObj));
	assert (M != NULL);
	
	// set attributes of matrix
	M->size = n+1;
	M->nnz = 0;
	// allocate space for row lists
	M->row = calloc(n+1, sizeof(List));

	// create zero state
	for (int i = 1; i < n+1; i++) {
		// fill rows with lists
		M->row[i] = newList();
	}

	return M;
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM) {
	if (*pM && pM) {
		int n = size(*pM)+1;
		// go through each row
		for (int i = 1; i < n; i++) {
			List L = (*pM)->row[i];
			// go through each entry
			moveFront(L);
			while (index(L) >= 0) {
				Entry E = get(L);
				freeEntry(&E);
				moveNext(L);
			}
			freeList(&L);
		}
		// free rows array
		free((*pM)->row);
		(*pM)->row = NULL;

		// free matrix
		free(*pM);
		*pM = NULL;
	}
}

// access functions -------------------------------------------------

// size()
// Return the size of square Matrix M.
int size(Matrix M) {
	return M->size-1;
}
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M) {
	return M->nnz;
}
// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) {
	// if sizes aren't equivalent
	if (size(A) != size(B)) {
		return 0;
	}
	// go through rows
	int n = size(A)+1;
	for (int i = 1; i < n; i++) {
		List a = A->row[i];
		List b = B->row[i];

		if (length(a) != length(b)) {
			return 0;
		}

		// check entries
		moveFront(a);
		moveFront(b);
		while (index(a) >= 0) {
			Entry aE = get(a);
			Entry bE = get(b);
			// if any entries aren't equal
			if (aE->val != bE->val) {
				return 0;
			}
			moveNext(a);
			moveNext(b);
		}
	}
	// if we reach the end, return 1
	return 1;
}

// manipulation procedures ---------------------------------------------

// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M) {
	// go through every row
	for (int i = 1; i < size(M); i++) {
		List L = M->row[i];
		// clear every row and set every entry to 0
		while (length(L) > 0) {
			moveFront(L);
			Entry E = get(L);
			freeEntry(&E);
			deleteFront(L);
		}
	}
	M->nnz = 0;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
	// preconditions
	if (i < 1 || i > size(M)) {	
		fprintf(stderr, "Error. Tried changeEntry with i out of bounds\n");
		exit(EXIT_FAILURE);
	}
	else if (j < 1 || j > size(M)) {
		fprintf(stderr, "Error. Tried changeEntry with j out of bounds\n");
		exit(EXIT_FAILURE);
	}

	// get list to traverse
	List L = M->row[i];
	
	// if row is empty
	if (!(length(L) > 0)) {
		if (x != 0.0) {
			append(L, newEntry(j, x));
			M->nnz++;
		}
		return;
	}

	// if row not empty, insert entry in correct column
	moveFront(L);
	while (index(L) >= 0) {
		Entry E = get(L);
		// if column has a value
		if (E->col == j) {
			freeEntry(&E);
			// if x is not zero
			if (x != 0.0) {
				set(L, newEntry(j, x));
				M->nnz++;
			}
			// if x is zero
			else {
				delete(L);
				M->nnz--;
			}
			break;
		}
		// if column was empty
		else if (E->col > j) {
			// if x is not zero
			if (x != 0.0) {
				insertBefore(L, newEntry(j, x));
				M->nnz++;
			}
			break;
		}
		moveNext(L);
	}

	// if we went past the end
	if (index(L) == -1 && x != 0.0) {
		append(L, newEntry(j, x));
		M->nnz++;
	}

}

// matrix arithmetic operations --------------------------------------------

// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
	Matrix B = newMatrix(size(A));
	int n = size(A)+1;

	// go through rows	
	for (int i = 1; i < n; i++) {
		List a = A->row[i];
		List b = B->row[i];

		moveFront(a);
		while (index(a) >= 0) {
			Entry curr = get(a);

			Entry E = newEntry(curr->col, curr->val);
			append(b, E);
			B->nnz++;

			moveNext(a);
		}
	}
	
	return B;
}
// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A) {
	Matrix T = newMatrix(size(A));

	int n = size(A)+1;

	for (int i = 1; i < n; i++) {
		List L = A->row[i];
		
		moveFront(L);
		while (index(L) >= 0) {
			Entry E = get(L);
			changeEntry(T, E->col, i, E->val);
			moveNext(L);
		}
	}
	
	return T;
}
// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
	Matrix M = newMatrix(size(A));

	int n = size(A)+1;
	for (int i = 1; i < n; i++) {
		List R = A->row[i];
		List L = M->row[i];
		moveFront(R);
		while (index(R) >= 0) {
			Entry E = get(R);

			Entry N = newEntry(E->col, (x * E->val));
			if (N->val != 0.0) {
				append(L, N);
				M->nnz++;
			}

			moveNext(R);
		}
	}

	return M;
}

// helper function for sum
void add(Matrix M, List C, List A, List B) {
	moveFront(A);
	moveFront(B);

	while (index(A) >= 0 && index(B) >= 0) {
		Entry a = get(A);
		Entry b = get(B);

		if (a->col == b->col) {
			double value = a->val + b->val;
			if (value != 0.0) {
				Entry E = newEntry(a->col, value);
				append(C, E);
				M->nnz++;	
			}
			moveNext(A);
			moveNext(B);
		}
		else if (a->col < b->col) {
			Entry E = newEntry(a->col, a->val);
			append(C, E);
			M->nnz++;
			
			moveNext(A);
		}
		else {
			Entry E = newEntry(b->col, b->val);
			append(C, E);
			M->nnz++;
			
			moveNext(B);
		}
	}

	// Handle stragglers, if any	
	while (index(A) >= 0) {
        	Entry a = get(A);

        	Entry E = newEntry(a->col, a->val);
        	append(C, E);
		M->nnz++;

        	moveNext(A);
    	}
    	while (index(B) >= 0) {
        	Entry b = get(B);

        	Entry E = newEntry(b->col, b->val);
        	append(C, E);
		M->nnz++;

        	moveNext(B);
    	}	
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {	
	// if sizes aren't equal
	if (size(A) != size(B)) {
		fprintf(stderr, "Error. Tried sum on different sized matrices\n");
		exit(EXIT_FAILURE);
	}

	// use copy in case A = B
	Matrix C = copy(B);

	Matrix M = newMatrix(size(A));

	// go through rows
	int n = size(A)+1;
	for (int i = 1; i < n; i++) {
		List a = A->row[i];
		List b = C->row[i];
		List m = M->row[i];
		
		add(M, m, a, b);
	}

	freeMatrix(&C);	
	// return sum matrix
	return M;
}

// helper function for diff
void sub(Matrix M, List C, List A, List B) {
	moveFront(A);
	moveFront(B);

	while (index(A) >= 0 && index(B) >= 0) {
		Entry a = get(A);
		Entry b = get(B);

		if (a->col == b->col) {
			double value = a->val - b->val;
			if (value != 0.0) {
				Entry E = newEntry(a->col, value);
				append(C, E);
				M->nnz++;
			}
			moveNext(A);
			moveNext(B);
		}
		else if (a->col < b->col) {
			Entry E = newEntry(a->col, a->val);
			append(C, E);
			M->nnz++;

			moveNext(A);
		}
		else {
			Entry E = newEntry(b->col, 0.0 - b->val);
			append(C, E);
			M->nnz++;

			moveNext(B);
		}
	}

	// Handle stragglers, if any	
	while (index(A) >= 0) {
        	Entry a = get(A);

		Entry E = newEntry(a->col, a->val);
        	append(C, E);
		M->nnz++;

        	moveNext(A);
    	}
    	while (index(B) >= 0) {
        	Entry b = get(B);

		Entry E = newEntry(b->col, 0.0 - b->val);
        	append(C, E);
		M->nnz++;

		moveNext(B);
    	}	
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
	// if sizes aren't equal
	if (size(A) != size(B)) {
		fprintf(stderr, "Error. Tried product on different sized matrices\n");
		exit(EXIT_FAILURE);
	}

	// use copy in case A = B
	Matrix C = copy(B);

	Matrix M = newMatrix(size(A));

	// go through rows
	int n = size(A)+1;
	for (int i = 1; i < n; i++) {
		List a = A->row[i];
		List b = C->row[i];
		List m = M->row[i];
		
		sub(M, m, a, b);
	}

	freeMatrix(&C);	
	// return sum matrix
	return M;
}

// helper function for product
double vectorDot(List P, List Q) {
	double product = 0.0;

	// go through both lists
	moveFront(P);
	moveFront(Q);
	while (index(P) >= 0 && index(Q) >= 0) {
		Entry p = get(P);
		Entry q = get(Q);
		
		if (p->col == q->col) {
			// calculate product
			product += (p->val * q->val);
			moveNext(P);
			moveNext(Q);
		}
		
		else if (p->col < q->col) {
			moveNext(P);
		}
		else {
			moveNext(Q);
		}
	}
	// return vector dot product
	return product;
}
// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
	// if sizes aren't equal
	if (size(A) != size(B)) {
		fprintf(stderr, "Error. Tried product on different sized matrices\n");
		exit(EXIT_FAILURE);
	}

	Matrix P = newMatrix(size(A));

	// transpose B for multiplication
	Matrix T = transpose(B);
	
	int n = size(A)+1;
	for (int i = 1; i < n; i++) {
		List a = A->row[i];
		for (int j = 1; j < n; j++) {
			List t = T->row[j];

			double value = vectorDot(a, t);
			if (value != 0.0) {
				Entry p = newEntry(j, value);
				append(P->row[i], p);
				P->nnz++;
			}
		}	
	}
	
	freeMatrix(&T);
	return P;
}

// prints matrix
void printMatrix(FILE* out, Matrix M) {
	int n = size(M) + 1;
	// go through rows
	for (int i = 1; i < n; i++) {
		List L = M->row[i];

		if (length(L) > 0) {
			fprintf(out, "%d: ", i);
		}

		// go through column of entries
		for (moveFront(L); index(L) >= 0; moveNext(L)) {
			Entry E = get(L);
			fprintf(out, "(" COL ", " VAL ") ", E->col, E->val);
		}

		if (length(L) > 0) {
			fprintf(out, "\n");
		}
	}	
}
