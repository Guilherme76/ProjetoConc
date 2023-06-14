#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 100000
#define EPSILON 1e-6
#define MAX_ITERACOES 100

double funcao(double x) {
    // Função para a qual queremos encontrar as raízes
    return pow(x, 3) - 2 * pow(x, 2) - 11 * x + 12;
}

double derivada_funcao(double x) {
    // Derivada da função
    return 3 * pow(x, 2) - 4 * x - 11;
}

double metodo_newton(double x) {
    int iteracoes = 0;
    while (fabs(funcao(x)) > EPSILON && iteracoes < MAX_ITERACOES) {
        x = x - funcao(x) / derivada_funcao(x);
        iteracoes++;
    }
    return x;
}

int main() {
    double* vetor = (double*)malloc(TAMANHO * sizeof(double));  // Vetor alocado dinamicamente com 100.000 elementos

    // Preenche o vetor com valores iniciais
    for (int i = 0; i < TAMANHO; i++) {
        vetor[i] = -10.0 + i * 0.2;
    }

    double tempo_inicial = omp_get_wtime();

    // Executa o método de Newton para cada elemento do vetor de forma paralela
    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (int i = 0; i < TAMANHO; i++) {
            double raiz = metodo_newton(vetor[i]);
            printf("Raiz encontrada para o elemento %d: %lf\n", i, raiz);
        }
    }

    double tempo_final = omp_get_wtime();
    double tempo_execucao = tempo_final - tempo_inicial;

    printf("Tempo de execução: %lf segundos\n", tempo_execucao);

    free(vetor);  // Liberar a memória alocada

    return 0;
}