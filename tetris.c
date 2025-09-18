#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

// Estrutura da peça
typedef struct Peca 
{
    int id;
    char nome;
    struct Peca* prox;
} Peca;

// Estrutura da fila
typedef struct 
{
    Peca* frente;
    Peca* tras;
    int tamanhoAtual;
} Fila;

// Tipos válidos de peças
char tiposValidos[] = {'I', 'O', 'T', 'L'};

// Inicializa a fila
void inicializarFila(Fila* fila) 
{
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanhoAtual = 0;
}

// Gera uma peça automaticamente
Peca* gerarPeca(int id) 
{
    char nome = tiposValidos[rand() % 4];

    Peca* nova = (Peca*) malloc(sizeof(Peca));
    if (!nova) 
    {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    nova->id = id;
    nova->nome = nome;
    nova->prox = NULL;

    printf("Peça gerada automaticamente: [%c %d]\n", nome, id);
    return nova;
}

// Adiciona peça à fila
void enfileirar(Fila* fila, Peca* nova) 
{
    if (fila->tamanhoAtual >= MAX) 
    {
        printf("Fila cheia. Não é possível adicionar mais peças.\n");
        free(nova); // evita vazamento de memória
        return;
    }

    if (fila->tras == NULL) 
    {
        fila->frente = fila->tras = nova;
    } else 
    {
        fila->tras->prox = nova;
        fila->tras = nova;
    }

    fila->tamanhoAtual++;
    printf("Peça [%c %d] adicionada à fila.\n", nova->nome, nova->id);
}

// Remove peça do início da fila
void desenfileirar(Fila* fila) 
{
    if (fila->frente == NULL) 
    {
        printf("Fila vazia. Nenhuma peça para jogar.\n");
        return;
    }

    Peca* temp = fila->frente;
    printf("Jogando peça [%c %d]...\n", temp->nome, temp->id);

    fila->frente = fila->frente->prox;
    if (fila->frente == NULL)
        fila->tras = NULL;

    free(temp);
    fila->tamanhoAtual--;
}

// Exibe o estado atual da fila
void mostrarFila(Fila* fila) 
{
    printf("\nConfira a seguir seu estado:\n\n");
    printf("Fila de peças\n");

    if (fila->frente == NULL) 
    {
        printf("[ Fila vazia ]\n");
        return;
    }

    Peca* atual = fila->frente;
    while (atual != NULL) 
    {
        printf("[%c %d] ", atual->nome, atual->id);
        atual = atual->prox;
    }
    printf("\n");
}

// Função principal
int main() 
{
    Fila fila;
    int idCounter = 0;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);

    // Inicializa com 5 peças
    for (int i = 0; i < MAX; i++) 
    {
        Peca* nova = gerarPeca(idCounter++);
        enfileirar(&fila, nova);
    }

    // Loop principal
    do 
    {
        mostrarFila(&fila);

        printf("\n--- MENU ---\n");
        printf("1. Jogar próxima peça\n");
        printf("2. Adicionar nova peça (gerada automaticamente)\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer do teclado

        switch (opcao) 
        {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                if (fila.tamanhoAtual < MAX) 
                {
                    Peca* nova = gerarPeca(idCounter++);
                    enfileirar(&fila, nova);
                } else {
                    printf("Fila cheia. Jogue uma peça antes de adicionar outra.\n");
                }
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 3);

    // Libera memória restante
    while (fila.frente != NULL)
        desenfileirar(&fila);

    return 0;
}