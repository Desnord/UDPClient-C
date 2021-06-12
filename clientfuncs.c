#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listas.h"
#include "clientfuncs.h"

/* ------------------------ funcoes basicas do cliente [IMPRIMIR INFORMAÇÕES] ------------------------ */
void printListaString(NoString *lista)
{
    NoString *n_at;
    n_at = lista->prox;
    int ctd = 1;
    while (n_at != NULL)
    {
        printf(" [%02d] - %s \n",ctd,n_at->str);
        n_at = n_at->prox;
        ctd++;
    }
}
void printPerfil(Perfil *p)
{
    if(p == NULL)
        return;

    /* imprime os dados mais simples */
    printf(" Email: %s\n",p->email);
    printf(" Nome: %s Sobrenome: %s\n",p->nome,p->sobrenome);
    printf(" Residência: %s\n",p->cidade_residencia);
    printf(" Formação Acadêmica: %s\n",p->formacao);
    printf(" Ano de Formatura: %d\n\n",p->ano_formatura);

    // imprime lista de habilidades
    PRINTCL(CLLP," -> "), printf("Habilidades: \n");
    printListaString(p->habilidades);

    // imprime lista de experiencia
    PRINTCL(CLLP,"\n -> "), printf("Experiência: \n");
    printListaString(p->experiencia);
}
void printListaPerfil(NoPerfil *lista)
{
    if(lista == NULL)
        return;

    printf("\n");
    NoPerfil *at;
    at = lista->prox;
    int ctd = 0;
    while(at != NULL)
    {
        PRINTCL(CLLC," <Perfil [%03d]>\n",ctd);
        printPerfil(at->pessoa); /* print perfil atual */
        printf("\n");            /* separa saida       */
        at = at->prox;           /* proximo perfil     */
        ctd++;
    }
    printf("\n");
}
void printListaNomeEmail(NoPerfilEmailNome *lista)
{
    if(lista == NULL)
        return;

    printf("\n");
    NoPerfilEmailNome *at;
    at = lista->prox;

    int ctd = 0;
    while(at != NULL)
    {
        PRINTCL(CLLC," <Perfil [%03d]>\n",ctd);
        printf(" Email: %s \n",at->perfil->email);
        printf(" Nome: %s Sobrenome: %s \n\n",at->perfil->nome,at->perfil->sobrenome);
        at = at->prox;
        ctd++;
    }
    printf("\n");
}
void printListaNomeEmailCurso(NoPerfilEmailNomeCurso *lista)
{
    if(lista == NULL)
        return;

    printf("\n");
    NoPerfilEmailNomeCurso *at;
    at = lista->prox;

    int ctd = 0;
    while(at != NULL)
    {
        PRINTCL(CLLC," <Perfil [%03d]>\n",ctd);
        printf(" Email: %s \n",at->perfil->email);
        printf(" Nome: %s Sobrenome: %s \n",at->perfil->nome,at->perfil->sobrenome);
        printf(" Formação Acadêmica: %s \n\n",at->perfil->formacao);
        at = at->prox;
        ctd++;
    }
    printf("\n");
}
void printResp(char res, int op)
{
    if(op == 5)
    {
        if(res == '1')
            PRINTCL(CLLG, "Perfil encontrado: \n\n");
        else if(res == '0')
            PRINTCL(CLLR, "Não existe perfil cadastrado com o email informado. \n\n");
        else if(res == 'N')
            PRINTCL(CLLR,"Erro ao receber dados. \n\n");
    }
    else if(op == 6)
    {
        if(res == '1')
            PRINTCL(CLLG,"Perfil inserido com sucesso! \n\n");
        else if(res == '0')
            PRINTCL(CLLR,"Esse email já está registrado.\n\n");
        else if(res == 'N')
            PRINTCL(CLLR,"Erro ao receber confirmacao. \n\n");
    }
    else if(op == 7)
    {
        if(res == '2')
            PRINTCL(CLLG,"Experiência adicionada com sucesso! \n\n");
        else if(res == '0' || res == '1')
            PRINTCL(CLLR,"Experiência duplicada ou Perfil inexistente. \n\n");
        else if(res == 'N')
            PRINTCL(CLLR,"Erro ao receber confirmacao. \n\n");
    }
    else if(op == 8)
    {
        if(res == '1')
            PRINTCL(CLLG,"Perfil removido com sucesso! \n\n");
        else if(res == '0')
            PRINTCL(CLLR,"Perfil informado inexistente. \n\n");
        else if(res == 'N')
            PRINTCL(CLLR,"Erro ao receber confirmacao. \n\n");
    }
}
/* --------------------------- funcoes basicas do cliente [IMPRIMIR / Extras] ------------------------ */
void printConnStatus(char t, char *ip)
{
    if(t == 'E')
    {
        PRINT2C(" erro de conexão.\n",CLLR,CLW," pressione uma tecla para encerrar...");
        getchar();
        exit(0);
    }
    else
        PRINT2C("conectado ao servidor: ",CLLG,CLY,"%s\n",ip);
}
void printMenu(char *user)
{
    PRINTCL(CLLB,"-------------------------------------------------------------------------------------------\n");
          printf("| [0] - para sair                                                                         |\n");
          printf("| [1] - listar todas as pessoas (email e nome) formadas em um determinado curso           |\n");
          printf("| [2] - listar todas as pessoas (email e nome) que possuam uma determinada habilidade     |\n");
          printf("| [3] - listar todas as pessoas (email, nome e curso) formadas em um determinado ano      |\n");
          printf("| [4] - listar todas as informações de todos os perfis                                    |\n");
          printf("| [5] - dado o email de um perfil, retornar suas informações                              |\n");
    if(!strcmp(user,"Admin"))
    {
          printf("| [6] - cadastrar um novo perfil utilizando o email como identificador                    |\n");
          printf("| [7] - acrescentar uma nova experiência profissional em um perfil                        |\n");
          printf("| [8] - remover um perfil a partir de seu identificador (email)                           |\n");
    }
    PRINTCL(CLLB,"-------------------------------------------------------------------------------------------\n");
}
int getConfig(char *ip, char *user)
{
    // config [le ip, porta e usuario do arquivo]
    FILE *config = fopen("config.txt","r");
    int port = 9000;
    if(config != NULL)
    {
        fscanf(config,"%s",user);
        fscanf(config,"%s",ip);
        fscanf(config,"%d",&port);
        fclose(config);
    }
    return port;
}