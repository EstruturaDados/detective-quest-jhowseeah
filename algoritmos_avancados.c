#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 20

// ===================== CÓDIGOS DE COR =====================
#define RESET "\033[0m"
#define NEGRITO "\033[1m"
#define VERMELHO "\033[31m"
#define VERDE "\033[32m"
#define AMARELO "\033[33m"
#define AZUL "\033[34m"
#define MAGENTA "\033[35m"
#define CIANO "\033[36m"
#define CINZA "\033[90m"

// ===================== ESTRUTURAS =====================
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct Pista {
    char descricao[100];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

typedef struct ItemHash {
    char pista[100];
    char suspeito[50];
    struct ItemHash *prox;
} ItemHash;

ItemHash *tabelaHash[TAM_HASH];

// ===================== FUNÇÕES AUXILIARES =====================
unsigned int hash(char *str) {
    unsigned int valor = 0;
    for (int i = 0; str[i] != '\0'; i++)
        valor += str[i];
    return valor % TAM_HASH;
}

Sala *criarSala(char *nome) {
    Sala *nova = (Sala *)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = nova->dir = NULL;
    return nova;
}

Pista *inserirPista(Pista *raiz, char *descricao) {
    if (raiz == NULL) {
        Pista *nova = (Pista *)malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esq = inserirPista(raiz->esq, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->dir = inserirPista(raiz->dir, descricao);
    return raiz;
}

void exibirPistas(Pista *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("  - %s\n", raiz->descricao);
        exibirPistas(raiz->dir);
    }
}

void inserirNaHash(char *pista, char *suspeito) {
    unsigned int indice = hash(pista);
    ItemHash *novo = (ItemHash *)malloc(sizeof(ItemHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

char *encontrarSuspeito(char *pista) {
    unsigned int indice = hash(pista);
    ItemHash *atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}

int contarPistasPorSuspeito(Pista *raiz, char *suspeito) {
    if (raiz == NULL)
        return 0;
    int cont = 0;
    if (strcmp(encontrarSuspeito(raiz->descricao), suspeito) == 0)
        cont = 1;
    return cont + contarPistasPorSuspeito(raiz->esq, suspeito) +
           contarPistasPorSuspeito(raiz->dir, suspeito);
}

// ===================== PISTAS POR SALA =====================
char *obterPistaPorSala(char *sala) {
    if (strcmp(sala, "Biblioteca") == 0)
        return "Livro antigo com páginas rasgadas";
    if (strcmp(sala, "Cozinha") == 0)
        return "Faca suja de sangue";
    if (strcmp(sala, "Sala de Estar") == 0)
        return "Pegada de sapato masculino";
    if (strcmp(sala, "Escritorio") == 0)
        return "Copo com batom";
    if (strcmp(sala, "Porão") == 0)
        return "Luvas escondidas";
    if (strcmp(sala, "Jardim") == 0)
        return "Chave caída entre as flores";
    if (strcmp(sala, "Garagem") == 0)
        return "Pedaço de tecido rasgado no carro";
    if (strcmp(sala, "Quarto Principal") == 0)
        return "Diário com anotações suspeitas";
    if (strcmp(sala, "Banheiro") == 0)
        return "Perfume raro derramado";
    if (strcmp(sala, "Sótão") == 0)
        return "Retrato antigo com alguém riscado";
    return "Nenhuma pista encontrada";
}

// ===================== VISUALIZAÇÃO =====================
void exibirMapaMansao() {
    printf(CIANO "\n==================== MAPA DA MANSÃO ====================\n" RESET);
    printf(AZUL "                   [Biblioteca]\n");
    printf(AZUL "                   /         \\\n");
    printf(AZUL "          [Cozinha]          [Sala de Estar]\n");
    printf(AZUL "          /       \\              /        \\\n");
    printf(AZUL "   [Jardim]     [Garagem]   [Escritorio]  [Porão]\n");
    printf(AZUL "                               /        \\\n");
    printf(AZUL "                    [Quarto Principal]  [Banheiro]\n");
    printf(AZUL "                              \\\n");
    printf(AZUL "                            [Sótão]\n");
    printf(CIANO "========================================================\n" RESET);
}

// ===================== EXPLORAÇÃO =====================
void explorarSalas(Sala *atual, Pista **raizPistas) {
    if (atual == NULL)
        return;

    printf("\n%sVocê entrou na sala:%s %s%s%s\n", NEGRITO, RESET, AMARELO, atual->nome, RESET);
    char *pista = obterPistaPorSala(atual->nome);

    if (strcmp(pista, "Nenhuma pista encontrada") != 0) {
        printf("%sVocê encontrou uma pista:%s %s%s%s\n", VERDE, RESET, MAGENTA, pista, RESET);
        *raizPistas = inserirPista(*raizPistas, pista);
    } else {
        printf("%sNenhuma pista foi encontrada aqui.%s\n", CINZA, RESET);
    }

    char opcao;
    printf("\nEscolha seu próximo destino:\n");
    if (atual->esq)
        printf("  (e) Ir para %s%s%s\n", AMARELO, atual->esq->nome, RESET);
    if (atual->dir)
        printf("  (d) Ir para %s%s%s\n", AMARELO, atual->dir->nome, RESET);
    printf("  (s) %sSair da mansão%s\n", VERMELHO, RESET);
    printf("Opção: ");
    scanf(" %c", &opcao);
    opcao = tolower(opcao);

    if (opcao == 'e' && atual->esq)
        explorarSalas(atual->esq, raizPistas);
    else if (opcao == 'd' && atual->dir)
        explorarSalas(atual->dir, raizPistas);
    else if (opcao == 's')
        printf("\n%sVocê decidiu encerrar a investigação.%s\n", CINZA, RESET);
    else
        printf("\n%sOpção inválida!%s\n", VERMELHO, RESET);
}

// ===================== JULGAMENTO =====================
void mostrarSuspeitos() {
    printf("\n%s👤 Suspeitos disponíveis:%s\n", CIANO, RESET);
    printf("  1. Sra. Helena\n");
    printf("  2. Sr. Moreira\n");
    printf("  3. Chef Ricardo\n");
    printf("  4. Jardineiro Paulo\n");
    printf("  5. Motorista Carlos\n");
}

void verificarSuspeitoFinal(Pista *raizPistas) {
    char acusado[50];
    printf("\n%s================== FASE FINAL ==================%s\n", NEGRITO, RESET);
    printf("%sPistas coletadas:%s\n", CIANO, RESET);
    exibirPistas(raizPistas);

    mostrarSuspeitos();
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistasPorSuspeito(raizPistas, acusado);
    printf("\n========================================================\n");
    if (qtd >= 2)
        printf("%s✅ A acusação procede!%s %s é o culpado (%d pistas o incriminam).\n",
               VERDE, RESET, acusado, qtd);
    else
        printf("%s❌ Acusação inconsistente.%s %s tem apenas %d pista(s) contra ele.\n",
               VERMELHO, RESET, acusado, qtd);
    printf("========================================================\n");
}

// ===================== MAIN =====================
int main() {
    // Inicializa hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // Associa pistas a suspeitos
    inserirNaHash("Livro antigo com páginas rasgadas", "Sr. Moreira");
    inserirNaHash("Faca suja de sangue", "Chef Ricardo");
    inserirNaHash("Pegada de sapato masculino", "Sr. Moreira");
    inserirNaHash("Copo com batom", "Sra. Helena");
    inserirNaHash("Luvas escondidas", "Sra. Helena");
    inserirNaHash("Chave caída entre as flores", "Jardineiro Paulo");
    inserirNaHash("Pedaço de tecido rasgado no carro", "Motorista Carlos");
    inserirNaHash("Diário com anotações suspeitas", "Sra. Helena");
    inserirNaHash("Perfume raro derramado", "Sra. Helena");
    inserirNaHash("Retrato antigo com alguém riscado", "Sr. Moreira");

    // Monta a mansão maior
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *jardim = criarSala("Jardim");
    Sala *garagem = criarSala("Garagem");
    Sala *escritorio = criarSala("Escritorio");
    Sala *porao = criarSala("Porão");
    Sala *quarto = criarSala("Quarto Principal");
    Sala *banheiro = criarSala("Banheiro");
    Sala *sotao = criarSala("Sótão");

    biblioteca->esq = cozinha;
    biblioteca->dir = salaEstar;
    cozinha->esq = jardim;
    cozinha->dir = garagem;
    salaEstar->esq = escritorio;
    salaEstar->dir = porao;
    escritorio->esq = quarto;
    escritorio->dir = banheiro;
    banheiro->dir = sotao;

    Pista *raizPistas = NULL;

    printf(NEGRITO "\n========== Detective Quest: Mansão Misteriosa ==========\n" RESET);
    printf("Bem-vindo, detetive! Sua missão é descobrir quem é o culpado.\n");
    printf("Explore os cômodos, colete pistas e faça sua acusação final.\n");

    exibirMapaMansao();
    explorarSalas(biblioteca, &raizPistas);
    verificarSuspeitoFinal(raizPistas);

    printf("\n%sJogo encerrado. Obrigado por jogar!%s\n\n", CIANO, RESET);
    return 0;
}
