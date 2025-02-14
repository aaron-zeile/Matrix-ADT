/*** Sparse.c ***/
/*** Aaron Zeile ***/
/*** ID: azeile ***/
/*** pa4 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

#define MAX_LEN 300

int main(int argc, char* argv[]) {
	// if not two command line arguments
	if (argc != 3) {
		fprintf(stderr, "Error. Must have exactly 2 command line arguments.\n");
		exit(EXIT_FAILURE);
	}

	char line[MAX_LEN];
	char tokenBuffer[MAX_LEN];
	char *token;
	int token_count = 0;
	int line_count = 0;
	int matrix[3];
	

	// open files for reading and writing
	FILE *in = fopen(argv[1], "r");
	if (!in) {
		fprintf(stderr, "Error. Couldn't open input file.\n");
		exit(EXIT_FAILURE);
	}

	FILE *out = fopen(argv[2], "w");
	if (!out) {
		fprintf(stderr, "Error. Couldn't open output file.\n");
		exit(EXIT_FAILURE);
	}

// read first line ------------------------------------------------------------
	while (fgets(line, MAX_LEN, in))  {
	      	// get tokens in this line
	      	tokenBuffer[0] = '\0';
		token_count = 0;

      		// get first token
      		token = strtok(line, " \n");
      
      		while (token) { // we have a token
         		// update token buffer
         		strcat(tokenBuffer, "   ");
         		strcat(tokenBuffer, token);
         		strcat(tokenBuffer, " \n");

         		// get next token
			matrix[token_count] = atoi(token);
			token_count++;
      			token = strtok(NULL, " \n");
      		}

		line_count++;
		
		// break when we have matrix attributes
		// n, a, and b
		if (line_count == 1) {
			break;
		}
		
   	}

	// create matrices
	Matrix A = newMatrix(matrix[0]);
	Matrix B = newMatrix(matrix[0]);

	// move past blank line
	fgets(line, MAX_LEN, in);

// build matrices ------------------------------------------------------------------------
	line_count = 0;
	int row = 0;
	int col = 0;
	double val = 0.0;

	while (fgets(line, MAX_LEN, in))  {
	      	// get tokens in this line
	      	tokenBuffer[0] = '\0';
		token_count = 0;


      		// get first token
      		token = strtok(line, " \n");
      
      		while (token) { // we have a token
         		// update token buffer
         		strcat(tokenBuffer, "   ");
         		strcat(tokenBuffer, token);
         		strcat(tokenBuffer, " \n");

         		// get needed tokens
			if (token_count == 0) {
			 	row = atoi(token);
			}
			else if (token_count == 1) {
				col = atoi(token);
			}
			else if (token_count == 2) {
				val = atof(token);
			}
			token_count++;
      			token = strtok(NULL, " \n");
      		}

		changeEntry(A, row, col, val);
		line_count++;
		
		// break when we have all NNZ of A
		if (line_count == matrix[1]) {
			break;
		}
		
   	}

	// move past blank line
	fgets(line, MAX_LEN, in);

	line_count = 0;
	row = 0;
	col = 0;
	val = 0.0;

	while (fgets(line, MAX_LEN, in))  {
	      	// get tokens in this line
	      	tokenBuffer[0] = '\0';
		token_count = 0;


      		// get first token
      		token = strtok(line, " \n");
      
      		while (token) { // we have a token
         		// update token buffer
         		strcat(tokenBuffer, "   ");
         		strcat(tokenBuffer, token);
         		strcat(tokenBuffer, " \n");

         		// get needed tokens
			if (token_count == 0) {
			 	row = atoi(token);
			}
			else if (token_count == 1) {
				col = atoi(token);
			}
			else if (token_count == 2) {
				val = atof(token);
			}
			token_count++;
      			token = strtok(NULL, " \n");
      		}

		changeEntry(B, row, col, val);
		line_count++;
		
		// break when we have all NNZ of A
		if (line_count == matrix[2]) {
			break;
		}
		
   	}

// print output --------------------------------------------------------------------------

  // starting matrices and attributes
	// A
	fprintf(out, "A has %d non-zero entries: \n", matrix[1]);
	printMatrix(out, A);
	fprintf(out, "\n");	
	// B
	fprintf(out, "B has %d non-zero entries: \n", matrix[2]);
	printMatrix(out, B);
	fprintf(out, "\n");	

  // scalar mult
	Matrix M = scalarMult(1.5, A);
	fprintf(out, "(1.5) * A = \n");
	printMatrix(out, M);	
	fprintf(out, "\n");	

  // sum
	// A + B
	Matrix S = sum(A, B);
	fprintf(out, "A+B = \n");
	printMatrix(out, S);
	fprintf(out, "\n");	
	// A + A
	Matrix S2 = sum(A, A);
	fprintf(out, "A+A = \n");
	printMatrix(out, S2);
	fprintf(out, "\n");	

  // diff
  	// B - A
	Matrix D = diff(B, A);
	fprintf(out, "B-A = \n");
	printMatrix(out, D);
	fprintf(out, "\n");
	// A - A
	Matrix D2 = diff(A, A);
	fprintf(out, "A-A = \n");
	printMatrix(out, D2);
	fprintf(out, "\n");

  // transpose
  	Matrix T = transpose(A);
	fprintf(out, "Transpose(A) =\n");
	printMatrix(out, T);
	fprintf(out, "\n");

  // product
  	// A * B
	Matrix P = product(A, B);
	fprintf(out, "A*B =\n");
	printMatrix(out, P);
	fprintf(out, "\n");
	// B * B
	Matrix P2 = product(B, B);
	fprintf(out, "B*B =\n");
	printMatrix(out, P2);
	fprintf(out, "\n");

	
// free all heap memory -------------------------------------------------------------------
	freeMatrix(&A);
	freeMatrix(&B);
	freeMatrix(&M);
	freeMatrix(&S);
	freeMatrix(&S2);
	freeMatrix(&D);
	freeMatrix(&D2);
	freeMatrix(&T);
	freeMatrix(&P);
	freeMatrix(&P2);

	fclose(in);
	fclose(out);
  return 0;
}
