#include "bind.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vector structure
typedef struct vector_t {
	int data_size, len, max_len;
	void ** data;
	void (* push_back) ( char * str );
	int (* size) ();
} StringVec;

// A vector push_back function
void vec_push_back( void ** args ) {
	StringVec * vec = args[0];
	char * add = args[1];
	if ( vec->data == 0 ) {
		vec->max_len = 8;
		vec->data = malloc( vec->data_size * vec->max_len );
	}
	else if ( vec->len >= vec->max_len ) {
		vec->max_len *= 2;
		vec->data = realloc(vec->data, vec->max_len * vec->data_size);
	}
	vec->data[vec->len] = add;
	vec->len += 1;
}

// A vector size function
int vec_size( void ** args ) {
	return ((StringVec *) args[0])->len;
}

// A vector constructor
StringVec* make_vector() {
	StringVec * v = calloc(1, sizeof(StringVec));
	v->data_size = sizeof(void*);

	// Partially bind push_back
	v->push_back = partial_bind((void*)vec_push_back, 2, 1, v);

	// Fully bind size
	v->size = (int (*) ()) full_bind((void*)vec_size, 1, v);
	return v;
}


// Test C-bind
int main() {

	// String to add
	char * buf = malloc(0x1000);
	strcpy(buf, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	// The vector
	StringVec * v = make_vector();
	for ( int i = 0; i < 32; ++i ) {
		v->push_back( buf + i );
	}

	// Printing
	printf("Size = %d\n", (int) v->size());
	for ( int i = 0; i < v->size(); ++i ) {
		printf("v->data[%d] = %s\n", i, (char*) v->data[i]);
	}
}
