#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// Constantes
#define TAM_FILA 5
#define TAM_PILHA 3

// --- Estrutura da Peça ---
typedef struct {
    char nome; 
    int id;    
} Peca;

// --- Estruturas de Dados ---
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int traseiro;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo; 
} Pilha;

// --- Variável Global de ID ---
int proximo_id = 0;

// ---------------------------------------------
//           PROTÓTIPOS DAS FUNÇÕES
// ---------------------------------------------
void inicializarFila(FilaCircular *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca();
int filaEstaVazia(const FilaCircular *f);
int filaEstaCheia(const FilaCircular *f);
void enfileirar(FilaCircular *f, Peca p);
Peca desenfileirar(FilaCircular *f);
Peca peekFila(const FilaCircular *f);
int pilhaEstaVazia(const Pilha *p);
int pilhaEstaCheia(const Pilha *p);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
Peca peekPilha(const Pilha *p);
void jogarPeca(FilaCircular *f);
void reservarPeca(FilaCircular *f, Pilha *p);
void usarPecaReservada(Pilha *p);
void trocarPecaAtual(FilaCircular *f, Pilha *p);
void trocaMultipla(FilaCircular *f, Pilha *p);
void exibirEstadoAtual(const FilaCircular *f, const Pilha *p);
void limparBuffer();

// ---------------------------------------------
//              FUNÇÃO PRINCIPAL (Não Alterada)
// ---------------------------------------------

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL)); 

    FilaCircular fila;
    Pilha pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    printf("--- 🚀 Gerenciador Estratégico de Peças ---\n");
    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }
    printf("Fila inicializada com %d peças.\n", TAM_FILA);

    do {
        exibirEstadoAtual(&fila, &pilha);
        
        printf("\nOpções disponíveis:\n");
        printf("1. 🏃 Jogar peça da frente da fila\n");
        printf("2. 💾 Enviar peça da fila para a pilha de reserva\n");
        printf("3. ⭐ Usar peça da pilha de reserva\n");
        printf("4. 🔄 Trocar peça da frente da fila com o topo da pilha\n");
        printf("5. 🔁 Trocar os %d primeiros da fila com as %d peças da pilha (Troca Múltipla)\n", TAM_PILHA, TAM_PILHA);
        printf("0. 🚪 Sair\n");
        printf("Escolha uma ação (0-%d): ", 5);
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n❌ Entrada inválida. Tente novamente.\n");
            limparBuffer();
            opcao = -1; 
            continue;
        }
        limparBuffer();

        printf("\n------------------------------------------------\n");
        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("👋 Encerrando o programa. Fuga bem-sucedida!\n");
                break;
            default:
                printf("❌ Opção não reconhecida.\n");
        }
        printf("------------------------------------------------\n");

    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------
//           IMPLEMENTAÇÃO DAS FUNÇÕES
// ---------------------------------------------

// --- Inicialização e Geração ---
void inicializarFila(FilaCircular *f) {
    f->frente = -1; f->traseiro = -1; f->tamanho = 0;
}
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'}; // Tipos de Tetromino
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    novaPeca.nome = tipos[rand() % num_tipos];
    novaPeca.id = proximo_id++;
    return novaPeca;
}

// --- Funções de Fila Circular (Mantidas) ---
int filaEstaVazia(const FilaCircular *f) {
    return f->tamanho == 0;
}
int filaEstaCheia(const FilaCircular *f) {
    return f->tamanho == TAM_FILA;
}
void enfileirar(FilaCircular *f, Peca p) {
    if (filaEstaCheia(f)) { return; }
    if (filaEstaVazia(f)) { f->frente = 0; }
    f->traseiro = (f->traseiro + 1) % TAM_FILA;
    f->itens[f->traseiro] = p;
    f->tamanho++;
}
Peca desenfileirar(FilaCircular *f) {
    Peca pecaRemovida = {'\0', -1};
    if (filaEstaVazia(f)) { return pecaRemovida; }
    pecaRemovida = f->itens[f->frente];
    f->tamanho--;
    if (f->tamanho == 0) {
        f->frente = -1; f->traseiro = -1;
    } else {
        f->frente = (f->frente + 1) % TAM_FILA;
    }
    return pecaRemovida;
}
Peca peekFila(const FilaCircular *f) {
    if (filaEstaVazia(f)) { return (Peca){'\0', -1}; }
    return f->itens[f->frente];
}

// --- Funções de Pilha (Mantidas) ---
int pilhaEstaVazia(const Pilha *p) {
    return p->topo == -1;
}
int pilhaEstaCheia(const Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}
void empilhar(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) { return; }
    p->topo++;
    p->itens[p->topo] = peca;
}
Peca desempilhar(Pilha *p) {
    Peca pecaRemovida = {'\0', -1};
    if (pilhaEstaVazia(p)) { return pecaRemovida; }
    pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}
Peca peekPilha(const Pilha *p) {
    if (pilhaEstaVazia(p)) { return (Peca){'\0', -1}; }
    return p->itens[p->topo];
}

// --- Funções de Ações Estratégicas (Jogar, Reservar, Usar) ---
void jogarPeca(FilaCircular *f) {
    if (filaEstaVazia(f)) { printf("Ação: Fila de peças vazia, não há o que jogar.\n"); return; }
    Peca pecaJogada = desenfileirar(f);
    printf("Ação: Peça jogada -> [%c %d].\n", pecaJogada.nome, pecaJogada.id);
    Peca novaPeca = gerarPeca();
    enfileirar(f, novaPeca);
    printf("     -> Nova peça [%c %d] gerada e enfileirada para manter o fluxo.\n", novaPeca.nome, novaPeca.id);
}
void reservarPeca(FilaCircular *f, Pilha *p) {
    if (filaEstaVazia(f)) { printf("Ação: Fila de peças vazia, não há o que reservar.\n"); return; }
    if (pilhaEstaCheia(p)) { printf("Ação: Pilha de reserva cheia (Máx: %d), não é possível reservar.\n", TAM_PILHA); return; }
    Peca pecaReservada = desenfileirar(f);
    empilhar(p, pecaReservada);
    printf("Ação: Peça [%c %d] movida da fila para a reserva (pilha).\n", pecaReservada.nome, pecaReservada.id);
    Peca novaPeca = gerarPeca();
    enfileirar(f, novaPeca);
    printf("     -> Nova peça [%c %d] gerada e enfileirada para manter o fluxo.\n", novaPeca.nome, novaPeca.id);
}
void usarPecaReservada(Pilha *p) {
    if (pilhaEstaVazia(p)) { printf("Ação: Pilha de reserva vazia, não há o que usar.\n"); return; }
    Peca pecaUsada = desempilhar(p);
    printf("Ação: Peça reservada usada -> [%c %d].\n", pecaUsada.nome, pecaUsada.id);
}

// --- Funções de Troca Estratégica (Mantidas) ---
void trocarPecaAtual(FilaCircular *f, Pilha *p) {
    if (filaEstaVazia(f) || pilhaEstaVazia(p)) { 
        printf("Ação: Fila e/ou Pilha vazia. Não é possível trocar.\n"); 
        return; 
    }
    Peca pecaFila = desenfileirar(f); 
    Peca pecaPilha = desempilhar(p);
    enfileirar(f, pecaPilha);
    empilhar(p, pecaFila);
    printf("Ação: Troca pontual realizada!\n");
    printf("     -> Fila recebeu: [%c %d] (antigo topo da pilha).\n", pecaPilha.nome, pecaPilha.id);
    printf("     -> Pilha recebeu: [%c %d] (antiga frente da fila).\n", pecaFila.nome, pecaFila.id);
}

/**
 * @brief Troca as três primeiras peças da fila com as três peças da pilha.
 */
void trocaMultipla(FilaCircular *f, Pilha *p) {
    if (f->tamanho < TAM_PILHA) {
        printf("Ação: ❌ Falha na Troca Múltipla. Fila tem apenas %d peças (requer %d).\n", f->tamanho, TAM_PILHA);
        return;
    }
    if (p->topo < TAM_PILHA - 1) { 
        printf("Ação: ❌ Falha na Troca Múltipla. Pilha tem apenas %d peças (requer %d).\n", p->topo + 1, TAM_PILHA);
        return;
    }
    
    printf("Ação: 🔁 Troca Múltipla iniciada entre os %d primeiros da Fila e a Pilha.\n", TAM_PILHA);
    
    Peca tempFila[TAM_PILHA];
    Peca tempPilha[TAM_PILHA];

    // 1. Remove as 3 peças da Fila (Frente) para o array temporário
    for (int i = 0; i < TAM_PILHA; i++) {
        tempFila[i] = desenfileirar(f);
    }
    
    // 2. Remove as 3 peças da Pilha (Topo -> Base) para o array temporário
    for (int i = 0; i < TAM_PILHA; i++) {
        tempPilha[i] = desempilhar(p);
    }
    
    // 3. Empilha os itens da Fila (do array temporário) na Pilha. 
    //    (Ordem invertida: Frente da Fila (tempFila[0]) vai para a BASE da Pilha)
    for (int i = 0; i < TAM_PILHA; i++) {
        empilhar(p, tempFila[i]);
    }

    // 4. CORREÇÃO CRÍTICA: Enfileira os itens da Pilha (do array temporário) na Fila.
    //    O item que era o TOPO da Pilha (tempPilha[0]) deve ir para a FRENTE da Fila.
    for (int i = 0; i < TAM_PILHA; i++) {
        enfileirar(f, tempPilha[i]);
    }
    
    printf("Ação: ✅ Troca de blocos de %d peças realizada com sucesso!\n", TAM_PILHA);
}


// --- Funções de Utilidade (Mantidas) ---
void exibirEstadoAtual(const FilaCircular *f, const Pilha *p) {
    printf("\n====================== ESTADO ATUAL ======================\n");
    
    // Exibição da Fila
    printf("Fila de Peças (Frente -> Traseiro, Tam: %d/%d): ", f->tamanho, TAM_FILA);
    if (filaEstaVazia(f)) {
        printf("[Vazia]\n");
    } else {
        int i = f->frente;
        int count = 0;
        while (count < f->tamanho) {
            printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
            i = (i + 1) % TAM_FILA;
            count++;
        }
        printf("\n");
    }
    // 

    // Exibição da Pilha
    printf("Pilha de Reserva (Topo -> Base, Tam: %d/%d): ", p->topo + 1, TAM_PILHA);
    if (pilhaEstaVazia(p)) {
        printf("[Vazia]\n");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
        printf("\n");
    }
    // 
    
    printf("==========================================================\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}