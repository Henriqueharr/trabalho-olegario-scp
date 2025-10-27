#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "doublylinkedlist.h"
#include "menus.h"

const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};

void* criarCliente(Tipos dataType)
{
   short y = 7;
   WINDOW *telas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(5, 30, (i + 1) * y, 150);
      wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
      waddstr(telas[i], label[i]);
      if(dataType == PESSOA_JURIDICA && i == 4)
      {
         mvwaddstr(telas[i], 0, 0, label[5]);
      }
      wrefresh(telas[i]);
      wrefresh(stdscr);
      napms(25);
   }

   if(dataType == PESSOA_FISICA)
   {
      //PessoaFisica *novoCliente = (PessoaFisica*)calloc(1, sizeof(PessoaFisica));
      
   }
   getch();

   for(short i = 0; i < 5; i++)
   {
      transicao1(telas[i], 0, 5);
      delwin(telas[i]);
   }

}