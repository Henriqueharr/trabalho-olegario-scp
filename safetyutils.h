#ifndef SAFETY_UTILS_H
#define SAFETY_UTILS_H

#include <ncurses.h>
#include <stdbool.h>

short validarStr(const char *str);
bool validar_cpf(char *cpf_ptr);
bool validar_cnpj(char *cnpj_ptr);
bool todos_digitos_iguais(const char *str);
short lerSizeT(size_t *valor, WINDOW *tela);
short lerStr(char *str, size_t buffer, WINDOW *tela);
void lerCPF(char *str, WINDOW *tela);
void lerCNPJ(char *str, WINDOW *tela);


#endif