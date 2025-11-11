#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>
#include "clientes.h"
#include "doublylinkedlist.h"
#include "menus.h"
#include "safetyutils.h"

extern double teladevy, teladevx;

void* criarCliente(Tipos dataType, List *lista)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};

   unsigned short y = maxstdy * (7.0/teladevy);
   unsigned short x = maxstdx * (30.0/teladevx);
   unsigned short xpos = maxstdx * (150.0/teladevx);
   unsigned short ytamerro = maxstdy * (10.0/teladevy);
   unsigned short xtamerro = maxstdx * (36.0/teladevx);
   unsigned short yposerro = maxstdy * (1.0/teladevy);
   unsigned short xposerro = maxstdx * (86.0/teladevx);
   unsigned short btsalvar = 6 * y + y * 0.5;

   WINDOW *telas[5], *subtelas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(y, x, (i + 1) * y, xpos);
      wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
      wattron(telas[i], COLOR_PAIR(4));
      mvwaddstr(telas[i], 0, 0, label[i]);
      if(dataType == PESSOA_JURIDICA && i == 4)
      {
         mvwaddstr(telas[i], 0, 0, label[5]);
      }
      wattroff(telas[i], COLOR_PAIR(4));
      subtelas[i] = derwin(telas[i], y * (3.0/y), x - 4, y * (2.0/y), x * (2.0/x));
      wrefresh(telas[i]);
      napms(15);
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

   if(!novoCliente) goto escape;

   unsigned short key = KEY_UP;
   MenuCriarCliente opc = ID;
   //ID//Nome//Endereço//Telefone//CPF ou CNPJ//confirmar saida
   unsigned short preenchidos[6] = {};
   unsigned short tudofeito = 0;
   unsigned short stdy = y + y * 0.5;
   mvwaddch(stdscr, stdy, xpos - 2, '>');
   curs_set(FALSE);
   keypad(stdscr, TRUE);
   noecho();
   cbreak();
   while(!preenchidos[5])
   {
      key = wgetch(stdscr);
      switch(key)
      {
         case 'Z':
         case 'z':
         mvwaddch(stdscr, stdy, xpos - 2, ' ');
         wrefresh(stdscr);
         goto escape;
         case KEY_UP:
         if(opc > 1)
         {
            mvwaddch(stdscr, stdy, xpos - 2, ' ');
            stdy -= y;
            mvwaddch(stdscr, stdy, xpos - 2, '>');
            opc--;
         }
         break;
         case KEY_DOWN:
         if(opc < 5 || (opc == 5 && tudofeito))
         {
            mvwaddch(stdscr, stdy, xpos - 2, ' ');
            stdy += y;
            mvwaddch(stdscr, stdy, xpos - 2, '>');
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
               cortina(subtelas[opc - 1], 0, 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               if(check == -3) goto pulouID;
               Node* repetido = findByID(lista, get_ctype(novoCliente,GenericCast)->data.id);
               while(check != 1 || repetido)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == 0) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                  if(check == -1) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                  if(check == -2) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                  if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este ID de cliente já existe");
                  erroDuplo:
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
                  if(check == -3) goto pulouID;
                  curs_set(FALSE);
                  repetido = findByID(lista, get_ctype(novoCliente,GenericCast)->data.id);

               }
               werase(subtelas[opc - 1]);
               mvwprintw(subtelas[opc - 1], 0, 0, "%zu", get_ctype(novoCliente,GenericCast)->data.id);
               wrefresh(subtelas[opc - 1]);
               preenchidos[opc - 1] = 1;
            }
            break;
            pulouID:
            werase(subtelas[opc - 1]);
            if(preenchidos[opc - 1]) mvwprintw(subtelas[opc - 1], 0, 0, "%zu", get_ctype(novoCliente,GenericCast)->data.id);
            wrefresh(subtelas[opc - 1]);
            break;
            case NOME:
            {
               cortina(subtelas[opc - 1], 0, 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               }
               preenchidos[opc - 1] = 1;
            }
            break;
            case ENDERECO:
            {
               cortina(subtelas[opc - 1], 0, 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");                  werase(subtelas[opc - 1]);
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               }
               preenchidos[opc - 1] = 1;
            }
            break;
            case TELEFONE:
            {
               cortina(subtelas[opc - 1], 0, 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               }
               preenchidos[opc - 1] = 1;
            }
            break;
            case CPF_CNPJ:
            {
               char cpf_cnpj[20];
               cbreak();
               cortina(subtelas[opc - 1], 0, 0, y * (3.0/7.0), 10);
               if(dataType == PESSOA_FISICA)
               {
                  wmove(subtelas[opc - 1], 0, 0);
                  keypad(subtelas[opc - 1], TRUE);
                  short confirmou = lerCPF(cpf_cnpj, subtelas[opc - 1]);
                  Node *repetido = findCByCPF_CNPJ(lista, cpf_cnpj);
                  keypad(subtelas[opc - 1], FALSE);
                  if(!confirmou)
                  {
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     goto skip;
                  }
                  while(!validar_cpf(cpf_cnpj) || repetido)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado já está cadastrado no sistema");
                     else gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     cbreak();
                     keypad(subtelas[opc - 1], TRUE);
                     confirmou = lerCPF(cpf_cnpj, subtelas[opc - 1]);
                     repetido = findCByCPF_CNPJ(lista, cpf_cnpj);
                     keypad(subtelas[opc - 1], FALSE);
                     if(!confirmou) goto skip;
                  }
                  strncpy(get_ctype(novoCliente, PessoaFisica)->cpf, cpf_cnpj, 14);
                  get_ctype(novoCliente, PessoaFisica)->cpf[14] = '\0';
               }
               else
               {
                  wmove(subtelas[opc - 1], 0, 0);
                  keypad(subtelas[opc - 1], TRUE);
                  short confirmou = lerCNPJ(cpf_cnpj, subtelas[opc - 1]);
                  Node *repetido = findCByCPF_CNPJ(lista, cpf_cnpj);
                  keypad(subtelas[opc - 1], FALSE);
                  if(!confirmou) goto skip;

                  while(!validar_cnpj(cpf_cnpj) || repetido)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CNPJ digitado já está cadastrado no sistema");
                     else gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CNPJ digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     cbreak();
                     keypad(subtelas[opc - 1], TRUE);
                     confirmou = lerCNPJ(cpf_cnpj, subtelas[opc - 1]);
                     repetido = findCByCPF_CNPJ(lista, cpf_cnpj);
                     keypad(subtelas[opc - 1], FALSE);
                     if(!confirmou) goto skip;
                  }
                  strncpy(get_ctype(novoCliente, PessoaJuridica)->cnpj, cpf_cnpj, 19);
                  get_ctype(novoCliente, PessoaJuridica)->cnpj[19] = '\0';
               }
               preenchidos[opc - 1] = 1;
               break;
               skip:
               werase(subtelas[opc - 1]);
               if(preenchidos[opc - 1]) mvwaddstr(subtelas[opc - 1], 0, 0, (dataType == PESSOA_FISICA ? get_ctype(novoCliente, PessoaFisica)->cpf : get_ctype(novoCliente, PessoaJuridica)->cnpj));
               wrefresh(subtelas[opc - 1]);
            }
            break;
            case FINALIZAR:
            preenchidos[opc - 1] = 1;
            break;
         }
         noecho();
         cbreak();
         tudofeito = 1;
         for(unsigned short i = 0; i < 5; i++) if(!preenchidos[i]) tudofeito = 0;
         if(tudofeito)
         {
            wattron(stdscr, COLOR_PAIR(1));
            mvwaddstr(stdscr, btsalvar, xpos, "Salvar cliente");
            wattroff(stdscr, COLOR_PAIR(1));
         }
         break;
      }
      curs_set(FALSE);
      wrefresh(stdscr);
   }

   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 0, y, 20);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   cortina(stdscr, 0, btsalvar - 1, btsalvar + 1, 25);

   return novoCliente;

   escape:
   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 0, y, 20);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
   free(novoCliente);

   cortina(stdscr, 0, btsalvar - 1, btsalvar + 1, 25);

   return NULL;
}

void listarClientes(List *listaClientes)
{
   noecho();
   cbreak();
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   unsigned int n;
   size_t ind = 0;
   (listaClientes->tam % 6 == 0 ? (n = listaClientes->tam / 6) : (n = listaClientes->tam / 6 + 1));
   
   Node *pagina[n];
   
   unsigned short ytamerro = maxstdy * (10.0/teladevy);
   unsigned short xtamerro = maxstdx * (30.0/teladevx);
   unsigned short yposerro = maxstdy * (2.0/teladevy);
   
   if(!listaClientes->head) goto listavazia;
   
   Node *atual = listaClientes->head;

   for(unsigned int cont = 1; atual; atual = atual->next, cont++)
   {
      if(cont % 6 == 1)
      {
         pagina[ind] = atual;
         ind++;
      }
   }
   
   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Direita : Avançar página");
   mvwaddstr(stdscr, 2, 1, "Esquerda: Retroceder página");
   mvwaddstr(stdscr, 3, 1, "P/p     : Consultar por página");
   mvwaddstr(stdscr, 4, 1, "I/i     : Consultar por ID");
   mvwaddstr(stdscr, 5, 1, "Enter   : Confirmar");
   mvwaddstr(stdscr, 6, 1, "Z/z     : Voltar");
   
   unsigned short yascii = maxstdy * (28.0/teladevy);
   unsigned short xascii1 = maxstdx * (2.0/teladevx);
   unsigned short xascii2 = maxstdx * (204.0/teladevx);
   
   mvwaddstr(stdscr, yascii + 0, xascii1, "  ██╗");
   mvwaddstr(stdscr, yascii + 1, xascii1, " ██╔╝");
   mvwaddstr(stdscr, yascii + 2, xascii1, "██╔╝ ");
   mvwaddstr(stdscr, yascii + 3, xascii1, "╚██╗ ");
   mvwaddstr(stdscr, yascii + 4, xascii1, " ╚██╗");
   mvwaddstr(stdscr, yascii + 5, xascii1, "  ╚═╝");
   mvwaddstr(stdscr, yascii + 0, xascii2, "██╗  ");
   mvwaddstr(stdscr, yascii + 1, xascii2, "╚██╗ ");
   mvwaddstr(stdscr, yascii + 2, xascii2, " ╚██╗");
   mvwaddstr(stdscr, yascii + 3, xascii2, " ██╔╝");
   mvwaddstr(stdscr, yascii + 4, xascii2, "██╔╝ ");
   mvwaddstr(stdscr, yascii + 5, xascii2, "╚═╝  ");
   
   wnoutrefresh(stdscr);
   
   unsigned short yi = maxstdy * (15.0/teladevy), xi[] = {maxstdx * (15.0/teladevx), maxstdx * 0.5 - (maxstdx * (50.0/teladevx)) * 0.5, maxstdx * (146.0/teladevx)};
   
   logo(maxstdy * (4.0/teladevy), maxstdx * 0.5 - 11);
   
   WINDOW *selecaoPagina = newwin(3, 19, yi - 3, xi[0]);
   WINDOW *subsel = derwin(selecaoPagina, 1, 8, 1, 1);
   wborder(selecaoPagina, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
   mvwaddstr(selecaoPagina, 0, 0, "Página");
   mvwprintw(selecaoPagina, 1, 1, "        /%d", n);
   wnoutrefresh(selecaoPagina);
   
   WINDOW *telas[6];
   WINDOW *subtelas[6];
   
   unsigned short ytamaba = yi;
   unsigned short xtamaba = maxstdx * (50.0/teladevx);
   
   for(unsigned short i = 0; i < 6; i++)
   {
      telas[i] = newwin(ytamaba, xtamaba, yi, xi[i % 3]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], ytamaba - 2, xtamaba - 2, 1, 1);
      wnoutrefresh(telas[i]);
      if(i == 2) yi += (ytamaba + 2);
   }
   
   wnoutrefresh(stdscr);
   
   ind = 1;
   atual = pagina[0];

   keypad(stdscr, TRUE);
   unsigned short cmd = KEY_LEFT;
   while(cmd != 'Z' && cmd != 'z')
   {
      switch(cmd)
      {
         case KEY_LEFT:
         {
            if(ind > 0)
            {
               ind--;
               werase(subsel);
               mvwprintw(subsel, 0, 0, "%zu", ind + 1);
               wnoutrefresh(subsel);
               atual = pagina[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wnoutrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                   expand_node(atual, GenericCast)->data.id,
                                                   expand_node(atual, GenericCast)->data.name,
                                                   expand_node(atual, GenericCast)->data.address,
                                                   expand_node(atual, GenericCast)->data.phonenumber,
                                                   ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
                     wnoutrefresh(subtelas[i]);
                     atual = atual->next;
                  }
               }
            }
         }
         break;
         case KEY_RIGHT:
         {
            if(ind < n - 1)
            {
               ind++;
               werase(subsel);
               mvwprintw(subsel, 0, 0, "%zu", ind + 1);
               wnoutrefresh(subsel);
               atual = pagina[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wnoutrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                   expand_node(atual, GenericCast)->data.id,
                                                   expand_node(atual, GenericCast)->data.name,
                                                   expand_node(atual, GenericCast)->data.address,
                                                   expand_node(atual, GenericCast)->data.phonenumber,
                                                   ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
                     wnoutrefresh(subtelas[i]);
                     atual = atual->next;
                  }
               }
            }
         }
         break;
         case 'P':
         case 'p':
         {
            echo();
            nocbreak();
            curs_set(TRUE);
            werase(subsel);
            wrefresh(subsel);
            wmove(subsel, 0, 0);
            short check = lerSizeT(&ind, subsel);
            while(check != 1 || ind > n || ind <= 0)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               if(check == 0) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                  goto duploerro;
               }
               if(check == -1) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                  goto duploerro;
               }
               if(check == -2) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                  goto duploerro;
               }
               if(ind > n || ind <= 0)
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "A página requisitada não existe");
               }
               duploerro:
               werase(subsel);
               wrefresh(subsel);
               nocbreak();
               echo();
               curs_set(TRUE);
               check = lerSizeT(&ind, subsel);
            }
            curs_set(FALSE);
            cbreak();
            noecho();
            ind--;
            werase(subsel);
            mvwprintw(subsel, 0, 0, "%zu", ind + 1);
            wnoutrefresh(subsel);
            atual = pagina[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wnoutrefresh(subtelas[i]);
               if(atual)
               {
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                expand_node(atual, GenericCast)->data.id,
                                                expand_node(atual, GenericCast)->data.name,
                                                expand_node(atual, GenericCast)->data.address,
                                                expand_node(atual, GenericCast)->data.phonenumber,
                                                ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
                  wnoutrefresh(subtelas[i]);
                  atual = atual->next;
               }
            }
         }
         break;
         case 'I':
         case 'i':
         {
            WINDOW *selid = newwin(3, 25, ytamaba - 6, xi[0]);
            WINDOW *subselid = derwin(selid, 1, 23, 1, 1);
            wborder(selid, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
            mvwaddstr(selid, 0, 0, "Consultar ID");
            wrefresh(selid);
            echo();
            nocbreak();
            curs_set(TRUE);
            wmove(subselid, 0, 0);
            size_t targetID;
            short check = lerSizeT(&targetID, subselid);
            if(check == -3) goto cancelarID;
            while(check != 1)
            {
               cbreak();
               noecho();
               curs_set(FALSE); 
               if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
               if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
               if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
               nocbreak();
               echo();
               curs_set(TRUE);
               repetirID:
               werase(subselid);
               wrefresh(subselid);
               wmove(subselid, 1, 3);
               check = lerSizeT(&targetID, subselid);
               if(check == -3) goto cancelarID;
            }
            int dist = pathToCNode(listaClientes, targetID);
            if(dist == -1)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
               nocbreak();
               echo();
               curs_set(TRUE);
               goto repetirID;
            }
            ind = dist / 6;
            werase(subsel);
            mvwprintw(subsel, 0, 0, "%zu", ind + 1);
            wnoutrefresh(subsel);
            atual = pagina[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wnoutrefresh(subtelas[i]);
               if(atual)
               {
                  if(expand_node(atual, GenericCast)->data.id == targetID) wattron(subtelas[i], COLOR_PAIR(1));
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                expand_node(atual, GenericCast)->data.id,
                                                expand_node(atual, GenericCast)->data.name,
                                                expand_node(atual, GenericCast)->data.address,
                                                expand_node(atual, GenericCast)->data.phonenumber,
                                                ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
                  wnoutrefresh(subtelas[i]);
                  wattroff(subtelas[i], COLOR_PAIR(1));
                  atual = atual->next;
               }
            }
            cancelarID:
            cortina(selid, 0, 0, 3, 25);
            delwin(subselid);
            delwin(selid);
            noecho();
            cbreak();
         }
         break;
      }
      curs_set(FALSE);
      doupdate();
      cmd = wgetch(stdscr);
   }

   for(unsigned short i = 0; i < 6; i++)
   {
      abrir(telas[i], round(ytamaba * 0.5), round(ytamaba * 0.5), 10);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   return;

   listavazia:
   curs_set(FALSE);
   gerarAviso(ytamerro, xtamerro, maxstdy * 0.5 - (ytamerro) * 0.5, maxstdx * 0.5 - (xtamerro) * 0.5, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
   curs_set(TRUE);
}

void editarCliente(List *listaClientes)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   noecho();
   cbreak();

   if(!listaClientes->head) goto listavazia;

   unsigned short ydadostam = maxstdy * (20.0/teladevy);
   unsigned short xdadostam = maxstdx * (45.0/teladevx);
   unsigned short ytamerro = maxstdy * (10.0/teladevy);
   unsigned short xtamerro = maxstdx * (36.0/teladevx);
   unsigned short yposerro = maxstdy * 0.5 - ytamerro * 0.5;
   unsigned short xposerro = maxstdx * (156.0/teladevx);

   WINDOW *dados = newwin(ydadostam,xdadostam, maxstdy * 0.5 - ydadostam * 0.5, maxstdx * (48.0/teladevx));
   WINDOW *subdados = derwin(dados, ydadostam - 2,xdadostam - 2, 1, 1);
   wborder(dados, '[', ']', '>', '<', '@', '@', '@', '@');
   wnoutrefresh(dados);

   

   Node *atual = listaClientes->head;

   unsigned short cmd = KEY_LEFT;

   keypad(stdscr, TRUE);

   while(cmd != 'z' && cmd != 'Z')
   {
      switch(cmd)
      {
         case KEY_RIGHT:
         {
            if(atual->next)
            {
               atual = atual->next;
               werase(subdados);
            }
         }
         break;
         case KEY_LEFT:
         {
            if(atual->prev)
            {
               atual = atual->prev;
               werase(subdados);
            }
         }
         break;
         case '\n':
         {
            const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};
            
            WINDOW *telas[5];
            WINDOW *subtelas[5];

            unsigned short y = maxstdy * (7.0/teladevy);
            unsigned short x = maxstdx * (30.0/teladevx);
            unsigned short xpos = maxstdx * (105.0/teladevx);

            size_t idAlt = expand_node(atual, GenericCast)->data.id;
            char nome[100], endereco[200], telefone[21], cpf_cnpj[20];
            strncpy(nome, expand_node(atual, GenericCast)->data.name, 100);
            strncpy(endereco, expand_node(atual, GenericCast)->data.address, 200);
            strncpy(telefone, expand_node(atual, GenericCast)->data.phonenumber, 21);
            strcpy(cpf_cnpj, (atual->dataType == PESSOA_FISICA ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));

            const char *at[] = {nome, endereco, telefone, cpf_cnpj};

            for(short i = 0; i < 5; i++)
            {
               telas[i] = newwin(y, x, (i + 1) * y, xpos);
               wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
               wattron(telas[i], COLOR_PAIR(4));
               waddstr(telas[i], label[i]);
               if(atual->dataType == PESSOA_JURIDICA && i == 4)
               {
                  mvwaddstr(telas[i], 0, 0, label[5]);
               }
               wattroff(telas[i], COLOR_PAIR(4));
               subtelas[i] = derwin(telas[i], y * (3.0/7.0), x - 4, y * (2.0/y), x * (2.0/x));
               if(i > 0)
               {
                  mvwaddstr(subtelas[i], 0, 0, at[i - 1]);
                  wnoutrefresh(subtelas[i]);
               }
               else
               {
                  mvwprintw(subtelas[i], 0, 0, "%zu", idAlt);
                  wnoutrefresh(subtelas[i]);
               }
               wnoutrefresh(telas[i]);
               doupdate();
               napms(15);
            }
            
            bool mudou = 0;

            goto edicao;
            reload:
            
            for(short i = 0; i < 5; i++)
            {
               wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
               wattron(telas[i], COLOR_PAIR(4));
               mvwaddstr(telas[i], 0, 0, label[i]);
               if(atual->dataType == PESSOA_JURIDICA && i == 4)
               {
                  mvwaddstr(telas[i], 0, 0, label[5]);
               }
               wattroff(telas[i], COLOR_PAIR(4));
               switch(i)
               {
                  case 0:
                  mvwprintw(subtelas[i], 0, 0, "%zu", idAlt);
                  wnoutrefresh(subtelas[i]);
                  break;
                  case 1:
                  mvwaddstr(subtelas[i], 0, 0, nome);
                  wnoutrefresh(subtelas[i]);
                  break;
                  case 2:
                  mvwaddstr(subtelas[i], 0, 0, endereco);
                  wnoutrefresh(subtelas[i]);
                  break;
                  case 3:
                  mvwaddstr(subtelas[i], 0, 0, telefone);
                  wnoutrefresh(subtelas[i]);
                  break;
                  case 4:
                  mvwaddstr(subtelas[i], 0, 0, cpf_cnpj);
                  wnoutrefresh(subtelas[i]);
                  break;
               }
               wnoutrefresh(telas[i]);
               doupdate();
               napms(15);
            }

            wattron(stdscr, COLOR_PAIR(2));
            mvwaddstr(stdscr, y * 6 + y * 0.5, xpos, "Salvar alterações");
            wnoutrefresh(stdscr);
            wattroff(stdscr, COLOR_PAIR(2));
            
            edicao:
            unsigned short setay = 2 * y + y * 0.5;
            MenuCriarCliente selecao = NOME;
            cmd = KEY_UP;

            curs_set(FALSE);

            while(cmd != '\n' || selecao != FINALIZAR)
            {
               switch(cmd)
               {
                  case 'Z':
                  case 'z':
                  goto cancelar;
                  break;
                  case KEY_UP:
                  {
                     if(selecao > ID)
                     {
                        mvwaddch(stdscr, setay, xpos - 3, ' ');
                        setay -= y;
                        mvwaddch(stdscr, setay, xpos - 3, '>');
                        selecao--;
                     }
                  }
                  break;
                  case KEY_DOWN:
                  {
                     if(selecao < CPF_CNPJ || (selecao == CPF_CNPJ && mudou))
                     {
                        mvwaddch(stdscr, setay, xpos - 3, ' ');
                        setay += y;
                        mvwaddch(stdscr, setay, xpos - 3, '>');
                        selecao++;
                     }
                  }
                  break;
                  case '\n':
                  {
                     curs_set(TRUE);
                     echo();
                     nocbreak();
                     switch(selecao)
                     {
                        case ID:
                        {
                           
                           werase(subtelas[selecao - 1]);
                           wrefresh(subtelas[selecao - 1]);
                           wmove(subtelas[selecao - 1], 0, 0);
                           short check = lerSizeT(&idAlt, subtelas[selecao - 1]);
                           Node *repetido = findByID(listaClientes, idAlt);
                           if(repetido == atual && check == 1 || check == -3) goto cancelouID;
                           while(check != 1 || (repetido && repetido != atual))
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              if(check == 0) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                              if(check == -1) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                              if(check == -2) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                              if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este ID de cliente já existe");
                              erroDuplo:
                              nocbreak();
                              echo();
                              curs_set(TRUE);
                              werase(subtelas[selecao - 1]);
                              wrefresh(subtelas[selecao - 1]);
                              wmove(subtelas[selecao - 1], 0, 0);
                              check = lerSizeT(&idAlt, subtelas[selecao - 1]);
                              curs_set(FALSE);
                              repetido = findByID(listaClientes, idAlt);
                              if(repetido == atual && check == 1 || check == -3) goto cancelouID;
                           }
                        }
                        mudou = 1;
                        cancelouID:
                        werase(subtelas[selecao - 1]);
                        mvwprintw(subtelas[selecao - 1], 0, 0, "%zu",idAlt);
                        wrefresh(subtelas[selecao - 1]);
                        noecho();
                        cbreak();
                        break;
                        case NOME:
                        {
                           werase(subtelas[selecao - 1]);
                           wrefresh(subtelas[selecao - 1]);
                           wmove(subtelas[selecao - 1], 0, 0);
                           short check = lerStr(nome, sizeof(nome), subtelas[selecao - 1]);
                           while(check != 1)
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                              nocbreak();
                              echo();
                              curs_set(TRUE);
                              werase(subtelas[selecao - 1]);
                              wrefresh(subtelas[selecao - 1]);
                              wmove(subtelas[selecao - 1], 0, 0);
                              check = lerStr(nome, sizeof(nome), subtelas[selecao - 1]);
                           }
                        }
                        noecho();
                        cbreak();
                        mudou = 1;
                        break;
                        case ENDERECO:
                        {
                           werase(subtelas[selecao - 1]);
                           wrefresh(subtelas[selecao - 1]);
                           wmove(subtelas[selecao - 1], 0, 0);
                           short check = lerStr(endereco, sizeof(endereco), subtelas[selecao - 1]);
                           while(check != 1)
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                              nocbreak();
                              echo();
                              curs_set(TRUE);
                              werase(subtelas[selecao - 1]);
                              wrefresh(subtelas[selecao - 1]);
                              wmove(subtelas[selecao - 1], 0, 0);
                              check = lerStr(endereco, sizeof(endereco), subtelas[selecao - 1]);
                           }
                        }
                        noecho();
                        cbreak();
                        mudou = 1;
                        break;
                        case TELEFONE:
                        {
                           werase(subtelas[selecao - 1]);
                           wrefresh(subtelas[selecao - 1]);
                           wmove(subtelas[selecao - 1], 0, 0);
                           short check = lerStr(telefone, sizeof(telefone), subtelas[selecao - 1]);
                           while(check != 1)
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                              nocbreak();
                              echo();
                              curs_set(TRUE);
                              werase(subtelas[selecao - 1]);
                              wrefresh(subtelas[selecao - 1]);
                              wmove(subtelas[selecao - 1], 0, 0);
                              check = lerStr(telefone, sizeof(telefone), subtelas[selecao - 1]);
                           }
                        }
                        noecho();
                        cbreak();
                        mudou = 1;
                        break;
                        case CPF_CNPJ:
                        char inter[20];
                        {
                           keypad(subtelas[selecao - 1], TRUE);
                           cbreak();
                           werase(subtelas[selecao - 1]);
                           wrefresh(subtelas[selecao - 1]);
                           wmove(subtelas[selecao - 1], 0, 0);
                           if(atual->dataType == PESSOA_FISICA)
                           {
                              short check = lerCPF(inter, subtelas[selecao - 1]);
                              if(!check) goto cancelouCPF_CNPJ;
                              Node *repetido = findCByCPF_CNPJ(listaClientes, inter);
                              if(repetido == atual) goto cancelouCPF_CNPJ;
               
                              while(!validar_cpf(inter) || repetido)
                              {
                                 noecho();
                                 curs_set(FALSE);
                                 if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado já está cadastrado no sistema");
                                 else gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                                 werase(subtelas[selecao - 1]);
                                 wrefresh(subtelas[selecao - 1]);
                                 wmove(subtelas[selecao - 1], 0, 0);
                                 echo();
                                 curs_set(TRUE);
                                 check = lerCPF(inter, subtelas[selecao - 1]);
                                 if(!check) goto cancelouCPF_CNPJ;
                                 repetido = findCByCPF_CNPJ(listaClientes, inter);
                                 if(repetido == atual) goto cancelouCPF_CNPJ;
                              }
                              strcpy(cpf_cnpj, inter);
                           }
                           else
                           {
                              short check = lerCNPJ(inter, subtelas[selecao - 1]);
                              if(!check) goto cancelouCPF_CNPJ;
                              Node *repetido = findCByCPF_CNPJ(listaClientes, inter);
                              if(repetido == atual) goto cancelouCPF_CNPJ;
                              while(!validar_cnpj(inter) || repetido)
                              {
                                 noecho();
                                 curs_set(FALSE);
                                 if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado já está cadastrado no sistema");
                                 else gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                                 werase(subtelas[selecao - 1]);
                                 wrefresh(subtelas[selecao - 1]);
                                 wmove(subtelas[selecao - 1], 0, 0);
                                 echo();
                                 curs_set(TRUE);
                                 check = lerCNPJ(inter, subtelas[selecao - 1]);
                                 if(!check) goto cancelouCPF_CNPJ;
                                 repetido = findCByCPF_CNPJ(listaClientes, inter);
                                 if(repetido == atual) goto cancelouCPF_CNPJ;
                              }
                              strcpy(cpf_cnpj, inter);
                           }
                        }
                        mudou = 1;                        
                        noecho();
                        cbreak();
                        break;
                        cancelouCPF_CNPJ:
                        werase(subtelas[selecao - 1]);
                        mvwaddstr(subtelas[selecao - 1], 0, 0, cpf_cnpj);
                        wrefresh(subtelas[selecao - 1]);
                        noecho();
                        cbreak();
                        break;
                     }
                  }
                  if(mudou)
                  {
                     wattron(stdscr, COLOR_PAIR(2));
                     mvwaddstr(stdscr, y * 6 + y * 0.5, xpos, "Salvar alterações");
                     wnoutrefresh(stdscr);
                     wattroff(stdscr, COLOR_PAIR(2));
                  }
                  break;
               }
               doupdate();
               curs_set(FALSE);
               cmd = wgetch(stdscr);
            }

            
            for(short i = 0; i < 5; i++) cortina(telas[i], 0, 0, y, 10);
            cortina(stdscr, 0,  y * 6 + y * 0.5,  y * 6 + y * 0.5 + 1, 0);
            
            curs_set(FALSE);
            
            cmd = KEY_UP;
            bool confirmar = false;

            WINDOW *alter = newwin(ydadostam,xdadostam, maxstdy * 0.5 - ydadostam * 0.5, xpos + maxstdx * (45.0/teladevx));
            WINDOW *subalter = derwin(alter, ydadostam - 2,xdadostam - 2, 1, 1);
            wborder(alter, '[', ']', '>', '<', '@', '@', '@', '@');

            mvwprintw(subalter, 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                          idAlt,
                                          nome,
                                          endereco,
                                          telefone,
                                          cpf_cnpj);

            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5, xpos, "Deseja realmente salvar alterações?");
            wattron(stdscr, COLOR_PAIR(3));
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 1, xpos, "(Essa ação não tem retorno)");
            wattroff(stdscr, COLOR_PAIR(3));

            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 3, xpos + 3, "Sim");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 4, xpos + 3, "Não");

            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 7, xpos, "                 ██╗  ");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 8, xpos, "                 ╚██╗ ");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 9, xpos, "█████╗█████╗█████╗╚██╗");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 +10, xpos, "╚════╝╚════╝╚════╝██╔╝");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 +11, xpos, "                 ██╔╝ ");
            mvwaddstr(stdscr, maxstdy * 0.5 - ydadostam * 0.5 +12, xpos, "                 ╚═╝  ");
            wnoutrefresh(stdscr);
            wnoutrefresh(alter);
            wnoutrefresh(subalter);
            doupdate();

            while(cmd != '\n')
            {
               switch(cmd)
               {
                  case 'Z':
                  case 'z':
                  mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 3, xpos, ' ');
                  mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 4, xpos, ' ');
                  confirmar = 0;
                  goto skip2;
                  break;
                  case KEY_UP:
                  if(!confirmar)
                  {
                     mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 4, xpos, ' ');
                     mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 3, xpos, '>');
                     wrefresh(stdscr);
                     confirmar = !confirmar;
                  }
                  break;
                  case KEY_DOWN:
                  if(confirmar)
                  {
                     mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 3, xpos, ' ');
                     mvwaddch(stdscr, maxstdy * 0.5 - ydadostam * 0.5 + 4, xpos, '>');
                     wrefresh(stdscr);
                     confirmar = !confirmar;
                  }
                  break;
               }
               wrefresh(stdscr);
               cmd = wgetch(stdscr);
            }

            skip2:

            cortina(alter, 0, 0, ydadostam, 5);
            delwin(alter);
            delwin(subalter);

            cortina(stdscr, xpos, maxstdy * 0.5 - ydadostam * 0.5, maxstdy * 0.5 - ydadostam * 0.5 + 12, 5);

            if(!confirmar) goto reload;

            expand_node(atual, GenericCast)->data.id = idAlt;
            strncpy(expand_node(atual, GenericCast)->data.name, nome, 100);
            strncpy(expand_node(atual, GenericCast)->data.address, endereco, 200);
            strncpy(expand_node(atual, GenericCast)->data.phonenumber, telefone, 21);
            strcpy((atual->dataType == PESSOA_FISICA ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj), cpf_cnpj);

            cortina(alter, 0, 0, ydadostam, 5);
            delwin(alter);
            delwin(subalter);

            cortina(stdscr, xpos, maxstdy * 0.5 - ydadostam * 0.5, maxstdy * 0.5 - ydadostam * 0.5 + 12, 5);

            for(short i = 0; i < 5; i++) {delwin(telas[i]); delwin(subtelas[i]);}
            wattron(subdados, COLOR_PAIR(2));
            break;
            cancelar:
            
            mvwaddch(stdscr, setay, xpos - 3, ' ');
            wrefresh(stdscr);

            for(short i = 0; i < 5; i++)
            {
               cortina(telas[i], 0, 0, y, 10);
               delwin(subtelas[i]);
               delwin(telas[i]);
            }

            abrir(stdscr, y * 6 + y * 0.5, 1, 0);
            
         }
         break;
         case 'i':
         case 'I':
         {
            WINDOW *selid = newwin(3, xdadostam, maxstdy * 0.5 - ydadostam * 0.5 - 3, maxstdx * (48.0/teladevx));
            WINDOW *subselid = derwin(selid, 1, xdadostam - 2, 1, 1);
            wborder(selid, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
            waddstr(selid, "Consultar ID");
            wnoutrefresh(selid);
            doupdate();

            again:
            echo();
            nocbreak();
            curs_set(TRUE);
            werase(subselid);
            size_t targetID;
            short check = lerSizeT(&targetID, subselid);
            if(check == -3) goto cancelouBusca;
            while(check != 1)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
               if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
               if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
               nocbreak();
               echo();
               curs_set(TRUE);
               werase(subselid);
               wrefresh(subselid);
               wmove(subselid, 0, 0);
               check = lerSizeT(&targetID, subselid);
               if(check == -3) goto cancelouBusca;
            }

            Node *tmp = findByID(listaClientes, targetID);

            if(!tmp)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
               werase(subselid);
               wrefresh(subselid);
               wmove(subselid, 0, 0);
               goto again;
            }
            atual = tmp;
            cancelouBusca:
            cortina(selid, 0, 0, 3, 5);
            delwin(subselid);
            delwin(selid);

            cbreak();
            noecho();
         }
         break;
      }
      werase(subdados);

      mvwprintw(subdados, 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                 expand_node(atual, GenericCast)->data.id,
                                 expand_node(atual, GenericCast)->data.name,
                                 expand_node(atual, GenericCast)->data.address,
                                 expand_node(atual, GenericCast)->data.phonenumber,
                                 ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));
   
      wnoutrefresh(subdados);
      curs_set(FALSE);
      doupdate();
      wattroff(subdados, COLOR_PAIR(2));
      cmd = wgetch(stdscr);
   }

   return;

   listavazia:
   curs_set(FALSE);
   gerarAviso(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (10.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
   curs_set(TRUE);
}

Node* findCByCPF_CNPJ(List *lista, const char *cpf_cnpj)
{
   if(!lista->head) return NULL;

   if(cpf_cnpj[2] == '.')
   {
      for(Node *path = lista->head; path; path = path->next)
      {
         if(path->dataType == PESSOA_JURIDICA)
         {
            if(strcmp(expand_node(path, PessoaJuridica)->cnpj, cpf_cnpj) == 0) return path;
         }
      }

      return NULL;
   }

   for(Node *path = lista->head; path; path = path->next)
   {
      if(path->dataType == PESSOA_FISICA)
      {
         if(strcmp(expand_node(path, PessoaFisica)->cpf, cpf_cnpj) == 0) return path;
      }
   }

   return NULL;
}