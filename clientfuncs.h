#ifndef CLIENTFUNCS_H
#define CLIENTFUNCS_H

// codigos de cores
#define CLLR "\033[1;31m"
#define CLLG "\033[1;32m"
#define CLLB "\033[1;34m"
#define CLLP "\033[1;35m"
#define CLLC "\033[1;36m"
#define CLW "\033[0m"
#define CLY "\033[0;33m"

// escreve colorido
#define PRINTCL(COLOR,...) printf(COLOR), printf(__VA_ARGS__), printf(CLW)

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

/* ---------------------------- funcoes basicas do cliente [IMPRIMIR Extras] ------------------------- */
// imprime resposta de uma operacao
void printResp(char res, int op);
// imprime menu de opcoes (tipo = 0 é usuario padrao e tipo = 1 é usuario admin)
void printMenu(int tipo);

#endif
