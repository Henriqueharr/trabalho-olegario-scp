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
         switch(opc)
         {
            case ID:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               booleans[opc - 1] = 1;
            }
            break;
            case NOME:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               booleans[opc - 1] = 1;
            }
            break;
            case ENDERECO:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               booleans[opc - 1] = 1;
            }
            break;
            case TELEFONE:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               booleans[opc - 1] = 1;
            }
            break;
            case CPF_CNPJ:
            {
               transicao1(subtelas[opc - 1], 0, 3);
               if(dataType == PESSOA_FISICA) lerStr(get_ctype(novoCliente,PessoaFisica)->cpf, sizeof(get_ctype(novoCliente,PessoaFisica)->cpf), subtelas[opc - 1]);
               else lerStr(get_ctype(novoCliente,PessoaJuridica)->cnpj, sizeof(get_ctype(novoCliente,PessoaJuridica)->cnpj), subtelas[opc - 1]);
               booleans[opc - 1] = 1;
            }
            break;
            case FINALIZAR:
            booleans[opc - 1] = 1;
            break;
         }
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

   return novoCliente;

   escape:
   for(short i = 0; i < 5; i++)
   {
      transicao1(telas[i], 0, 5);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
   free(novoCliente);
   return NULL;
}