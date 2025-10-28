#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "doublylinkedlist.h"
#include "menus.h"
#include "safetyutils.h"

const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};

void* criarCliente(Tipos dataType)
{
   short y = 7;
   WINDOW *telas[5], *subtelas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(5, 30, (i + 1) * y, 150);
      wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
      waddstr(telas[i], label[i]);
      if(dataType == PESSOA_JURIDICA && i == 4)
      {
         mvwaddstr(telas[i], 0, 0, label[5]);
      }
      subtelas[i] = derwin(telas[i], 3, 28, 1, 1);
      wrefresh(telas[i]);
      wrefresh(stdscr);
      napms(25);
   }


   void *novoCliente;

   if(dataType == PESSOA_FISICA)
   {
      novoCliente = (PessoaFisica*)calloc(1, sizeof(PessoaFisica));
   }
   else
   {
      novoCliente = (PessoaJuridica*)calloc(1, sizeof(PessoaJuridica));
   }

   unsigned short key = KEY_UP;
   MenuCriarCliente opc = ID;
   //ID//Nome//Endereço//Telefone//CPF ou CNPJ//confirmar saida
   unsigned short booleans[6] = {};
   unsigned short tudofeito = 0;
   unsigned short stdy = 9;
   mvwaddch(stdscr, stdy, 148, '>');
   curs_set(FALSE);
   keypad(stdscr, TRUE);
   noecho();
   cbreak();
   while(!booleans[5])
   {
      key = wgetch(stdscr);
      switch(key)
      {
         case 'Z':
         case 'z':
         goto escape;
         case KEY_UP:
         if(opc > 1)
         {
            mvwaddch(stdscr, stdy, 148, ' ');
            stdy -= 7;
            mvwaddch(stdscr, stdy, 148, '>');
            opc--;
         }
         break;
         case KEY_DOWN:
         if(opc < 5 || (opc == 5 && tudofeito))
         {
            mvwaddch(stdscr, stdy, 148, ' ');
            stdy += 7;
            mvwaddch(stdscr, stdy, 148, '>');
            opc++;
         }
         break;
         case '\n':
         echo();
         nocbreak();
         switch(opc)
         {
            case ID:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               wmove(subtelas[opc - 1], 1, 3);
               short check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == 0) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                  if(check == -2) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case NOME:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               wmove(subtelas[opc - 1], 1, 3);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case ENDERECO:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               wmove(subtelas[opc - 1], 1, 3);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");                  werase(subtelas[opc - 1]);
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case TELEFONE:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               wmove(subtelas[opc - 1], 1, 3);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case CPF_CNPJ:
            {
               cbreak();
               transicao1(subtelas[opc - 1], 0, 3);
               if(dataType == PESSOA_FISICA)
               {
                  wmove(subtelas[opc - 1], 1, 3);
                  lerCPF(get_ctype(novoCliente,PessoaFisica)->cpf, subtelas[opc - 1]);
                  while(!validar_cpf(get_ctype(novoCliente,PessoaFisica)->cpf))
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 1, 3);
                     cbreak();
                     lerCPF(get_ctype(novoCliente,PessoaFisica)->cpf, subtelas[opc - 1]);
                  }
               }
               else
               {
                  wmove(subtelas[opc - 1], 1, 3);
                  lerCPF(get_ctype(novoCliente,PessoaJuridica)->cnpj, subtelas[opc - 1]);
                  while(!validar_cnpj(get_ctype(novoCliente,PessoaJuridica)->cnpj))
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "O CNPJ digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 1, 3);
                     cbreak();
                     lerCNPJ(get_ctype(novoCliente,PessoaJuridica)->cnpj, subtelas[opc - 1]);
                  }
               }
               booleans[opc - 1] = 1;
            }
            break;
            case FINALIZAR:
            booleans[opc - 1] = 1;
            break;
         }
         noecho();
         cbreak();
         tudofeito = 1;
         for(unsigned short i = 0; i < 5; i++)
         {
            if(!booleans[i])
            {
               tudofeito = 0;
            }
         }
         break;
      }
      curs_set(FALSE);

      if(tudofeito)
      {
         unsigned short x, y;
         getyx(stdscr, y, x);
         mvwaddstr(stdscr, 44, 150, "Salvar cliente");
         wmove(stdscr, y, x);
      }

      wrefresh(stdscr);
   }

   for(short i = 0; i < 5; i++)
   {
      transicao1(telas[i], 0, 5);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   transicao1(stdscr, 43, 45);

   return novoCliente;

   escape:
   for(short i = 0; i < 5; i++)
   {
      transicao1(telas[i], 0, 5);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
   free(novoCliente);

   transicao1(stdscr, 43, 45);

   return NULL;
}