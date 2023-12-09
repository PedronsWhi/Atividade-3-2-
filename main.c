#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Livro {
    char nome[100];
    char autor[100];
    struct Livro* prox;
};

struct FilaLivros {
    struct Livro* frente;
    struct Livro* fim;
};

struct PilhaLivros {
    struct Livro* topo;
};

struct Livro* criarLivro(const char* nome, const char* autor) {
    struct Livro* startLivro = (struct Livro*)malloc(sizeof(struct Livro));
    strcpy(startLivro->nome, nome);
    strcpy(startLivro->autor, autor);
    startLivro->prox = NULL;
    return startLivro;
}

struct FilaLivros* criarFilaLivros() {
    struct FilaLivros* fila = (struct FilaLivros*)malloc(sizeof(struct FilaLivros));
    fila->frente = fila->fim = NULL;
    return fila;
}

struct PilhaLivros* criarPilhaLivros() {
    struct PilhaLivros* pilha = (struct PilhaLivros*)malloc(sizeof(struct PilhaLivros));
    pilha->topo = NULL;
    return pilha;
}

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void inserirLivroFila(struct FilaLivros* fila, const char* nome, const char* autor) {
    struct Livro* startLivro = criarLivro(nome, autor);

    if (fila->frente == NULL) {
        fila->frente = fila->fim = startLivro;
    } else {
        fila->fim->prox = startLivro;
        fila->fim = startLivro;
    }
}

void inserirLivroPilha(struct PilhaLivros* pilha, const char* nome, const char* autor) {
    struct Livro* startLivro = criarLivro(nome, autor);
    startLivro->prox = pilha->topo;
    pilha->topo = startLivro;
}

void removerLivroFila(struct FilaLivros* fila, const char* startLivro) {
    if (fila->frente == NULL) {
        printf("A fila de livros está vazia.\n");
        return;
    }

    struct Livro* anterior = NULL;
    struct Livro* atual = fila->frente;

    while (atual != NULL) {
        if (strcmp(atual->nome, startLivro) == 0) {
            if (anterior == NULL) {
                fila->frente = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            printf("O livro '%s' foi removido da fila.\n", startLivro);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    printf("O livro '%s' não foi encontrado na fila de livros.\n", startLivro);
}

void removerLivroPilha(struct PilhaLivros* pilha, const char* startLivro) {
    if (pilha->topo == NULL) {
        printf("A pilha de livros está vazia.\n");
        return;
    }

    struct Livro* topo = pilha->topo;

    if (strcmp(topo->nome, startLivro) == 0) {
        pilha->topo = topo->prox;
        free(topo);
        printf("O livro '%s' foi removido da pilha.\n", startLivro);
        return;
    } else {
        printf("O livro '%s' não foi encontrado na pilha de livros.\n", startLivro);
    }
}

void exibirFilaLivros(struct FilaLivros* fila) {
    struct Livro* atual = fila->frente;
    printf("Fila de livros:\n");
    while (atual != NULL) {
        printf("Livro: %s - Autor: %s\n", atual->nome, atual->autor);
        atual = atual->prox;
    }
}

void exibirPilhaLivros(struct PilhaLivros* pilha) {
    struct Livro* atual = pilha->topo;
    printf("Pilha de livros:\n");
    while (atual != NULL) {
        printf("Livro: %s - Autor: %s\n", atual->nome, atual->autor);
        atual = atual->prox;
    }
}

int main() {
    struct FilaLivros* filaLivros = criarFilaLivros();
    struct PilhaLivros* pilhaLivros = criarPilhaLivros();

    FILE* arquivoFila = fopen("fila_de_livros.txt", "r");
    if (arquivoFila != NULL) {
        char linha[256];
        while (fgets(linha, sizeof(linha), arquivoFila)) {
            char nome[100], autor[100];
            sscanf(linha, "%99[^;];%99[^\n]", nome, autor);
            inserirLivroFila(filaLivros, nome, autor);
        }
        fclose(arquivoFila);
    }

    FILE* arquivoPilha = fopen("pilha_de_livros.txt", "r");
    if (arquivoPilha != NULL) {
        char linha[256];
        while (fgets(linha, sizeof(linha), arquivoPilha)) {
            char nome[100], autor[100];
            sscanf(linha, "%99[^;];%99[^\n]", nome, autor);
            inserirLivroPilha(pilhaLivros, nome, autor);
        }
        fclose(arquivoPilha);
    }

    int escolha;
    char startLivro[100], autorLivro[100];

    while (1) {
        printf("\033c");
        printf("\nMenu:\n");
        printf("1. Exibir a fila de livros -------------\n");
        printf("2. adicionar um novo livro na fila -----\n");
        printf("3. adicionar um novo livro na pilha ----\n");
        printf("4. Apagar um livro da fila -------------\n");
        printf("5. Apagar um livro da pilha ------------\n");
        printf("6. Sair\n");

        printf("Escolha uma das opções: ");
        scanf("%d", &escolha);
        printf("\033c");

        switch (escolha) {
            case 1:
                exibirFilaLivros(filaLivros);
                printf("Digite qualquer coisa para continuar\n");
                flush_stdin();
                getchar();
                break;
            case 2:
                printf("Digite o nome do livro: ");
                flush_stdin();
                fgets(startLivro, sizeof(startLivro), stdin);
                startLivro[strcspn(startLivro, "\n")] = 0;

                printf("Digite o autor do livro: ");
                fgets(autorLivro, sizeof(autorLivro), stdin);
                autorLivro[strcspn(autorLivro, "\n")] = 0;

                inserirLivroFila(filaLivros, startLivro, autorLivro);

                arquivoFila = fopen("fila_de_livros.txt", "a");
                fprintf(arquivoFila, "%s;%s\n", startLivro, autorLivro);
                fclose(arquivoFila);

                printf("O livro foi adicionado na fila.\n");
                
                break;
            case 3:
                printf("Digite o nome do livro: ");
                flush_stdin();
                fgets(startLivro, sizeof(startLivro), stdin);
                startLivro[strcspn(startLivro, "\n")] = 0;

                printf("Digite o autor do livro: ");
                fgets(autorLivro, sizeof(autorLivro), stdin);
                autorLivro[strcspn(autorLivro, "\n")] = 0;

                inserirLivroPilha(pilhaLivros, startLivro, autorLivro);

                arquivoPilha = fopen("pilha_de_livros.txt", "a");
                fprintf(arquivoPilha, "%s;%s\n", startLivro, autorLivro);
                fclose(arquivoPilha);

                printf("O livro foi adicionado na pilha.\n");
                
                break;
            case 4:
                printf("Digite o nome do livro para ser removido da fila: ");
                flush_stdin();
                fgets(startLivro, sizeof(startLivro), stdin);
                startLivro[strcspn(startLivro, "\n")] = 0;
                removerLivroFila(filaLivros, startLivro);

                arquivoFila = fopen("fila_de_livros.txt", "w");
                struct Livro* atual = filaLivros->frente;
                while (atual != NULL) {
                    fprintf(arquivoFila, "%s;%s\n", atual->nome, atual->autor);
                    atual = atual->prox;
                }
                fclose(arquivoFila);
                break;
            case 5:
                printf("Digite o nome do livro a ser removido da pilha: ");
                flush_stdin();
                fgets(startLivro, sizeof(startLivro), stdin);
                startLivro[strcspn(startLivro, "\n")] = 0;
                removerLivroPilha(pilhaLivros, startLivro);

                arquivoPilha = fopen("pilha_de_livros.txt", "w");
                struct Livro* topo = pilhaLivros->topo;
                while (topo != NULL) {
                    fprintf(arquivoPilha, "%s;%s\n", topo->nome, topo->autor);
                    topo = topo->prox;
                }
                fclose(arquivoPilha);

                break;
            case 6:
                return 0;
            default:
                printf("Opção inválida. Escolha as que foram sugeridas\n");
        }
    }
    return 0;
}
