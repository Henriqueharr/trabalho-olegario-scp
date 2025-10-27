#ifndef SAFETY_UTILS_H
#define SAFETY_UTILS_H




#include <ncurses.h>

unsigned int validarStr(const char *str);
void lerSizeT(size_t *valor, WINDOW *tela);
void lerStr(char *str, size_t buffer, WINDOW *tela);



#endif