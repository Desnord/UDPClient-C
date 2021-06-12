#ifndef CLIENTFUNCS_H
#define CLIENTFUNCS_H

/* macros extras ------------------ */
// usados apenas para simplificação do código / melhor apresentação das informações
#define CLLR "\033[1;31m"
#define CLLG "\033[1;32m"
#define CLW "\033[0m"
#define CLLB "\033[1;34m"
#define CLLP "\033[1;35m"
#define CLLC "\033[1;36m"
#define CLY "\033[0;33m"

// escreve colorido
#define PRINTCL(COLOR,...) printf(COLOR), printf(__VA_ARGS__), printf(CLW)
// imita escrita e leitura em apenas um comando
#define INPUT(texto1,...) printf(texto1), scanf(__VA_ARGS__)
// escreve com duas cores em sequencia
#define PRINT2C(A,COLOR1,COLOR2,...) printf(COLOR1), printf(A), printf(COLOR2), printf(__VA_ARGS__), printf(CLW)

/* ------------------------ funcoes basicas do cliente [IMPRIMIR INFORMAÇÕES] ------------------------ */
// imprime lista de string (lista de habilidades ou lista de experiencia)
void printListaString(NoString *lista);
// imprime um perfil
void printPerfil(Perfil *p);
// imprime lista de perfis
void printListaPerfil(NoPerfil *lista);
// imprime lista de perfis simplificada (nome,sobrenome,email)
void printListaNomeEmail(NoPerfilEmailNome *lista);
// imprime lista de perfis simplificada (nome,sobrenome,email e curso)
void printListaNomeEmailCurso(NoPerfilEmailNomeCurso *lista);

/* --------------------------- funcoes basicas do cliente [IMPRIMIR / Extras] ------------------------ */
// trata encerramento do programa em erro de conexão
void printConnStatus(char t, char *ip);
// imprime resposta de uma operacao
void printResp(char res, int op);
// imprime menu de opcoes (de acordo com o tipo de usuario [Admin imprime todas as opcoes])
void printMenu(char *user);
// le ip, porta e tipo de usuario do arquivo (retorna porta)
int getConfig(char *ip, char *user);
#endif
