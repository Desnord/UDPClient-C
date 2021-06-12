#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* inclui arquivos auxiliares ----- */
#include "listas.h"
#include "clientfuncs.h"

/* SOCKET ------------------------- */
#include <netdb.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
typedef struct sockaddr_in SockAddr_in;
typedef struct sockaddr SockAddr;

#define TEMP "tempfile.txt"

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/

// leitura dos dados de cada opcao possivel (depois gera o arquivo temporario de envio)
void leituraMSG(char *OptBuffer)
{
    FILE *temp = fopen(TEMP,"w");
    fprintf(temp,"%s\n", OptBuffer);

    if(!strcmp(OptBuffer,"1"))
    {
        char str[100];
        printf("digite o curso: ");
        gets(str);

        fprintf(temp,"%s\n", str);
    }
    else if(!strcmp(OptBuffer,"2"))
    {
        char str[100];
        printf("digite a habilidade: ");
        gets(str);

        fprintf(temp,"%s\n", str);
    }
    else if(!strcmp(OptBuffer,"3"))
    {
        int ano;
        INPUT("digite o ano: ", "%d%*c", &ano);

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
        char str[100];
        char labels[5][23] = {"o email","o nome","o sobrenome","a cidade de residencia","a formacao"};

        for(int i=0; i<5; i++)
        {
            printf("digite %s: ",labels[i]);
            gets(str);
            fprintf(temp,"%s\n", str);
        }
        int ano;
        INPUT("digite o ano de formatura: ", "%d%*c", &ano);
        fprintf(temp,"%d\n", ano);

        NoString *auxhab = (NoString*)malloc(sizeof(NoString));
        NoString *auxexp = (NoString*)malloc(sizeof(NoString));
        auxexp->str = NULL;
        auxhab->str = NULL;
        auxexp->prox = NULL;
        auxhab->prox = NULL;

        // leitura dos dados mais complexos
        fprintf(temp,"%s\n", "+=========+");
        char opt2 = 's';
        char *exphab;

        INPUT("inserir habilidade (s) ou (n): ", "%c", &opt2);
        while(opt2 == 's')
        {
            exphab = malloc(sizeof(char)*100);
            INPUT("digite uma habilidade: ", "%s%*c", exphab);
            !existeString(exphab,auxhab) ? stringListInsert(auxhab,exphab) : free(exphab);
            INPUT("continuar inserindo habilidades (s) ou (n): ", "%c%*c", &opt2);
        }

        for(NoString *at=auxhab->prox; at!=NULL; at = at->prox)
            fprintf(temp,"%s\n", at->str);


        fprintf(temp,"%s\n", "+=========+");
        opt2 = 's';
        INPUT("inserir experiência (s) ou (n): ", "%c", &opt2);
        while(opt2 == 's')
        {
            exphab = malloc(sizeof(char)*100);
            INPUT("digite uma experiência: ", "%s%*c", exphab);
            !existeString(exphab,auxexp) ? stringListInsert(auxexp,exphab) : free(exphab);
            INPUT("continuar inserindo experiências (s) ou (n): ", "%c%*c", &opt2);
        }

        for(NoString *at=auxexp->prox; at!=NULL; at = at->prox)
            fprintf(temp,"%s\n", at->str);
        fprintf(temp,"%s\n", "+=========+");

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

// envio dos dados do arquivo temporario ao servidor
void enviaMSG(int socketFD, SockAddr_in server)
{
    FILE *temp = fopen(TEMP, "r");
    char line[200];

    while(fgets( line, 200, temp) != NULL)
        sendto(socketFD, line, 200, 0, (SockAddr*)&server, sizeof(server));
    strcpy(line,"EOF");
    sendto(socketFD,  line, 200, 0, (SockAddr*)&server, sizeof(server));

    fclose(temp);
    remove(TEMP);
}

// recebimento da mensagem do servidor, dados ou resposta da operacao (gera arquivo temporario com os dados)
void recebeMSG(int socketFD, SockAddr_in server, int *len)
{
    FILE *temp = fopen(TEMP, "w");
    for(;;)
    {
        char RBuffer[200];
        memset(RBuffer, 0, sizeof(RBuffer));
        recvfrom(socketFD, RBuffer, sizeof(RBuffer), 0, (SockAddr*)&server, len);
        if (strcmp(RBuffer, "EOF") == 0)
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
        NoPerfilEmailNome *lista = malloc(sizeof(NoPerfilEmailNome));
        lista->perfil = NULL;
        lista->prox = NULL;

        char line[200],email[100],nome[100],sobrenome[100];
        int ctd = 0;
        while(fgets(line,200,temp))
        {
            if(!strcmp(line,"NULL"))
            {
                lista = NULL;
                break;
            }
            else if(!strcmp(line,"+=========+"))
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
                line[(int)strlen(line)-1] = '\0';

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
        NoPerfilEmailNomeCurso *lista = malloc(sizeof(NoPerfilEmailNomeCurso));
        lista->perfil = NULL;
        lista->prox = NULL;

        char line[200],email[100],nome[100],sobrenome[100],curso[100];
        int ctd = 0;
        while(fgets(line,200,temp))
        {
            if(!strcmp(line,"NULL"))
            {
                lista = NULL;
                break;
            }
            else if(!strcmp(line,"+=========+"))
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
                line[(int)strlen(line)-1] = '\0';

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

    }
    else if(!strcmp(OptBuffer,"5"))
    {
        Perfil *p = (Perfil*)malloc(sizeof(Perfil));
        p->experiencia = (NoString *)malloc(sizeof(NoString));
        p->habilidades = (NoString *)malloc(sizeof(NoString));
        p->habilidades->str = NULL;
        p->habilidades->prox = NULL;
        p->experiencia->str = NULL;
        p->experiencia->prox = NULL;

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
                line[(int)strlen(line)-1] = '\0';
                p->ano_formatura = (int)strtol(line,NULL,10);
                ctd++;
            }
            else if(ctd < 8)
            {
                NoString *STRlist =  p->habilidades;
                if(!strcmp(line,"+=========+"))
                    ctd++;
                else
                {
                    line[(int)strlen(line)-1] = '\0';
                    char *strAUX = malloc(sizeof(char) * ((int)strlen(line)+1));
                    strcpy(strAUX, line);

                    // insere na lista de habilidades/experiencia
                    stringListInsert(STRlist, strAUX);
                }
            }
            else
            {
                NoString *STRlist = p->experiencia;
                if(!strcmp(line,"+=========+"))
                    ctd++;
                else
                {
                    line[(int)strlen(line)-1] = '\0';
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
        char line[10] = "N";
        if(fgets(line,10,temp))
            printResp(OptBuffer[0], (int)line[0]);
    }
    fclose(temp);
    remove(TEMP);
}

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/

void comunicacao(char *user, int socketFD, SockAddr_in server)
{
    int len = sizeof(server);
    for(;;)
    {
        char OPBuffer[10];
        memset(OPBuffer, '\0', 10);

        // [imprime menu de opcoes]
        printMenu(user);

        // [le tipo de operacao]
        INPUT("escolha uma opção: ", "%[^\n]%*c", OPBuffer);
        printf("\n");

        // [usuario nao eh o admin, entao apenas ignora que ele digitou uma opcao privilegiada]
        if(strcmp(user,"Admin") != 0 && (!strcmp(OPBuffer,"6") || !strcmp(OPBuffer,"7") || !strcmp(OPBuffer,"8") || !strcmp(OPBuffer,"ENCERRAR")))
        {
            system("clear");
            continue;
        }

        // encerrar programa
        if(!strcmp(OPBuffer,"0"))
            break;

        leituraMSG(OPBuffer);             // realiza leitura dos dados da opcao (quando houver) e cria arquivo temp para enviar ao servidor
        enviaMSG(socketFD,server);        // envia mensagem ao servidor
        //recebeMSG(socketFD,server,&len);  // recebe resposta do servidor
        //recebeMSG(socketFD,server,&len);  // recebe resposta do servidor
        //imprimeMSG(OPBuffer);             // imprime informacao de acordo com a mensagem recebida

        // [aguarda confirmacao para limpar o terminal]
        INPUT("pressione uma tecla para continuar...", "%*c");
        printf("\n");
        system("clear");
    }
}

/* ----------------------------------------------------- MAIN ------------------------------------------------------ */
int main()
{
    // le ip, porta e tipo de usuario no arquivo de configuracao
    char user[7] = "Normal";        // valor (default)
    char ip[16] = "127.0.0.1";      // valor (default)
    int port = getConfig(ip,user);  // valor default é 9000

    // SOCKET
    SockAddr_in server;                                      // estrutura do socket
    int socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // cria file descriptor do socket

    // verifica socket
    if (socketFD != -1)
    {
        // atribui valores à estrutura do socket
        server.sin_family = AF_INET;            // AF_INET é a familia de protocolos do IPV4
        server.sin_addr.s_addr = inet_addr(ip); // ip da máquina executando o servidor
        server.sin_port = htons(port);          // porta do servidor

        comunicacao(user,socketFD,server); // realiza troca de mensagem com o servidor
        close(socketFD);                   // encerra socket
        return 0;
    }
    printConnStatus('E',NULL);
}