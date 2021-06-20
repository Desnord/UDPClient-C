#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listas.h"
#include "clientfuncs.h"

/* SOCKET ------------------------- */
#include <netdb.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

#define CONFIG "ClientFiles/config.txt" // arquivo de configuracao
#define TEMP "tempfile.txt" // arquivo temporario

/* -------------------------- funcoes de recebimento e envio de informacoes -------------------------- */

void leituraMSG(char *OptBuffer) // leitura dos dados de cada opcao possivel (depois gera o arquivo temporario de envio)
{
    FILE *temp = fopen(TEMP,"w");
    fprintf(temp,"%s\n", OptBuffer);

    if(!strcmp(OptBuffer,"1") || !strcmp(OptBuffer,"2"))
    {
        char str[100];

        if (!strcmp(OptBuffer,"1"))
          printf("digite o curso: ");
        else
          printf("digite a habilidade: ");

        gets(str);
        fprintf(temp,"%s\n", str);
    }
    else if(!strcmp(OptBuffer,"3"))
    {
        int ano;
        char str[100];
		    memset(str,'\0',200);
        printf("digite o ano: ");
        gets(str);
        ano = (int)strtol(str, (char **)NULL, 10);
        fprintf(temp,"%d\n", ano);
    }
    else if(!strcmp(OptBuffer,"5"))
    {
        char str[100];
        printf("digite o email: ");
        gets(str);
        fprintf(temp,"%s\n", str);
    }
    else if(!strcmp(OptBuffer,"6"))
    {
        char *infos[6], labels[6][23] = {"o email","o nome","o sobrenome","a cidade de residencia","a formacao","o ano de formatura"};

        for(int i=0; i<6; i++)
        {
          infos[i] = malloc(sizeof(char)*200);
          printf("Digite %s: ",labels[i]);
          gets(infos[i]);
        }

        // aloca memoria para os dados mais complexos
  			NoString *auxexp = newStringList();
  			NoString *auxhab = newStringList();

        // leitura dos dados mais complexos
  			char opt2[20];
  			printf("Inserir habilidade (s) ou (n): ");
  			gets(opt2);

        while (!strcmp(opt2,"s"))
				{
					printf("Digite uma habilidade: ");
					char *exphab = malloc(sizeof(char)*100);
					gets(exphab);
					!existeString(exphab,auxhab) ? stringListInsert(auxhab,exphab) : free(exphab);

					printf("Continuar inserindo habilidades (s) ou (n): ");
					gets(opt2);
				}

        printf("Inserir experiência (s) ou (n): ");
  			gets(opt2);
        while (!strcmp(opt2,"s"))
				{
					printf("Digite uma experiência: ");
					char *exphab = malloc(sizeof(char)*100);
					gets(exphab);
					!existeString(exphab,auxexp) ? stringListInsert(auxexp,exphab) : free(exphab);

					printf("Continuar inserindo experiência (s) ou (n): ");
					gets(opt2);
				}

        for(int i=0; i<5; i++)
        {
          fprintf(temp,"%s\n", infos[i]);
          free(infos[i]);
        }

        int ano = 0;
        ano = (int)strtol(infos[5], (char **)NULL, 10);
        fprintf(temp,"%d\n", ano);
        free(infos[5]);

        fprintf(temp,"%s\n", "+=====+");
        for(NoString *at=auxhab->prox; at!=NULL; at = at->prox)
            fprintf(temp,"%s\n", at->str);
        fprintf(temp,"%s\n", "+=====+");

        for(NoString *at=auxexp->prox; at!=NULL; at = at->prox)
            fprintf(temp,"%s\n", at->str);
        fprintf(temp,"%s\n", "+=====+");

        stringListFree(auxhab);
        stringListFree(auxexp);
    }
    else if(!strcmp(OptBuffer,"7"))
    {
        char eml[100];
        printf("digite o email: ");
        gets(eml);
        char exp[100];
        printf("digite a experiencia: ");
        gets(exp);

        fprintf(temp,"%s\n", eml);
        fprintf(temp,"%s\n", exp);
    }
    else if(!strcmp(OptBuffer,"8"))
    {
        char str[100];
        printf("digite o email: ");
        gets(str);
        fprintf(temp,"%s\n", str);
    }
    fclose(temp);
}

void enviaMSG(int socketFD, SockAddr_in server) // envio dos dados do arquivo temporario ao servidor
{
    FILE *temp = fopen(TEMP, "r");
    char line[200];
    memset(line,'\0',200);
    while(fgets( line, 200, temp) != NULL)
        sendto(socketFD, line, 200, 0, (SockAddr*)&server, sizeof(server));

    memset(line,'\0',200);
    strcpy(line,"EOF");
    sendto(socketFD,  line, 200, 0, (SockAddr*)&server, sizeof(server));

    fclose(temp);
    remove(TEMP);
}

void recebeMSG(int socketFD, SockAddr_in server, int *len) // recebimento da mensagem do servidor, dados ou resposta da operacao (gera arquivo temporario com os dados)
{
    FILE *temp = fopen(TEMP, "w");
    for(;;)
    {
        char RBuffer[200];
        memset(RBuffer, '\0', 200);
        recvfrom(socketFD, RBuffer, 200, 0, (SockAddr*)&server, len);
        //printf("%s\n", RBuffer);
        if (!strcmp(RBuffer, "EOF"))
            break;
        fprintf(temp, "%s", RBuffer);
    }
    fclose(temp);
}

// imprime mensagem recebida do servidor
void imprimeMSG(char *OptBuffer)
{
    FILE *temp = fopen(TEMP,"r");

    if(!strcmp(OptBuffer,"1") || !strcmp(OptBuffer,"2"))
    {
        NoPerfilEmailNome *lista = newNPENList();

        char line[200],email[100],nome[100],sobrenome[100];
        int ctd = 0;
        while(fgets(line,200,temp) != NULL)
        {
            line[(int)strlen(line)-1] = '\0';
            if(!strcmp(line,"NULL"))
            {
              break;
            }
            else if(!strcmp(line,"+=====+"))
            {
                PerfilEmailNome *p = malloc(sizeof(PerfilEmailNome));
                p->email = malloc((strlen(email)+1)*sizeof(char));
                p->nome = malloc((strlen(nome)+1)*sizeof(char));
                p->sobrenome = malloc((strlen(sobrenome)+1)*sizeof(char));

                strcpy(p->email,email);
                strcpy(p->nome,nome);
                strcpy(p->sobrenome,sobrenome);
                ctd = 0;
                NPENListInsert(lista,p);
            }
            else
            {
                if(!ctd)
                    strcpy(email,line);
                else if(ctd == 1)
                    strcpy(nome,line);
                else if(ctd == 2)
                    strcpy(sobrenome,line);
                ctd++;
            }
        }
        printListaNomeEmail(lista);
        NPENListFree(lista);
    }
    else if(!strcmp(OptBuffer,"3"))
    {
        NoPerfilEmailNomeCurso *lista = newNPENCList();

        char line[200],email[100],nome[100],sobrenome[100],curso[100];
        int ctd = 0;
        while(fgets(line,200,temp))
        {
            line[(int)strlen(line)-1] = '\0';
            if(!strcmp(line,"NULL"))
            {
              break;
            }
            else if(!strcmp(line,"+=====+"))
            {
                PerfilEmailNomeCurso *p = malloc(sizeof(PerfilEmailNomeCurso));
                p->email = malloc((strlen(email)+1)*sizeof(char));
                p->nome = malloc((strlen(nome)+1)*sizeof(char));
                p->sobrenome = malloc((strlen(sobrenome)+1)*sizeof(char));

                strcpy(p->email,email);
                strcpy(p->nome,nome);
                strcpy(p->sobrenome,sobrenome);
                strcpy(p->formacao,curso);
                ctd = 0;
                NPENCListInsert(lista,p);
            }
            else
            {
                if(!ctd)
                    strcpy(email,line);
                else if(ctd == 1)
                    strcpy(nome,line);
                else if(ctd == 2)
                    strcpy(sobrenome,line);
                else if(ctd == 3)
                    strcpy(curso,line);

                ctd++;
            }
        }
        printListaNomeEmailCurso(lista);
        NPENCListFree(lista);
    }
    else if(!strcmp(OptBuffer,"4"))
    {
      NoPerfil *lista = newPerfilList();
      printListaPerfil(lista);
      perfilListFree(lista);
    }
    else if(!strcmp(OptBuffer,"5"))
    {
        Perfil *p = newPerfil();
        int ctd = 0;
        char line[200];
        memset(line, 0, 200);
        while(fgets(line,200,temp))
        {
            line[(int)strlen(line)-1] = '\0';

            if(!strcmp(line,"NULL"))
            {
                free(p);
                printResp('0',5);
                return;
            }

            if(ctd < 5)
            {
                char *campo = malloc(sizeof(char) * ((int)strlen(line)+1));

                if(!ctd)
                    p->email = campo;
                else if(ctd == 1)
                    p->nome = campo;
                else if(ctd == 2)
                    p->sobrenome = campo;
                else if(ctd == 3)
                    p->cidade_residencia = campo;
                else if(ctd == 4)
                    p->formacao = campo;

                strcpy(campo,line);
                ctd++;
            }
            else if(ctd == 5)
            {
                p->ano_formatura = (int)strtol(line,NULL,10);
                ctd++;
            }
            else if(ctd < 8)
            {
                NoString *STRlist =  p->habilidades;
                if(!strcmp(line,"+=====+"))
                    ctd++;
                else
                {
                    char *strAUX = malloc(sizeof(char) * ((int)strlen(line)+1));
                    strcpy(strAUX, line);

                    // insere na lista de habilidades/experiencia
                    stringListInsert(STRlist, strAUX);
                }
            }
            else
            {
                NoString *STRlist = p->experiencia;
                if(!strcmp(line,"+=====+"))
                    ctd++;
                else
                {
                    char *strAUX = malloc(sizeof(char) * ((int)strlen(line)+1));
                    strcpy(strAUX, line);

                    // insere na lista de habilidades/experiencia
                    stringListInsert(STRlist, strAUX);
                }
            }
        }
        printResp('1',5);
        printPerfil(p);
        freePerfil(p);
    }
    else if(!strcmp(OptBuffer,"6") || !strcmp(OptBuffer,"7") || !strcmp(OptBuffer,"8"))
    {
        char line[200];
        memset(line,'\0',200);

        if(fgets(line,200,temp) != NULL)
        {
          printf("%s\n",line);
          printResp(line[0], (int)OptBuffer[0]-48);
        }
    }
    fclose(temp);
    remove(TEMP);
}

/* funcao de comunicação entre cliente-servidor [gerencia as 8 operações] */
void comunicacao(int socketFD, char *user, SockAddr_in server)
{
    for (;;)
    {
  		char OPBuffer[200];
  		memset(OPBuffer, '\0', 200);

  		// [imprime menu de opcoes]
  		printMenu(!strcmp(user,"Admin"));

  		printf("Escolha uma opção: ");
  		gets(OPBuffer);
  		printf("\n");

      // [usuario nao eh o admin, entao apenas ignora que ele digitou uma opcao privilegiada]
      if(strcmp(user,"Admin") && !(!strcmp(OPBuffer,"6") || !strcmp(OPBuffer,"7") || !strcmp(OPBuffer,"8") || !strcmp(OPBuffer,"ENCERRAR")))
      {
        printf("Pressione uma tecla para continuar...");
        getchar();
        printf("\n");
        system("clear");
        continue;
      }
      else if(!strcmp(OPBuffer,"0") || (!strcmp(OPBuffer,"ENCERRAR") && strcmp(user,"Admin"))) // encerrar programa
      {
  			break;
      }
  		else // envia operacao ao servidor + recebe resposta
  		{
        int len = sizeof(server);
        leituraMSG(OPBuffer);             // realiza leitura dos dados da opcao (quando houver) e cria arquivo temp para enviar ao servidor
        enviaMSG(socketFD,server);        // envia mensagem ao servidor
        recebeMSG(socketFD,server,&len);  // recebe resposta do servidor
        imprimeMSG(OPBuffer);             // imprime informacao de acordo com a mensagem recebida
  		}

      printf("Pressione uma tecla para continuar...");
      getchar();
      printf("\n");
      system("clear");
    }
}

int getConfig(char *ip, char *user) // le ip, porta e tipo de usuario do arquivo (retorna porta)
{
	// config [le ip, porta e usuario do arquivo]
	FILE *config = fopen(CONFIG,"r");
	int port = 9000;
	char str[12];

	if(config != NULL)
	{
		fscanf(config,"%s",user);
		fscanf(config,"%s",ip);
		fscanf(config,"%s",str);
		port = (int)strtol(str,NULL,10);
		fclose(config);
	}
	return port;
}

/* ----------------------------------------------------- MAIN ------------------------------------------------------ */
int main()
{
	// le ip, porta e tipo de usuario no arquivo de configuracao
	char user[7] = "Normal";        // valor (default)
	char ip[16] = "127.0.0.1";      // valor (default)
	int port = getConfig(ip,user);  // valor default é 9000

	SockAddr_in server;                                      // estrutura do socket
	int socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // cria file descriptor do socket

	if(socketFD != -1)
	{
		// atribui valores à estrutura do socket
		server.sin_family = AF_INET;            // AF_INET é a familia de protocolos do IPV4
		server.sin_addr.s_addr = inet_addr(ip); // ip da máquina executando o servidor
		server.sin_port = htons(port);          // porta do servidor

		comunicacao(socketFD, user, server); // realiza troca de mensagem com o servidor
		close(socketFD);             	       // encerra socket
	}

	return 0;
}
