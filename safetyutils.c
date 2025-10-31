#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include "safetyutils.h"
#include "menus.h"

short validarStr(const char *str)
{
   if(*str == ' ') return -1;
   while(*str++)
   {
      if(*str != ' ') return 1;
   }
   return 0;
}

short lerSizeT(size_t *valor, WINDOW *tela)
{
   short y, x, maxy;
   short verificarLeitura = 0;
   long long intermediario;
   getyx(tela, y, x);
   maxy = getmaxy(tela);
   verificarLeitura = mvwscanw(tela, y, x, "%lld", &intermediario);

   if(intermediario < 0) return 0; //valor negativo
   if(verificarLeitura == 0) return -1; //entrada inválida
   if(verificarLeitura < 0) return -2; //entrada não encontrada

   *valor = intermediario;
   return 1;
}

short lerStr(char *str, size_t buffer, WINDOW *tela)
{
   unsigned short x, y, maxy = getmaxy(tela);
   getyx(tela, y, x);
   mvwgetnstr(tela, y, x, str, buffer - 1);
   return validarStr(str);
}

short lerCPF(char *str, WINDOW *tela)
{
    unsigned short x = getcurx(tela), y = getcury(tela);
    unsigned short ind = 0;
    strncpy(str, "000.000.000-00\0", 15);
    while(ind < 14)
    {
        unsigned short dig = wgetch(tela);
        if(dig >= '0' && dig <= '9')
        {
            *(str + ind) = (char)dig;
            ind++;
            x++;
            if(ind == 3 || ind == 7)
            {
                waddch(tela, '.');
                x++;
                ind++;
            }
            if(ind == 11)
            {
                waddch(tela, '-');
                x++;
                ind++;
            }
        }
        else
        {
            if(dig == KEY_BACKSPACE)
            {
                if(ind > 0)
                {
                    if(ind == 4 || ind == 8 || ind == 12)
                    {
                        x--;
                        mvwaddch(tela, y, x, ' ');
                        x--;
                        mvwaddch(tela, y, x, ' ');
                        wmove(tela, y, x);
                        ind -= 2;
                        continue;
                    }
                    x--;
                    mvwaddch(tela, y, x, ' ');
                    wmove(tela, y, x);
                    ind--;
                }
            }
            else
            {
               if(dig == 'z' || dig == 'Z') return 0;
               else
               {
                  mvwaddch(tela, y, x, ' ');
                  wmove(tela, y, x);
               }
            }
        }
    }
    return 1;
}

bool validar_cpf(char *cpf_ptr) 
{

   int n = strlen(cpf_ptr);
   char cpf_numeros[12];
   int j = 0;

   for (int i = 0; i < n; i++) 
   {
      if (cpf_ptr[i] >= '0' && cpf_ptr[i] <= '9') 
      {
         cpf_numeros[j++] = cpf_ptr[i];
      } 
      else if (cpf_ptr[i] != '.' && cpf_ptr[i] != '-') 
      {
         j = -1; 
         break;
      }
   }

   if (j == -1) return 0; 
   if (j != 11) return 0;
   cpf_numeros[j] = '\0';
   if (todos_digitos_iguais(cpf_numeros)) return 0;
   

   int cpf[11];
   for (int i = 0; i < 11; i++) cpf[i] = cpf_numeros[i] - '0';

   int soma = 0, mult = 10;
   for (int i = 0; i < 9; i++) 
   {
      soma += cpf[i] * mult;
      mult--;
   }

   int digito1 = (soma * 10) % 11;
   if (digito1 == 10) digito1 = 0;

   soma = 0; mult = 11;
   for (int i = 0; i < 10; i++) 
   {
      if (i < 9) soma += cpf[i] * mult;
      else soma += digito1 * mult;
      mult--;
   }

   int digito2 = (soma * 10) % 11;
   if (digito2 == 10) digito2 = 0;

   if (digito1 == cpf[9] && digito2 == cpf[10]) return 1;
   
   return 0;
}

short lerCNPJ(char *str, WINDOW *tela)
{
   unsigned short x = getcurx(tela), y = getcury(tela);
   unsigned short ind = 0;
   strncpy(str, "00.000.000/0001-00\0", 20); //3 8 11 16
   while(ind < 18)
   {
      unsigned short dig = wgetch(tela);
      if(dig >= '0' && dig <= '9')
      {
         *(str + ind) = (char)dig;
         ind++;
         x++;
         if(ind == 2 || ind == 6)
         {
               waddch(tela, '.');
               x++;
               ind++;
         }
         if(ind == 10)
         {
               waddch(tela, '/');
               x++;
               ind++;
         }
         if(ind == 15)
         {
               waddch(tela, '-');
               x++;
               ind++;
         }
      }
      else
      {
         if(dig == KEY_BACKSPACE)
         {
               if(ind > 0)
               {
                  if(ind == 3 || ind == 8 || ind == 11 || ind == 16)
                  {
                     x--;
                     mvwaddch(tela, y, x, ' ');
                     x--;
                     mvwaddch(tela, y, x, ' ');
                     wmove(tela, y, x);
                     ind -= 2;
                     continue;
                  }
                  x--;
                  mvwaddch(tela, y, x, ' ');
                  wmove(tela, y, x);
                  ind--;
               }
         }
         else
         {
            if(dig == 'z' || dig == 'Z') return 0;
            else
            {
               mvwaddch(tela, y, x, ' ');
               wmove(tela, y, x);
            }
         }
      }
   }
   return 1;
}

bool validar_cnpj(char *cnpj_ptr) 
{

   int n = strlen(cnpj_ptr);
   char cnpj_numeros[15];
   int j = 0;

   for (int i = 0; i < n; i++) 
   {
      if (cnpj_ptr[i] >= '0' && cnpj_ptr[i] <= '9') 
      {
         cnpj_numeros[j++] = cnpj_ptr[i];
      } 
      else if (cnpj_ptr[i] != '.' && cnpj_ptr[i] != '-' && cnpj_ptr[i] != '/') 
      {
         j = -1;
         break;
      }
   }

   if (j == -1) return 0;
   if (j != 14) return 0;
   cnpj_numeros[j] = '\0';
   if (todos_digitos_iguais(cnpj_numeros)) return 0;
   

   int cnpj[14];
   for (int i = 0; i < 14; i++) cnpj[i] = cnpj_numeros[i] - '0';

   int soma = 0, mult = 5;
   for (int i = 0; i < 12; i++) 
   {
      soma += cnpj[i] * mult;
      mult--;
      if (mult < 2) mult = 9;
   }

   int digito1 = (soma % 11) < 2 ? 0 : 11 - (soma % 11);

   soma = 0; mult = 6;
   for (int i = 0; i < 13; i++) 
   {
      if (i < 12) soma += cnpj[i] * mult;
      else soma += digito1 * mult;
      mult--;
      if (mult < 2) mult = 9;
   }

   int digito2 = (soma % 11) < 2 ? 0 : 11 - (soma % 11);

   if (digito1 == cnpj[12] && digito2 == cnpj[13]) return 1;
   
   return 0;
}

bool todos_digitos_iguais(const char *str) {
   int n = strlen(str);
   for (int i = 1; i < n; i++) 
   {
      if (str[i] != str[0]) return 0;
   }
   return 1;
}
