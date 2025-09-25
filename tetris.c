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

char tiposValidos[] = {'I', 'O', 'T', 'L'};

// Inicializações
void inicializarFila(Fila* fila) 
{
    fila -> frente = NULL;
    fila -> tras = NULL;
    fila -> tamanhoAtual = 0;
}

void inicializarPilha(Pilha* pilha) 
{
    pilha->topo = NULL;
    pilha->tamanhoAtual = 0;
}

// Gera peça aleatória
Peca* gerarPeca(int id) 
{
    Peca* nova = (Peca*)malloc(sizeof(Peca));
    if (!nova) 
    {
        printf("Erro de alocação.\n");
        exit(1);
    }
    nova -> id = id;
    nova -> nome = tiposValidos[rand() % 4];
    nova -> prox = NULL;
    printf("Peça gerada: [%c %d] \n", nova -> nome, nova -> id);
    return nova;
}

// Enfileira peça
void enfileirar(Fila* fila, Peca* nova)  
{
    if (fila -> tamanhoAtual >= MAX) 
    {
        printf("Fila cheia.\n");
        free(nova);
        return;
    }
    if (!fila -> frente) 
    {
        fila -> frente = fila -> tras = nova;
    } else 
    {
        fila -> tras -> prox = nova;
        fila -> tras = nova;
    }
    fila -> tamanhoAtual++;
    printf("Peça [%c %d] adicionada à fila. \n", nova -> nome, nova -> id);
}

// Remove peça da fila
Peca* desenfileirar(Fila* fila) 
{
    if (!fila->frente) 
    {
        printf("Fila vazia. \n");
        return NULL;
    }
    Peca* temp = fila -> frente;
    fila -> frente = temp -> prox;
    if (!fila->frente) fila->tras = NULL;
    temp -> prox = NULL;
    fila -> tamanhoAtual--;
    return temp;
}

// Empilha peça
void empilhar(Pilha* pilha, Peca* nova) 
{
    if (pilha->tamanhoAtual >= MAX_PILHA) 
    {
        printf("Pilha cheia.\n");
        free(nova);
        return;
    }
    nova -> prox = pilha -> topo;
    pilha -> topo = nova;
    pilha -> tamanhoAtual++;
    printf("Peça [%c %d] reservada. \n", nova -> nome, nova -> id);
}

// Desempilha
void desempilhar(Pilha* pilha) 
{
    if (!pilha -> topo) 
    {
        printf("Pilha vazia. \n");
        return;
    }
    Peca* temp = pilha -> topo;
    pilha -> topo = temp -> prox;
    pilha -> tamanhoAtual--;
    printf("Usando peça reservada [%c %d] \n", temp -> nome, temp -> id);
    free(temp);
}

// Troca entre frente da fila e topo da pilha
void trocarAtual(Fila* fila, Pilha* pilha) 
{
    if (!fila->frente || !pilha -> topo) 
    {
        printf("Não é possível trocar. Fila ou pilha vazia. \n");
        return;
    }
    Peca* f = fila -> frente;
    Peca* p = pilha -> topo;
    char nomeTemp = f -> nome;
    int idTemp = f -> id;
    f -> nome = p -> nome;
    f -> id = p -> id;
    p -> nome = nomeTemp;
    p -> id = idTemp;
    printf("Peças trocadas (fila <-> pilha). \n");
}

// Troca múltipla
void trocaMultipla(Fila* fila, Pilha* pilha) 
{
    if (fila -> tamanhoAtual < 3 || pilha -> tamanhoAtual < 3) 
    {
        printf("Troca múltipla indisponível (faltam peças). \n");
        return;
    }
    Peca* f = fila -> frente;
    Peca* p = pilha -> topo;
    for (int i = 0; i < 3; i++) 
    {
        char nomeTemp = f -> nome;
        int idTemp = f -> id;
        f -> nome = p -> nome;
        f -> id = p -> id;
        p -> nome = nomeTemp;
        p -> id = idTemp;
        f = f -> prox;
        p = p -> prox;
    }
    printf("Troca múltipla realizada. \n");
}

// Exibe fila e pilha
void mostrarEstado(Fila* fila, Pilha* pilha) 
{
    printf("\n========== ESTADO ATUAL ========== \n");

    printf("Fila de peças        ");
    Peca* atual = fila -> frente;
    while (atual) 
    {
        printf("[%c %d] ", atual -> nome, atual -> id);
        atual = atual -> prox;
    }
    if (!fila -> frente) printf("[ Fila vazia ]");
    printf("\n");

    printf("Pilha de reserva     (Topo -> Base): ");
    atual = pilha -> topo;
    while (atual) 
    {
        printf("[%c %d] ", atual -> nome, atual -> id);
        atual = atual -> prox;
    }
    if (!pilha->topo) printf("[ Pilha vazia ]");
    printf("\n ================================== \n");
}

// Programa principal
int main() 
{
    Fila fila;
    Pilha pilha;
    int opcao, idCounter = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < MAX; i++) 
    {
        enfileirar(&fila, gerarPeca(idCounter++));
    }

    do 
    {
        printf("\n MENU DE AÇÕES \n");
        printf("1 - Jogar peça \n");
        printf("2 - Reservar peça \n");
        printf("3 - Usar peça reservada \n");
        printf("4 - Trocar peça atual (fila <-> pilha) \n");
        printf("5 - Troca múltipla (3 peças) \n");
        printf("6 - Visualizar estado atual \n");
        printf("0 - Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        switch (opcao) 
        {
            case 1: 
            {
                Peca* jogada = desenfileirar(&fila);
                if (jogada) 
                {
                    printf("Jogando peça [%c %d] \n", jogada -> nome, jogada -> id);
                    free(jogada);
                    enfileirar(&fila, gerarPeca(idCounter++));
                }
                break;
            }
            case 2: 
            {
                if (pilha.tamanhoAtual >= MAX_PILHA) 
                {
                    printf("Pilha cheia. \n");
                    break;
                }
                Peca* reservada = desenfileirar(&fila);
                if (reservada) 
                {
                    empilhar(&pilha, reservada);
                    enfileirar(&fila, gerarPeca(idCounter++));
                }
                break;
            }
            case 3:
                desempilhar(&pilha);
                break;
            case 4:
                trocarAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 6:
                mostrarEstado(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    // Libera memória restante
    while (fila.frente) free(desenfileirar(&fila));
    while (pilha.topo) desempilhar(&pilha);

    return 0;
}