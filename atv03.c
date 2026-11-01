/* Programa para ordenar numeros e manipular arquivos em C */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define MAX 1000
#define ARQ_DADOS "dados.txt"
#define ARQ_ORDENADO "ordenado.txt"
 
void bubbleSort(int v[], int n) {
    int i, j, aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}
 
void insertionSort(int v[], int n) {
    int i, j, chave;
    for (i = 1; i < n; i++) {
        chave = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}
 
void selectionSort(int v[], int n) {
    int i, j, menor, aux;
    for (i = 0; i < n - 1; i++) {
        menor = i;
        for (j = i + 1; j < n; j++) {
            if (v[j] < v[menor]) menor = j;
        }
        if (menor != i) {
            aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
}


void merge(int v[], int ini, int meio, int fim) {
    int tam = fim - ini + 1;
    int *temp = (int *) malloc(tam * sizeof(int));
    int i = ini, j = meio + 1, k = 0;
 
    if (temp == NULL) {
        printf("Erro de memoria!\n");
        return;
    }
    while (i <= meio && j <= fim) {
        if (v[i] <= v[j]) temp[k++] = v[i++];
        else              temp[k++] = v[j++];
    }
    while (i <= meio) temp[k++] = v[i++];
    while (j <= fim)  temp[k++] = v[j++];
 
    for (k = 0; k < tam; k++) v[ini + k] = temp[k];
    free(temp);
}
 
void mergeSort(int v[], int ini, int fim) {
    if (ini < fim) {
        int meio = (ini + fim) / 2;
        mergeSort(v, ini, meio);
        mergeSort(v, meio + 1, fim);
        merge(v, ini, meio, fim);
    }
}
 
int particiona(int v[], int ini, int fim) {
    int pivo = v[fim], i = ini - 1, j, aux;
    for (j = ini; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            aux = v[i]; v[i] = v[j]; v[j] = aux;
        }
    }
    aux = v[i + 1]; v[i + 1] = v[fim]; v[fim] = aux;
    return i + 1;
}
 
void quickSort(int v[], int ini, int fim) {
    if (ini < fim) {
        int p = particiona(v, ini, fim);
        quickSort(v, ini, p - 1);
        quickSort(v, p + 1, fim);
    }
}
 
// Manipulacao de arquivos
 
// Grava n valores no arquivo (um por linha). Retorna 1 se ok, 0 se erro. 

int salvarArquivo(const char *nome, int v[], int n) {
    int i;
    FILE *f = fopen(nome, "w");
    if (f == NULL) {
        printf("Erro ao abrir '%s' para escrita!\n", nome);
        return 0;
    }
    for (i = 0; i < n; i++) fprintf(f, "%d\n", v[i]);
    fclose(f);
    return 1;
}
 
// Le valores do arquivo para o vetor. Retorna a quantidade lida (-1 se erro).
int lerArquivo(const char *nome, int v[]) {
    int n = 0;
    FILE *f = fopen(nome, "r");
    if (f == NULL) {
        printf("Erro ao abrir '%s'. Ele ja foi criado? (opcao 1)\n", nome);
        return -1;
    }
    while (n < MAX && fscanf(f, "%d", &v[n]) == 1) n++;
    fclose(f);
    return n;
}
 
// Funcoes auxiliares
 
void mostrarVetor(const char *titulo, int v[], int n) {
    int i;
    printf("%s (%d valores):\n", titulo, n);
    for (i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}
 
//  Pergunta o algoritmo e ordena o vetor. Retorna 1 se ordenou, 0 se opcao invalida.

int escolherEOrdenar(int v[], int n) {
    int op;
    printf("\nEscolha o algoritmo:\n");
    printf("1 - Bubble Sort\n");
    printf("2 - Insertion Sort\n");
    printf("3 - Selection Sort\n");
    printf("4 - Merge Sort\n");
    printf("5 - Quick Sort\n");
    printf("Opcao: ");
    scanf("%d", &op);
 
    switch (op) {
        case 1: bubbleSort(v, n);            break;
        case 2: insertionSort(v, n);         break;
        case 3: selectionSort(v, n);         break;
        case 4: mergeSort(v, 0, n - 1);      break;
        case 5: quickSort(v, 0, n - 1);      break;
        default:
            printf("Opcao invalida!\n");
            return 0;
    }
    return 1;
}
 
// Programa principal
 
int main(void) {
    int dados[MAX], ordenado[MAX];
    int n, i, op, pos, novoValor;
 
    srand((unsigned) time(NULL));
 
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Criar arquivo com dados desordenados\n");
        printf("2 - Ler e mostrar dados do arquivo\n");
        printf("3 - Ordenar dados e salvar em novo arquivo\n");
        printf("4 - Alterar um valor do arquivo e reordenar\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
 
        switch (op) {
            case 1:
                printf("Quantos valores deseja gerar (1 a %d)? ", MAX);
                scanf("%d", &n);
                if (n < 1 || n > MAX) {
                    printf("Quantidade invalida!\n");
                    break;
                }
                for (i = 0; i < n; i++) dados[i] = rand() % 1000; /* 0 a 999 */
                if (salvarArquivo(ARQ_DADOS, dados, n))
                    printf("Arquivo '%s' criado com %d valores.\n", ARQ_DADOS, n);
                break;
 
            case 2:
                n = lerArquivo(ARQ_DADOS, dados);
                if (n > 0) mostrarVetor("Dados lidos", dados, n);
                break;
 
            case 3:
                n = lerArquivo(ARQ_DADOS, dados);
                if (n <= 0) break;
                for (i = 0; i < n; i++) ordenado[i] = dados[i]; /* preserva o original */
                if (escolherEOrdenar(ordenado, n)) {
                    mostrarVetor("Dados ordenados", ordenado, n);
                    if (salvarArquivo(ARQ_ORDENADO, ordenado, n))
                        printf("Resultado salvo em '%s'.\n", ARQ_ORDENADO);
                }
                break;
 
            case 4:
                n = lerArquivo(ARQ_DADOS, dados);
                if (n <= 0) break;
                mostrarVetor("Dados atuais", dados, n);
                printf("Posicao a alterar (1 a %d): ", n);
                scanf("%d", &pos);
                if (pos < 1 || pos > n) {
                    printf("Posicao invalida!\n");
                    break;
                }
                printf("Novo valor para a posicao %d (atual: %d): ", pos, dados[pos - 1]);
                scanf("%d", &novoValor);
                dados[pos - 1] = novoValor;
                if (!salvarArquivo(ARQ_DADOS, dados, n)) break;
                printf("Arquivo '%s' atualizado.\n", ARQ_DADOS);
 
                for (i = 0; i < n; i++) ordenado[i] = dados[i];
                if (escolherEOrdenar(ordenado, n)) {
                    mostrarVetor("Dados reordenados", ordenado, n);
                    if (salvarArquivo(ARQ_ORDENADO, ordenado, n))
                        printf("Resultado salvo em '%s'.\n", ARQ_ORDENADO);
                }
                break;
 
            case 0:
                printf("Encerrando...\n");
                break;
 
            default:
                printf("Opcao invalida!\n");
        }
    } while (op != 0);
 
    return 0;
}