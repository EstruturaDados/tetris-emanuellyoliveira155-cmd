#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para a geração de peças aleatórias

// Constantes
#define TAM_FILA 5
#define TAM_PILHA 3

// --- Estrutura da Peça ---
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// --- Estruturas de Dados ---

// Fila Circular de Peças
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int traseiro;
    int tamanho;
} FilaCircular;

// Pilha de Reserva de Peças
typedef struct {
    Peca itens[TAM_PILHA];
    int topo; // Indica a posição do último elemento
} Pilha;

// --- Variável Global de ID (Para garantir IDs únicos) ---
int proximo_id = 0;

// ---------------------------------------------
//           PROTÓTIPOS DAS FUNÇÕES
// ---------------------------------------------

// Inicialização
void inicializarFila(FilaCircular *f);
void inicializarPilha(Pilha *p);

// Peça
Peca gerarPeca();

// Fila Circular (Enqueue, Dequeue, Auxiliares)
int filaEstaVazia(const FilaCircular *f);
int filaEstaCheia(const FilaCircular *f);
void enfileirar(FilaCircular *f, Peca p);
Peca desenfileirar(FilaCircular *f);
Peca peekFila(const FilaCircular *f); // Retorna a peça da frente sem remover

// Pilha (Push, Pop, Auxiliares)
int pilhaEstaVazia(const Pilha *p);
int pilhaEstaCheia(const Pilha *p);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
Peca peekPilha(const Pilha *p); // Retorna a peça do topo sem remover

// Ações Estratégicas
void jogarPeca(FilaCircular *f);
void reservarPeca(FilaCircular *f, Pilha *p);
void usarPecaReservada(Pilha *p);
void trocarPecaAtual(FilaCircular *f, Pilha *p);
void trocaMultipla(FilaCircular *f, Pilha *p);

// Utilidades
void exibirEstadoAtual(const FilaCircular *f, const Pilha *p);
void limparBuffer();

// ---------------------------------------------
//              FUNÇÃO PRINCIPAL
// ---------------------------------------------

int main() {
    // Inicializa o gerador de números aleatórios (para gerarPeca)
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
            opcao = -1; // Garante que o loop continue
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

/**
 * @brief Inicializa a Fila Circular.
 * @param f Ponteiro para a estrutura FilaCircular.
 */
void inicializarFila(FilaCircular *f) {
    f->frente = -1;
    f->traseiro = -1;
    f->tamanho = 0;
}

/**
 * @brief Inicializa a Pilha.
 * @param p Ponteiro para a estrutura Pilha.
 */
void inicializarPilha(Pilha *p) {
    p->topo = -1; // Pilha vazia
}

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * @return A peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'L', 'S', 'Z'}; // Mais tipos para variedade
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Gera um tipo de peça aleatório
    novaPeca.nome = tipos[rand() % num_tipos];
    // Atribui o ID único e o incrementa
    novaPeca.id = proximo_id++;
    
    return novaPeca;
}

// --- Funções de Fila Circular ---

/**
 * @brief Verifica se a fila está vazia.
 */
int filaEstaVazia(const FilaCircular *f) {
    return f->tamanho == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 */
int filaEstaCheia(const FilaCircular *f) {
    return f->tamanho == TAM_FILA;
}

/**
 * @brief Adiciona uma peça ao final da fila (Traseiro).
 */
void enfileirar(FilaCircular *f, Peca p) {
    if (filaEstaCheia(f)) {
        printf("⚠️ Fila cheia. Não é possível enfileirar.\n");
        return;
    }
    
    if (filaEstaVazia(f)) {
        f->frente = 0;
    }
    
    // Move o traseiro para a próxima posição circular
    f->traseiro = (f->traseiro + 1) % TAM_FILA;
    f->itens[f->traseiro] = p;
    f->tamanho++;
}

/**
 * @brief Remove e retorna a peça da frente da fila (Frente).
 * @return A peça removida.
 */
Peca desenfileirar(FilaCircular *f) {
    Peca pecaRemovida = {'\0', -1}; // Peça nula em caso de erro
    
    if (filaEstaVazia(f)) {
        printf("❌ Fila vazia. Não é possível desenfileirar.\n");
        return pecaRemovida;
    }
    
    pecaRemovida = f->itens[f->frente];
    f->tamanho--;
    
    if (f->tamanho == 0) {
        // Se a fila esvaziou, reinicia os ponteiros
        f->frente = -1;
        f->traseiro = -1;
    } else {
        // Move o frente para a próxima posição circular
        f->frente = (f->frente + 1) % TAM_FILA;
    }
    
    return pecaRemovida;
}

/**
 * @brief Retorna a peça da frente da fila sem removê-la.
 */
Peca peekFila(const FilaCircular *f) {
    if (filaEstaVazia(f)) {
        return (Peca){'\0', -1};
    }
    return f->itens[f->frente];
}

// --- Funções de Pilha ---

/**
 * @brief Verifica se a pilha está vazia.
 */
int pilhaEstaVazia(const Pilha *p) {
    return p->topo == -1;
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int pilhaEstaCheia(const Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

/**
 * @brief Adiciona uma peça ao topo da pilha.
 */
void empilhar(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) {
        printf("❌ Pilha cheia (Máx: %d). Não é possível empilhar.\n", TAM_PILHA);
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

/**
 * @brief Remove e retorna a peça do topo da pilha.
 * @return A peça removida.
 */
Peca desempilhar(Pilha *p) {
    Peca pecaRemovida = {'\0', -1}; // Peça nula em caso de erro
    if (pilhaEstaVazia(p)) {
        printf("❌ Pilha vazia. Não é possível desempilhar.\n");
        return pecaRemovida;
    }
    pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}

/**
 * @brief Retorna a peça do topo da pilha sem removê-la.
 */
Peca peekPilha(const Pilha *p) {
    if (pilhaEstaVazia(p)) {
        return (Peca){'\0', -1};
    }
    return p->itens[p->topo];
}

// --- Funções de Ações Estratégicas ---

/**
 * @brief Joga a peça da frente da fila e gera uma nova para repor.
 */
void jogarPeca(FilaCircular *f) {
    if (filaEstaVazia(f)) {
        printf("Ação: Fila de peças vazia, não há o que jogar.\n");
        return;
    }
    
    Peca pecaJogada = desenfileirar(f);
    printf("Ação: Peça jogada -> [%c %d].\n", pecaJogada.nome, pecaJogada.id);
    
    // Reposição automática: Adiciona uma nova peça à fila
    Peca novaPeca = gerarPeca();
    enfileirar(f, novaPeca);
    printf("     -> Nova peça [%c %d] gerada e enfileirada para manter o fluxo.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Move a peça da frente da fila para a pilha de reserva.
 */
void reservarPeca(FilaCircular *f, Pilha *p) {
    if (filaEstaVazia(f)) {
        printf("Ação: Fila de peças vazia, não há o que reservar.\n");
        return;
    }
    if (pilhaEstaCheia(p)) {
        printf("Ação: Pilha de reserva cheia (Máx: %d), não é possível reservar.\n", TAM_PILHA);
        return;
    }
    
    // 1. Remove da fila
    Peca pecaReservada = desenfileirar(f);
    
    // 2. Adiciona à pilha
    empilhar(p, pecaReservada);
    printf("Ação: Peça [%c %d] movida da fila para a reserva (pilha).\n", pecaReservada.nome, pecaReservada.id);

    // 3. Reposição automática: Adiciona uma nova peça à fila
    Peca novaPeca = gerarPeca();
    enfileirar(f, novaPeca);
    printf("     -> Nova peça [%c %d] gerada e enfileirada para manter o fluxo.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Remove a peça do topo da pilha, simulando seu uso.
 */
void usarPecaReservada(Pilha *p) {
    if (pilhaEstaVazia(p)) {
        printf("Ação: Pilha de reserva vazia, não há o que usar.\n");
        return;
    }
    
    Peca pecaUsada = desempilhar(p);
    printf("Ação: Peça reservada usada -> [%c %d].\n", pecaUsada.nome, pecaUsada.id);
}

/**
 * @brief Substitui a peça da frente da fila pela peça do topo da pilha.
 */
void trocarPecaAtual(FilaCircular *f, Pilha *p) {
    if (filaEstaVazia(f)) {
        printf("Ação: Fila vazia. Não é possível trocar.\n");
        return;
    }
    if (pilhaEstaVazia(p)) {
        printf("Ação: Pilha vazia. Não é possível trocar.\n");
        return;
    }
    
    // 1. Pega as peças sem remover
    Peca pecaFila = peekFila(f);
    Peca pecaPilha = peekPilha(p);

    // 2. Remove de ambas as estruturas
    desenfileirar(f); 
    desempilhar(p);
    
    // 3. Enfileira a peça da Pilha
    enfileirar(f, pecaPilha);
    
    // 4. Empilha a peça da Fila (Reposicionamento estratégico: A peça removida da fila vai para o topo da pilha)
    empilhar(p, pecaFila);
    
    printf("Ação: Troca pontual realizada!\n");
    printf("     -> Fila recebeu: [%c %d] (antigo topo da pilha).\n", pecaPilha.nome, pecaPilha.id);
    printf("     -> Pilha recebeu: [%c %d] (antiga frente da fila).\n", pecaFila.nome, pecaFila.id);
}

/**
 * @brief Troca as três primeiras peças da fila com as três peças da pilha.
 */
void trocaMultipla(FilaCircular *f, Pilha *p) {
    // Requisito: ambas devem ter pelo menos 3 peças (tamanho da pilha)
    if (f->tamanho < TAM_PILHA) {
        printf("Ação: ❌ Falha na Troca Múltipla. Fila tem apenas %d peças (requer %d).\n", f->tamanho, TAM_PILHA);
        return;
    }
    if (p->topo < TAM_PILHA - 1) { // -1 pois 'topo' é índice
        printf("Ação: ❌ Falha na Troca Múltipla. Pilha tem apenas %d peças (requer %d).\n", p->topo + 1, TAM_PILHA);
        return;
    }
    
    printf("Ação: 🔁 Troca Múltipla iniciada entre os %d primeiros da Fila e a Pilha.\n", TAM_PILHA);
    
    // As peças na Fila Cicular estão em posições não contíguas (circularidade),
    // mas a Pilha tem seus itens em posições contíguas (0 a topo).
    // O mais simples é usar um array temporário.
    
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
    //    Obs: A ordem é invertida para que o primeiro item da fila (índice 0) fique na base da pilha.
    for (int i = 0; i < TAM_PILHA; i++) {
        empilhar(p, tempFila[i]);
    }

    // 4. Enfileira os itens da Pilha (do array temporário) na Fila.
    //    Obs: A ordem é mantida para que o item que era do topo da pilha (índice 0 de tempPilha) vá para a frente da fila.
    for (int i = TAM_PILHA - 1; i >= 0; i--) { // Enfileirar na ordem inversa para preservar a ordem LIFO da pilha
        enfileirar(f, tempPilha[i]);
    }
    
    printf("Ação: ✅ Troca de blocos de %d peças realizada com sucesso!\n", TAM_PILHA);
    
    // Repõe a fila (pode não ser necessário se a fila não perdeu tamanho, mas garante a regra de 5)
    // Se a fila estava cheia antes e perdeu 3, ela recebeu 3 de volta, então está cheia.
    // Nenhuma reposição é necessária aqui, pois o tamanho da fila (5) e da pilha (3) foi mantido.
}

// --- Funções de Utilidade ---

/**
 * @brief Exibe o estado atual da Fila e da Pilha de forma formatada.
 */
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

    // Exibição da Pilha
    printf("Pilha de Reserva (Topo -> Base, Tam: %d/%d): ", p->topo + 1, TAM_PILHA);
    if (pilhaEstaVazia(p)) {
        printf("[Vazia]\n");
    } else {
        for (int i = p