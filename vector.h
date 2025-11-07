#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#define access_index(vector_ptr,type,position) (*((type*)vector_ptr.container[position]))

//Um Vector é um vetor/array de tamanho dinâmico
//Essa estrutura busca imitar o comportamento de um vector do C++
//Ela deve sempre ser inicializada com a função initVector
//Para acessar um elemento do Vector com mais facilidade, use o macro access_index
//
// typedef struct Vector
// {
//    size_t _capacity;
//    size_t _current_pb;
//    size_t _size;
//    void **container;
// } Vector;
typedef struct Vector
{
   size_t _capacity;
   size_t _current_pb;
   size_t _size;
   void **container;
} Vector;

//Essa função busca inicializar um Vector com valores iniciais
//Isso inclui capacidade, tamanho e container iniciais 
void initVector(Vector *vetor);

//Essa função insere elementos a partir de endereços destipados num Vector
void push_back(Vector *vetor, void* elemento);

//Essa função duplica a capacidade de um Vector
void _reserve(Vector *vetor);

//Essa função deleta um elemento dum Vector a partir de seu índice
//Em caso de índice inválido, a função não faz nada
void erasePos(Vector *vetor, size_t pos);


#endif