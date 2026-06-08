#include <stdio.h>
#include <string.h>

// ----- Constantes Globais ----
#define MAX_ALUNOS 100   // numero maximo de alunos cadastrados
#define MAX_NOME 40      // tamanho maximo do nome de cada aluno
// -----------------------------

// --- Garante (ou quase) Portabilidade ---
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
// -----------------------------

// --------- Decoracao ---------
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"
//#define DIVIDER "------------------------------------\n" // 36 * (-)
#define DIVIDER "====================================\n"   // 36 * (=)
// -----------------------------

// ----- Variaveis Globais -----
int totalAlunos = 0;                   // total de alunos cadastrados
unsigned int mat[MAX_ALUNOS] = {0};    // matriculas
float notas[MAX_ALUNOS][2] = {0};      // notas N1 e N2 dos alunos
float medias[MAX_ALUNOS] = {0};        // media dos alunos
char nomes[MAX_ALUNOS][MAX_NOME];            // nome dos alunos
int alunos[MAX_ALUNOS];                // indice dos alunos em ordem decrescente de media
/* 
struct tornaria isso muito mais simples, mas evitei a abstracao
na pratica, o indice dos vetores eh a real chave de identificacao dos alunos
mat[i], notas[i][j] e nomes[i] se referem a um mesmo aluno
*/ 
// -----------------------------

// ---------- Funcoes ----------
void clear () { // verifica o ambiente de compilacao e limpa o terminal
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux
    #endif
}

void await (int ms) { // verifica o ambiente de compilacao e para a execucao por 'ms' milissegundos
    #ifdef _WIN32
        Sleep(ms);          // Windows
    #else
        usleep(ms * 1000);  // Linux
    #endif
}

unsigned int readInt () { // garante tratamento na entrada de inteiros
    unsigned int value;

    while (scanf(" %u", &value) != 1) {
        printf("%s[ENTRADA INVALIDA]%s\n", RED, RESET);
        await(700);
        printf("\033[A\033[2K\r");
        printf("\033[A\033[2K\r");
        printf("> ");

        while(getchar() != '\n');
    }

    return value;
}

float readFloat () { // garante tratamento na entrada de float
    float value;

    while (scanf(" %f", &value) != 1 || value < 0.0 || value > 10.0) {
        printf("%s[ENTRADA INVALIDA]%s\n", RED, RESET);
        await(700);
        printf("\033[A\033[2K\r");
        printf("\033[A\033[2K\r");
        printf("> ");

        while(getchar() != '\n');
    }

    return value;
}

int* getAlunoByNome (char* nome, int al[MAX_ALUNOS]) { // retorna um vetor com o indice dos alunos, ou -1 caso nao encontre
    for (int i = 0; i < MAX_ALUNOS; i++) al[i] = -1;

    int j = 0;

    for (int i = 0; i < totalAlunos; i++) {
        if (!strcmp(nome, nomes[i])) al[j++] = i;
    }

    return al;
}

int getAlunoByMat (unsigned int matricula) { // retorna o indice do aluno, ou -1 caso nao encontre
    int i;
    for (i = 0; i < totalAlunos && mat[i] != matricula; i++);

    return i == totalAlunos ? -1 : i;
}

void bubbleSort () { // ordena o indice dos alunos em ordem decrescente de media
    for (int i = 0; i < totalAlunos; i++) alunos[i] = i;

    for (int i = 0; i < totalAlunos - 1; i++) {
        for (int j = 0; j < totalAlunos - i - 1; j++) {
            if (medias[alunos[j]] < medias[alunos[j + 1]]) {
                int aux = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = aux;
            }
        }
    }
}

int* getAprovados (int apv[3]) { // retorna o total de aprovados, reprovados e de recuperacao
    for (int i = 0; i < totalAlunos; i++) {
        if (medias[i] >= 6.0) apv[2]++;
        else if (medias[i] >= 4.0) apv[1]++;
        else apv[0]++;
    }

    return apv;
}

int* getMaiorMedia (int maiores[MAX_ALUNOS]) { // retorna todos os alunos com a maior media
    for (int i = 0; i < MAX_ALUNOS; i++) maiores[i] = -1;

    float maior = -1;

    for (int i = 0; i < totalAlunos; i++) {
        if (medias[i] > maior) maior = medias[i];
    }

    int j = 0;
    for (int i = 0; i < totalAlunos; i++) {
        if (medias[i] == maior) {
            maiores[j++] = i;
        }
    }

    return maiores;
}

int* getMenorMedia (int menores[MAX_ALUNOS]) { // retorna todos os alunos com a menor media
    for (int i = 0; i < MAX_ALUNOS; i++) menores[i] = -1;

    float menor = 999;

    for (int i = 0; i < totalAlunos; i++) {
        if (medias[i] < menor) menor = medias[i];
    }

    int j = 0;
    for (int i = 0; i < totalAlunos; i++) {
        if (medias[i] == menor) {
            menores[j++] = i;
        }
    }

    return menores;
}

float getMediaGeral () { // retorna a media da turma
    float media = 0;

    for (int i = 0; i < totalAlunos; i++) {
        media += medias[i];
    }

    return (media / totalAlunos);
}

const char* getColor (float nota) { // retorna uma cor baseado numa nota
    if (nota >= 6.0) return GREEN;
    if (nota >= 4.0) return YELLOW;
    return RED;
}

void exibirAluno (int i) { // recebe o indice do aluno e imprime as informacoes dele
    const char* colorN1 = getColor(notas[i][0]);
    const char* colorN2 = getColor(notas[i][1]);
    const char* colorStatus = getColor(medias[i]);
    
    printf("%s", DIVIDER);
    printf("%s[ALUNO ENCONTRADO]%s\n", GREEN, RESET);
    printf("%u\n", mat[i]);
    printf("%s\n", nomes[i]);
    printf("N1: %s%.1f%s     N2: %s%.1f%s\n", colorN1, notas[i][0], RESET, colorN2, notas[i][1], RESET);
    printf("Media Final: %s%.1f%s\n", colorStatus, medias[i], RESET);
    printf("Status: %s%s%s\n", colorStatus, (medias[i] >= 6.0) ? "APROVADO" : (medias[i] >= 4.0) ? "RECUPERACAO" : "REPROVADO", RESET);
    printf("%s", DIVIDER);
}

void setNota (float n1, float n2, int aluno) { // altera as notas de um aluno
    notas[aluno][0] = n1;
    notas[aluno][1] = n2;

    medias[aluno] = (n1 + n2) / 2.0;

    bubbleSort();
}

void buscarAlunos (int op) { // busca alunos enquanto o usuario quiser
    // op = 0: busca por nome / op = 1: busca por matricula

    if (op == 0) {
        char nome[40];

        printf("[Deixe em branco (Enter) para finalizar]\n");
        printf("%s", DIVIDER);

        while (1) {
            printf("Digite o nome do aluno\n> ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';

            if (nome[0] == 0) {
                clear();
                return;
            }

            await(250);
            clear();                    

            int al[MAX_ALUNOS]; // armazena todos os alunos cujo nome bate com o digitado
            getAlunoByNome(nome, al);
            
            if (al[0] == -1) printf("%s[ALUNO NAO ENCONTRADO]%s\n", RED, RESET);
            else {
                for (int i = 0; i < totalAlunos && al[i] != -1; i++) {
                    exibirAluno(al[i]);
                }
            }
        }
    }

    else {
        unsigned int matricula;
        
        printf("[Digite 0 para finalizar]\n");
        printf("%s", DIVIDER);

        while (1) {
            printf("Digite a matricula do aluno\n> ");
            matricula = readInt();

            if (!matricula) { // encerra as buscas
                clear();
                return;
            } 
            
            await(250);
            clear();                    

            int i = getAlunoByMat(matricula);
            
            if (i == -1) printf("%s[ALUNO NAO ENCONTRADO]%s\n", RED, RESET);
            else exibirAluno(i);
        }
    }
}

void cadastrarAlunos () { // cadastra alunos
    unsigned int matricula;
    char nome[MAX_NOME];
    
    if (totalAlunos < MAX_ALUNOS) {
        printf("[Digite 0 para finalizar]\n");
        printf("%s", DIVIDER);
    }

    while (1) {
        if (totalAlunos == MAX_ALUNOS) { // limite de alunos atingido
            printf("%s[LIMITE DE CADASTROS ATINGIDO (%d)]%s\n", RED, MAX_ALUNOS, RESET);
            await(1350);
            clear();
            return;
        }

        printf("Digite um numero de matricula\n> ");
        matricula = readInt();

        if (!matricula) { // encerra os cadastros
            clear();
            return;
        } 
        if (getAlunoByMat(matricula) != -1) {
            // matricula duplicada
            printf("%s[MATRICULA JA EXISTE]%s\n", RED, RESET);
            await(600);
            clear();
            continue;
        }

        printf("Digite o nome do aluno\n> ");
        while(getchar() != '\n');               // evita que o enter da matricula seja lido no fgets
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';

        await(200);
        clear();

        totalAlunos++;
        mat[totalAlunos - 1] = matricula;
        strcpy(nomes[totalAlunos - 1], nome);

        printf("%s", DIVIDER);
        printf("%s[ALUNO CADASTRADO - [%d / %d]]%s\n%d\n%s\n", GREEN, totalAlunos, MAX_ALUNOS, RESET, matricula, nome);
        printf("%s", DIVIDER);
    }
}

void lancarNotas () { // menu para lancar as notas dos alunos
    unsigned int matricula;

    printf("[Digite 0 para finalizar]\n");
    printf("[Lancar notas para um aluno que ja possui notas ira substituir as notas antigas]\n");
    printf("%s", DIVIDER);

    while (1) {
        printf("Digite a matricula do aluno\n> ");
        matricula = readInt();

        if (!matricula) { // encerra a edicao de notas
            clear();
            return;
        }

        int i = getAlunoByMat(matricula);

        await(200);
        clear();

        if (i == -1) printf("%s[ALUNO NAO ENCONTRADO]%s\n", RED, RESET);
        else {
            float n1 = 0, n2 = 0;

            printf("%s", DIVIDER);
            printf("Alterando as notas do aluno:\n   %s\n", nomes[i]);
            printf("%s", DIVIDER);

            printf("Digite a primeira nota do aluno\n> ");
            n1 = readFloat();

            printf("Digite a segunda nota do aluno\n> ");
            n2 = readFloat();

            printf("%s", DIVIDER);
            printf("%s[NOTAS ALTERADAS]%s\n%d\n%s\n", GREEN, RESET, matricula, nomes[i]);
            printf("N1: %.1f   ->   %.1f\n", notas[i][0], n1);
            printf("N2: %.1f   ->   %.1f\n", notas[i][1], n2);
            printf("%s", DIVIDER);
            setNota(n1, n2, i);
        }
    }
}

void gerarRelatorio () { // imprime todos os alunos em ordem decrescente de media
    if (!totalAlunos) printf("%s[Nenhum Cadastro Encontrado]%s\n\n", YELLOW, RESET);

    for (int i = 0; i < totalAlunos; i++) {
        printf("%s", DIVIDER);
        printf("ALUNO #%d\n", i + 1);
        exibirAluno(alunos[i]);
        printf("\n");
    }

    printf("[Pressione Enter para fechar]\n");
    getchar();
}

void gerarEstatisticas () { // gera e imprime as estatisticas da turma
    if (!totalAlunos) {
        printf("%s[TURMA VAZIA]%s\n", YELLOW, RESET);
        printf("\n[Pressione Enter para fechar]\n");
        getchar();
        return;
    } 

    int apv[3] = {0};
    int maior[MAX_ALUNOS], menor[MAX_ALUNOS];
    float media = getMediaGeral();
    getAprovados(apv);
    getMaiorMedia(maior);
    getMenorMedia(menor);

    printf("%s", DIVIDER);
    printf("Total de Alunos: %d\n", totalAlunos);
    printf("Media da Turma: %s%.1f%s\n", (media >= 6.0) ? GREEN : (media >= 4.0) ? YELLOW : RED, media, RESET);
    printf("Total de Aprovados: %s%d%s\n", GREEN, apv[2], RESET);
    printf("Total de Reprovados: %s%d%s\n", RED, apv[0], RESET);
    printf("Total de Recuperacao: %s%d%s\n", YELLOW, apv[1], RESET);
    printf("%s", DIVIDER);
    printf("Maior Media da Turma: %.1f\n", medias[maior[0]]);
    printf("Aluno(s):\n");
    for (int i = 0; i < totalAlunos && maior[i] != -1; i++) {
        printf("> %s   -   %d\n", nomes[maior[i]], mat[maior[i]]);
    }
    printf("%s", DIVIDER);
    printf("Menor Media da Turma: %.1f\n", medias[menor[0]]);
    printf("Aluno(s):\n");
    for (int i = 0; i < totalAlunos && menor[i] != -1; i++) {
        printf("> %s   -   %d\n", nomes[menor[i]], mat[menor[i]]);
    }
    printf("%s", DIVIDER);

    printf("[Pressione Enter para fechar]\n");
    getchar();
}

void createAluno (unsigned int matricula, char* nome, float n1, float n2) { // funcao pra debugging
    // facilita na hora de popular a lista de alunos para testes, mas eh descartavel
    totalAlunos++;
    mat[totalAlunos - 1] = matricula;
    strcpy(nomes[totalAlunos - 1], nome);
    setNota(n1, n2, totalAlunos - 1);
}

// -----------------------------

int main () {
    unsigned int option;
    
    do {
        clear();
        printf("%s", DIVIDER);
        printf("%sSistema Academico Inteligente%s\n", PURPLE, RESET);
        printf("%s", DIVIDER);

        printf("O que voce deseja fazer?\n");
        printf("[ 1 ] Cadastrar Alunos\n");
        printf("[ 2 ] Lancar Notas\n");
        printf("[ 3 ] Buscar Aluno por Nome\n");
        printf("[ 4 ] Buscar Aluno por Matricula\n");
        printf("[ 5 ] Gerar Relatorio\n");
        printf("[ 6 ] Estatisticas da Turma\n");
        printf("[ 0 ] Finalizar Sistema\n");
        printf("> ");
        option = readInt();

        switch (option) {
            case 1: // cadastro
                clear();
                cadastrarAlunos();
                break;

            case 2: // lanca notas
                clear();
                lancarNotas();
                break;
            
            case 3: // busca por nome
                clear();
                while(getchar() != '\n'); // consome o enter da opcao escolhida
                buscarAlunos(0);
                break;

            case 4: // busca por matricula
                clear();
                while(getchar() != '\n'); // consome o enter da opcao escolhida
                buscarAlunos(1);
                break;            

            case 5:
                clear();
                while(getchar() != '\n'); // consome o enter da opcao escolhida
                gerarRelatorio();
                break;

            case 6:
                clear();
                while(getchar() != '\n'); // consome o enter da opcao escolhida
                gerarEstatisticas();
                break;

            case 0: // finaliza
                await(300);
                clear();
                printf("%s[SISTEMA FINALIZADO]%s\n", YELLOW, RESET);
                break;

            default:
                printf("%sOPERACAO INVALIDA%s\n", RED, RESET);
                await(550);
                break;
        }
    } while (option);

    return 0;
}