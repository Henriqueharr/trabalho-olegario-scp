#ifndef SAFETY_UTILS_H
#define SAFETY_UTILS_H

#include <ncurses.h>
#include <stdbool.h>

//Função que verifica se a string está vazia ou iniciada com um espaço
short validarStr(const char *str);

//Essa função retorna 1 se um CPF for valido e 0 caso contrário
//Formato do CPF 000.000.000-00
bool validar_cpf(char *cpf_ptr);

//Essa função retorna 1 se um CNPJ for valido e 0 caso contrário
//Formato do CNPJ 00.000.000/0001-00
bool validar_cnpj(char *cnpj_ptr);

//Essa função retorna 1 caso todos os caracters de uma string sejam iguais e 0 caso contrário
bool todos_digitos_iguais(const char *str);

//Essa função lê de forma segura variáveis do tipo size_t (unsigned long)
//Deve ser especificado qual tela do ncurses está coletando o input
short lerSizeT(size_t *valor, WINDOW *tela);

//Essa função lê strings a partir de uma tela e da capacidade da string
//Deve ser especificado qual tela do ncurses está coletando o input
short lerStr(char *str, size_t buffer, WINDOW *tela);

//Essa função lê CPFs com formatação automática
//Aviso: Para posterior validação, deve-se usar validar_cpf()
//Deve ser especificado qual tela do ncurses está coletando o input
short lerCPF(char *str, WINDOW *tela);

//Essa função lê CNPJs com formatação automática
//Aviso: Para posterior validação, deve-se usar validar_cnpj()
//Deve ser especificado qual tela do ncurses está coletando o input
short lerCNPJ(char *str, WINDOW *tela);


#endif