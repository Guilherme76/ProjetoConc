#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TAMANHO 100000
#define EPSILON 1e-6
#define MAX_ITERACOES 100

double f(double x) {
    // Função para a qual queremos encontrar as raízes
    return pow(x, 3) - 2 * pow(x, 2) - 11 * x + 12;
}

double f_deriv(double x) {
    // Derivada da função f(x)
    return 3 * pow(x, 2) - 4 * x - 11;
}

double metodo_newton(double x) {
    int iteracoes = 0;
    while (fabs(f(x)) > EPSILON && iteracoes < MAX_ITERACOES) {
        x = x - f(x) / f_deriv(x);
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

    clock_t tempo_inicial = clock();

    // Executa o método de Newton para cada elemento do vetor
    for (int i = 0; i < TAMANHO; i++) {
        double raiz = metodo_newton(vetor[i]);
        printf("Raiz encontrada para o elemento %d: %lf\n", i, raiz);
    }

    clock_t tempo_final = clock();
    double tempo_execucao = (double)(tempo_final - tempo_inicial) / CLOCKS_PER_SEC;

    printf("Tempo de execução: %lf segundos\n", tempo_execucao);

    free(vetor);  // Libera a memória alocada

    return 0;
}
