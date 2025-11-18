#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include "clientes.h"
#include "safetyutils.h"
#include "menus.h"
#include "doublylinkedlist.h"

extern double teladevy, teladevx;

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
   short y, x;
   short verificarLeitura = 0;
   long long intermediario;
   getyx(tela, y, x);
   verificarLeitura = mvwscanw(tela, y, x, "%lld", &intermediario);

   if(intermediario < 0) return 0; //valor negativo
   if(verificarLeitura == 0) return -1; //entrada inválida
   if(verificarLeitura < 0) return -2; //entrada não encontrada
   if(intermediario == 0) return -3; //Cancelou

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

short lerData(char* str, WINDOW* tela)
{
   unsigned short x = getcurx(tela), y = getcury(tela);
    unsigned short ind = 0;
    unsigned short dig = '0';
    while(ind < 14 && dig != '\n')
    {
        dig = wgetch(tela);
        if(dig >= '0' && dig <= '9')
        {
            *(str + ind) = (char)dig;
            ind++;
            x++;
            if(ind == 2 || ind == 5)
            {
               *(str + ind) = '/';
               ind++;
               x++;
               waddch(tela, '/');
            }
        }
        else
        {
            if(dig == KEY_BACKSPACE)
            {
                if(ind > 0)
                {
                    if(ind == 3 || ind == 6)
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
                    ind--;
                    wmove(tela, y, x);
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
    if(ind <= 9)
    {
      return 0;
    }
    *(str + ind) = '\0';
    return 1;
}

short lerDouble(WINDOW *tela, double *valor)
{
   unsigned short y, x;
   getyx(tela, y, x);
   double intermediario;
   short check = mvwscanw(tela, y, x, "%lf", &intermediario);

   if(intermediario < 0) return 0; //valor negativo
   if(check == 0) return -1; //entrada inválida
   if(check < 0) return -2; //entrada não encontrada
   if(intermediario == 0) return -3; //Cancelou
 
   *valor = intermediario;
   return 1;
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

char **split(char *str)
{
   char **substrs = (char**)malloc(sizeof(char*));
   char *atributo = strtok(str, ";");

   for(short i = 0; atributo; i++)
   {
      *(substrs + i) = atributo;
      substrs = (char**)realloc(substrs, (i + 2) * sizeof(char*));
      atributo = strtok(NULL, ";");
   }

   return substrs;
}

void carregarDados(List *lista, const char *nomeArquivo)
{
   FILE *csv = fopen(nomeArquivo, "r");

   if(!csv) return;

   if(strcmp(nomeArquivo, "clientes.csv") == 0)
   {
      char buffer[500];

      while(fgets(buffer, sizeof(buffer), csv))
      {
         buffer[strcspn(buffer, "\n")] = '\0';

         char *restante;

         char **atributos = split(buffer);

         if(atributos[4][2] == '.')
         {
            PessoaJuridica *conteudo = (PessoaJuridica*)malloc(sizeof(PessoaJuridica));
            conteudo->data.id = strtoul(atributos[0], &restante, 10);
            strcpy(conteudo->data.name, atributos[1]);
            strcpy(conteudo->data.address, atributos[2]);
            strcpy(conteudo->data.phonenumber, atributos[3]);
            strcpy(conteudo->cnpj, atributos[4]);

            createInsertNode(lista, conteudo, PESSOA_JURIDICA);

            free(atributos);

            continue;
         }

         PessoaFisica *conteudo = (PessoaFisica*)malloc(sizeof(PessoaFisica));
         conteudo->data.id = strtoul(atributos[0], &restante, 10);
         strcpy(conteudo->data.name, atributos[1]);
         strcpy(conteudo->data.address, atributos[2]);
         strcpy(conteudo->data.phonenumber, atributos[3]);
         strcpy(conteudo->cpf, atributos[4]);

         createInsertNode(lista, conteudo, PESSOA_FISICA);

         free(atributos);
      }

      fclose(csv);

      return;
   }
}

void SalvarDados(List *lista, const char *nomeArquivo)
{
   FILE *csv = fopen(nomeArquivo, "w");

   if(!csv) return;

   char buffer[BUFSIZ * 20];
   setvbuf(csv, buffer, _IOFBF, sizeof(buffer));

   if(strcmp(nomeArquivo, "clientes.csv") == 0)
   {
      for(Node *atual = lista->head; atual; atual = atual->next)
      {
         fprintf(csv, "%zu;%s;%s;%s;%s\n",
                        expand_node(atual, GenericCast)->data.id,
                        expand_node(atual, GenericCast)->data.name,
                        expand_node(atual, GenericCast)->data.address,
                        expand_node(atual, GenericCast)->data.phonenumber,
                        ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
      }

      goto salvou;
   }



   salvou:
   char retorno[BUFSIZ];
   setvbuf(csv, retorno, _IOFBF, sizeof(retorno));
   fclose(csv);
}