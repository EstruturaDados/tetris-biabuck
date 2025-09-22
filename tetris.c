#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5
#define MAX_PILHA 3

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

// Estrutura da pilha
typedef struct 
{
    Peca* topo;
    int tamanhoAtual;
} Pilha;

// Tipos de peças
char tiposValidos[] = {'I', 'O', 'T', 'L'};

void inicializarFila(Fila* fila) 
{
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanhoAtual = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha* pilha)
{
    pilha -> topo = NULL;
    pilha -> tamanhoAtual = 0;
}

// Gera uma peça automaticamente
Peca* gerarPeca(int id) 
{
    char nome = tiposValidos[rand() % 4];

    Peca* nova = (Peca*) malloc(sizeof(Peca));
    if (!nova) 
    {
        printf("Erro de alocação de memória. \n");
        exit(1);
    }

    nova -> id = id;
    nova -> nome = nome;
    nova -> prox = NULL;

    printf("Peça gerada automaticamente: [%c %d] \n", nome, id);
    return nova;
}

// Adiciona peça 
void enfileirar(Fila* fila, Peca* nova) 
{
    if (fila -> tamanhoAtual >= MAX) 
    {
        printf("Fila cheia. Não é possível adicionar mais peças. \n");
        free(nova);
        return;
    }

    if (fila -> tras == NULL) 
    {
        fila -> frente = fila->tras = nova;
    } else 
    {
        fila->tras->prox = nova;
        fila->tras = nova;
    }

    fila -> tamanhoAtual++;
    printf("Peça [%c %d] adicionada à fila.\n", nova -> nome, nova -> id);
}

// Remove peça do início da fila
Peca* desenfileirar(Fila* fila) 
{
    if (fila -> frente == NULL) 
    {
        printf("Fila vazia. Nenhuma peça para jogar. \n");
        return NULL;
    }

    Peca* temp = fila -> frente;
    fila -> frente = fila -> frente->prox;
    if (fila -> frente == NULL)
        fila -> tras = NULL;

    temp -> prox = NULL;
    fila -> tamanhoAtual--;
    return temp;
}

// Adiciona peça no topo
void empilhar(Pilha* pilha, Peca* nova)
{
    if (pilha -> tamanhoAtual >= MAX_PILHA)
    {
        printf("Pilha cheia. Não é possível reservar mais peças.\n");
        free(nova);
        return;
    }

    nova -> prox = pilha->topo;
    pilha -> topo = nova;
    pilha -> tamanhoAtual++;
    printf("Peça [%c %d] reservada (empilhada). \n", nova -> nome, nova -> id);
}

// Remove peça do topo
void desempilhar(Pilha* pilha)
{
    if (pilha -> topo == NULL)
    {
        printf("Pilha vazia. Nenhuma peça reservada para usar. \n");
        return;
    }

    Peca* temp = pilha -> topo;
    pilha -> topo = temp -> prox;
    printf("Usando peça reservada [%c %d]... \n", temp -> nome, temp -> id);
    free(temp);
    pilha->tamanhoAtual--;
}

// Exibe o estado atual da fila e da pilha
void mostrarEstado(Fila* fila, Pilha* pilha)
{
    printf("\n Estado atual: \n \n");

    // Fila
    printf("Fila de peças        ");
    if (fila -> frente == NULL) 
    {
        printf("[ Fila vazia ] \n");
    } else 
    {
        Peca* atual = fila -> frente;
        while (atual != NULL) 
        {
            printf("[%c %d] ", atual->nome, atual -> id);
            atual = atual -> prox;
        }
        printf("\n");
    }

    // Pilha
    printf("Pilha de reserva     (Topo -> Base): ");
    if (pilha -> topo == NULL)
    {
        printf("[ Pilha vazia ]");
    } else
    {
        Peca* atual = pilha -> topo;
        while (atual != NULL)
        {
            printf("[%c %d] ", atual->nome, atual -> id);
            atual = atual -> prox;
        }
    }

    printf("\n");
}

// Função principal
int main() 
{
    Fila fila;
    Pilha pilha;
    int idCounter = 0;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < MAX; i++) 
    {
        Peca* nova = gerarPeca(idCounter++);
        enfileirar(&fila, nova);
    }

    // Loop principal
    do 
    {
        mostrarEstado(&fila, &pilha);

        printf("\n Opções de Ação: \n \n");
        printf("Código     Ação \n");
        printf("1          Jogar peça \n");
        printf("2          Reservar peça \n");
        printf("3          Usar peça reservada \n");
        printf("0          Sair \n");
        printf("\n Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        switch (opcao) 
        {
            case 1: 
            {
                Peca* jogada = desenfileirar(&fila);
                if (jogada)
                {
                    printf("Jogando peça [%c %d]... \n", jogada -> nome, jogada -> id);
                    free(jogada);
                    Peca* nova = gerarPeca(idCounter++);
                    enfileirar(&fila, nova);
                }
                break;
            }

            case 2:
            {
                if (pilha.tamanhoAtual >= MAX_PILHA)
                {
                    printf("Não é possível reservar mais peças. \n");
                    break;
                }

                Peca* reservada = desenfileirar(&fila);
                if (reservada)
                {
                    empilhar(&pilha, reservada);
                    Peca* nova = gerarPeca(idCounter++);
                    enfileirar(&fila, nova);
                }
                break;
            }

            case 3:
                desempilhar(&pilha);
                break;

            case 0:
                printf("Saindo... \n");
                break;

            default:
                printf("Opção inválida. Tente novamente. \n");
        }

    } while (opcao != 0);

    // Libera memória restante
    while (fila.frente != NULL)
    {
        Peca* temp = desenfileirar(&fila);
        if (temp) free(temp);
    }

    while (pilha.topo != NULL)
        desempilhar(&pilha);

    return 0;
}