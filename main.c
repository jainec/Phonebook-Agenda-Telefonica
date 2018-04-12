
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char nome[30];
    char sobrenome[30];
    char email1[60], email2[60], email3[60];
    char numero1[30], numero2[30], numero3[30];
}agenda;

//Variaveis globais
agenda *contatos;
int nroContatos;
FILE *arquivo;

//Prototipos das funcoes
FILE* abreArquivo(char caminho[], char modo);
void fechaArquivo(FILE *arquivo);
void gravaNumero(void);
void gravaEmArquivo(void);
void carregaNumero(void);
void carregaArquivo(void);
int buscaContato (char n[], char s[]);
int compare(char n1[], char n2[], char s1[], char s2[]);
void insereOrd(agenda aux);
int buscaBinariaNome(char n[]);
int validaNumero(char numero[]);
int validaNome(char numero[]);
int validaEmail(char numero[]);
int apagaRegistro(char n[], char s[]);
void imprimeMenuES(void);
void verificaNumeroES(char numero[]);
void verificaNomeES(char numero[]);
void verificaEmailES(char numero[]);
int adicionaContatoES();
void apagaContatoES(void);
int buscaEnavegaPelaAgendaES();
void imprimeES(void);

int main(){
    carregaArquivo();
    int opcao;

    do{
        imprimeMenuES();
        printf("Insira uma opcao: ");
        scanf("%d", &opcao);
        getchar();
        system("cls");

        switch (opcao){
            case 1:
                adicionaContatoES();
                break;
            case 2:
                buscaEnavegaPelaAgendaES();
                break;
            case 3:
                apagaContatoES();
                break;
            case 4:
                imprimeES();
                break;
            case 0:
                gravaEmArquivo();
                free(contatos);
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }

    }while (opcao != 0);

    return 0;
}

//****************FUNCOES DE NEGOCIO*******************

//equivale ao fopen
FILE* abreArquivo(char caminho[30], char modo){
    FILE *arquivo;
    switch(modo){
        case 'g':
            arquivo = fopen(caminho, "wt");
            break;
        case 'l':
            arquivo = fopen(caminho, "rt");
            break;
        case 'a':
            arquivo = fopen(caminho, "a");
            break;
    }
    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo");
        exit(0);
    }
    return arquivo;
}

//equivale ao fclose
void fechaArquivo(FILE *arquivo){
    fclose(arquivo);
}

//grava o valor que estiver na variavel global 'nroContatos' em um arquivo
void gravaNumero(){
    arquivo = abreArquivo("numero.txt", 'g');
    fprintf(arquivo, "%d", nroContatos);
    fechaArquivo(arquivo);
}

//grava todos os contatos em outro arquivo
void gravaEmArquivo(){
    int i;
    gravaNumero();
    arquivo = abreArquivo("agenda.txt", 'g');
      for(i = 0; i <= nroContatos; i++){
        fprintf(arquivo, contatos[i].nome);
        fprintf(arquivo, contatos[i].sobrenome);
        fprintf(arquivo, contatos[i].email1);
        fprintf(arquivo, contatos[i].email2);
        fprintf(arquivo, contatos[i].email3);
        fprintf(arquivo, contatos[i].numero1);
        fprintf(arquivo, contatos[i].numero2);
        fprintf(arquivo, contatos[i].numero3);
      }
    fechaArquivo(arquivo);
}

void carregaNumero(){
    arquivo = abreArquivo("numero.txt", 'l');
    fscanf(arquivo, "%d", &nroContatos);
    fechaArquivo(arquivo);
}

void carregaArquivo(){
    carregaNumero();
    contatos = calloc(nroContatos+1, sizeof(agenda));
    int i;
    arquivo = abreArquivo("agenda.txt", 'l');
    for(i = 0; i <= nroContatos; i++){
        fgets(contatos[i].nome, 30, arquivo);
        fgets(contatos[i].sobrenome, 30, arquivo);
        fgets(contatos[i].email1, 60, arquivo);
        fgets(contatos[i].email2, 60, arquivo);
        fgets(contatos[i].email3, 60, arquivo);
        fgets(contatos[i].numero1, 30, arquivo);
        fgets(contatos[i].numero2, 30, arquivo);
        fgets(contatos[i].numero3, 30, arquivo);
    }
    fechaArquivo(arquivo);
}

//busca binaria por nome nome e sobrenome para evitar duplicacoes na agenda
//retorna um índice se o contato buscado for encontrado no array, retorna -1 caso contrario
int buscaContato (char n[], char s[]){
    int e, m, d;     //e = esquerda, m = meio, d = direita
    e = 0; d = nroContatos-1;

    while(e <= d) {
        m = (e+d)/2;
        if( (strcmp(n, contatos[m].nome) == 0) && (strcmp(s, contatos[m].sobrenome) == 0) ){
            return m;
        }
        else if(strcmp(n, contatos[m].nome) == 0 && strcmp(s, contatos[m].sobrenome) < 0)
            d = m - 1;

        else if(strcmp(n, contatos[m].nome) == 0 && strcmp(s, contatos[m].sobrenome) > 0)
            e = m + 1;

        else if (strcmp(n, contatos[m].nome) < 0)
            d = m - 1;

        else if ( strcmp(n, contatos[m].nome) > 0)
            e = m + 1;
    }

    return -1;
}

//retorna se os nomes e/ou sobrenomes não estiverem ordenados
int compare(char n1[], char n2[], char s1[], char s2[]){
    return strcmp(n1, n2) > 0 ||
            strcmp(n1, n2) == 0 &&
            strcmp(s1, s2) > 0;
}

//insere um contato ordenadamente
void insereOrd(agenda aux) {
  int i = nroContatos - 1;
  while (i >= 0 && compare(contatos[i].nome, aux.nome, contatos[i].sobrenome, aux.sobrenome)) {
    contatos[i+1] = contatos[i];
    i--;
  }
  contatos[i+1] = aux;
  nroContatos++;
}

//retorna um indice se o nome buscado for encontrado no array, retorna -1 caso contrario
int buscaBinariaNome(char n[]) {
    int e, m, d;     //e = esquerda, m = meio, d = direita
    e = 0; d = nroContatos-1;

    while(e <= d) {
        m = (e+d)/2;
        if( strcmp(n, contatos[m].nome) == 0){
            return m;
        }
        if (strcmp(n, contatos[m].nome) < 0)
            d = m - 1;
        else if (strcmp(n, contatos[m].nome) > 0)
            e = m + 1;
    }
    return -1;
}

//verifica se o numero inserido satisfaz as condições impostas no projeto
//se satisfazer retorna 1, ao contrario retorna -1
int validaNumero(char numero[]){
    int i, count = 0;
    for(i = 0; i < strlen(numero); i++){
        if(isdigit(numero[i]) || numero[i] == '(' || numero[i] == ')' || numero[i] == '+' || numero[i] == ' ' || numero[i] == '\n')
            count++;
    }

    if(count == strlen(numero))
        return 1;
    else
        return -1;
}

//verifica se o nome/sobrenome inserido satisfaz as condições impostas no projeto
//se satisfazer retorna 1, ao contrario retorna -1
int validaNome(char nome[]){
    int i, count = 0;

    if(nome[0] == '\n')
        return -1;

    for(i = 0; i < strlen(nome); i++){
        if(isalpha(nome[i]))
            count++;
    }

    if(count == (strlen(nome)-1))
        return 1;
    else
        return -1;
}

//verifica se o email inserido satisfaz as condições impostas no projeto
//se satisfazer retorna 1, ao contrario retorna -1
int validaEmail(char email[]){
    int i, count = 0, count2 = 0;
    if(email[0] == '\n')
        return 1;

    for(i = 0; email[i] != '@'; i++){
        if('a' <= email[i] && email[i] <= 'z' )
            count++;
    }
    for(i = count; i < strlen(email); i++){
        if('a' <= email[i] && email[i] <= 'z' || email[i] == '.')
            count2++;
    }
    if(count != 0 && count2 != 0){
        if(count+count2 == strlen(email)-2)  //tamanho -2 count nao pega o '@'
            return 1;
    }
    else
        return -1;
}

//retorna 1 se o contato foi encontrado e apagado, retorna -1 caco contrario
int apagaRegistro(char n[], char s[]){
    int i, a = -1;
    if(buscaContato(n, s) != -1){
        i = buscaContato(n, s);
        while(i < nroContatos){
            contatos[i] = contatos[i+1];
            i++;
        }
        a = 1;
        nroContatos--;
    }
    if(a == -1)
        return -1;
    else
        return 1;
}

//FUNCOES DE ENTRADA/SAIDA

void imprimeMenuES(){
    printf("\t\t Menu da agenda: \n");
    printf("1. Adicionar contato\n");
    printf("2. Pesquisar contato pelo nome e navegar\n");
    printf("3. Apagar contato\n");
    printf("0. Sair\n");
}

//garante que enquanto o numero for invalido o usuario tera que tentar novamente
void verificaNumeroES(char numero[]){
    while(validaNumero(numero) == -1){
        printf("Numero invalido. Tente novamente: ");
        fgets(numero, 30, stdin);
        setbuf(stdin, NULL);
    }
}

//garante que enquanto o nome/sobrenome for invalido o usuario tera que tentar novamente
void verificaNomeES(char nome[]){
    while(validaNome(nome) == -1){
        printf("Entrada invalida. Tente novamente: ");
        fgets(nome, 30, stdin);
        setbuf(stdin, NULL);
    }
}

//garante que enquanto o email for invalido o usuario tera que tentar novamente
void verificaEmailES(char email[]){
    while(validaEmail(email) == -1){
        printf("Email invalido. Tente novamente: ");
        fgets(email, 60, stdin);
        setbuf(stdin, NULL);
    }
}

int adicionaContatoES(){
    agenda aux;
    printf("Insira o nome do contato: ");
    fgets(aux.nome, 30, stdin);
    setbuf(stdin, NULL);
    verificaNomeES(aux.nome);

    printf("Insira o sobrenome do contato: ");
    fgets(aux.sobrenome, 30, stdin);
    setbuf(stdin, NULL);
    verificaNomeES(aux.sobrenome);

    if(buscaContato(aux.nome, aux.sobrenome) != -1){
        printf("Contato ja cadastrado.\n");
        return 0;
    }

    else{
        printf("Insira ate tres emails do contato: \n");
        printf("Email 1: ");
        fgets(aux.email1, 60, stdin);
        setbuf(stdin, NULL);
        verificaEmailES(aux.email1);

        printf("Email 2: ");
        fgets(aux.email2, 60, stdin);
        setbuf(stdin, NULL);
        verificaEmailES(aux.email2);

        printf("Email 3: ");
        fgets(aux.email3, 60, stdin);
        setbuf(stdin, NULL);
        verificaEmailES(aux.email3);

        printf("Insira ate tres numeros telefonicos do contato: \n");
        printf("Numero 1: ");
        fgets(aux.numero1, 30, stdin);
        setbuf(stdin, NULL);
        verificaNumeroES(aux.numero1);

        printf("Numero 2: ");
        fgets(aux.numero2, 30, stdin);
        setbuf(stdin, NULL);
        verificaNumeroES(aux.numero2);

        printf("Numero 3: ");
        fgets(aux.numero3, 30, stdin);
        setbuf(stdin, NULL);
        verificaNumeroES(aux.numero3);
    }
    contatos = (agenda*) realloc (contatos, (nroContatos+1) * sizeof(agenda));
    insereOrd(aux);
    printf("Contato adicionado com sucesso.\n");
    system("pause");
}

void apagaContatoES(){
    char n[30], s[30];
    printf("Digite o nome do cantato que gostaria de apagar: ");
    fgets(n, 30, stdin);
    printf("Digite o sobrenome do contato que gosataria de apagar: ");
    fgets(s, 30, stdin);
    if(apagaRegistro(n, s) == 1)
        printf("Contato apagado com sucesso.\n");
    else
        printf("Contato nao encontrado.\n");
    system("pause");
}

int buscaEnavegaPelaAgendaES(){
    char n[30];
    printf("Digite o nome do contato que deseja pesquisar: ");
    fgets(n, 30, stdin);
    int opcao = -1;
    int i = buscaBinariaNome(n);

    if(i == -1){
        printf("Contato nao encontrado.\n");
        return 0;
    }
    else{
    printf("===============Contato %d===============\n", i+1);
    printf("\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", contatos[i].nome, contatos[i].sobrenome, contatos[i].email1, contatos[i].email2,
    contatos[i].email3, contatos[i].numero1, contatos[i].numero2, contatos[i].numero3);

        while(opcao != 0 ){
            printf("1.Anterior");
            printf("\t2.Proximo");
            printf("\t0.Sair\n");
            scanf("%d", &opcao);
            if(opcao == 2 || i == 0){
                i++;
                system("cls");
                printf("===============Contato %d===============\n", i+1);
                printf("\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", contatos[i].nome, contatos[i].sobrenome, contatos[i].email1,
                contatos[i].email2,contatos[i].email3, contatos[i].numero1, contatos[i].numero2, contatos[i].numero3);
            }

            if(opcao == 1 || i == nroContatos){
                i--;
                system("cls");
                printf("===============Contato %d===============\n", i+1);
                printf("\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", contatos[i].nome, contatos[i].sobrenome, contatos[i].email1,
                contatos[i].email2, contatos[i].email3, contatos[i].numero1, contatos[i].numero2, contatos[i].numero3);
            }
        }
    }
    system("cls");
    system("pause");
}

void imprimeES(){
    int i;
    for(i = 0; i < nroContatos; i++)
        printf("%s%s%s%s%s%s%s%s\n", contatos[i].nome, contatos[i].sobrenome, contatos[i].email1, contatos[i].email2,
        contatos[i].email3, contatos[i].numero1, contatos[i].numero2, contatos[i].numero3);
    system("pause");
}
