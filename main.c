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

/* -------------------------- funcoes de recebimento e envio de informacoes -------------------------- */

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
	
		sendto(socketFD, OPBuffer, 200, 0, (SockAddr*)&server, sizeof(server));
		
		// [usuario nao eh o admin, entao apenas ignora que ele digitou uma opcao privilegiada]
		if(strcmp(user,"Admin") != 0 && (!strcmp(OPBuffer,"6") || !strcmp(OPBuffer,"7") || !strcmp(OPBuffer,"8") || !strcmp(OPBuffer,"ENCERRAR")))
		{
			system("clear");
			continue;
		}
		else if(!strcmp(OPBuffer,"0")) // encerrar programa
		{
			break;
		}
		else // envia operacao ao servidor + recebe resposta
		{
			sendto(socketFD, OPBuffer, 200, 0, (SockAddr*)&server, sizeof(server));
		}
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
	
	SockAddr_in server;                                      		   // estrutura do socket
	int socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // cria file descriptor do socket

	if(socketFD != -1)
	{
		// atribui valores à estrutura do socket
		server.sin_family = AF_INET;            // AF_INET é a familia de protocolos do IPV4
		server.sin_addr.s_addr = inet_addr(ip); // ip da máquina executando o servidor
		server.sin_port = htons(port);          // porta do servidor
		
		comunicacao(socketFD, user, server); // realiza troca de mensagem com o servidor
		close(socketFD);             	     // encerra socket
	}
	
	return 0;
}
