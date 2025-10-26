#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#define get_value(vetor,type,position) (*((type*)vetor.container[position]))
#define push_back(vetor,elemento) pb(&vetor,elemento)

typedef struct Vector
{
   size_t _capacity;
   size_t _current_pb;
   size_t _size;
   void **container;
} Vector;

void initVector(Vector *vetor);
void pb(Vector *vetor, void* elemento);
void _reserve(Vector *vetor);
void erasePos(Vector *vetor, size_t pos);


#endif