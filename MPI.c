#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
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

int main(int argc, char** argv) {
    int num_processos, rank_processo;
    double* vetor;
    double* resultados;
    double tempo_inicial, tempo_final;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_processo);

    if (rank_processo == 0) {
        vetor = (double*)malloc(TAMANHO * sizeof(double));
        resultados = (double*)malloc(TAMANHO * sizeof(double));

        // Preenche o vetor com valores iniciais
        for (int i = 0; i < TAMANHO; i++) {
            vetor[i] = -10.0 + i * 0.2;
        }

        tempo_inicial = MPI_Wtime();
    }

    int tamanho_local = TAMANHO / num_processos;
    double* vetor_local = (double*)malloc(tamanho_local * sizeof(double));
    double* resultados_local = (double*)malloc(tamanho_local * sizeof(double));

    MPI_Scatter(vetor, tamanho_local, MPI_DOUBLE, vetor_local, tamanho_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Executa o método de Newton para cada elemento local do vetor de forma paralela
    for (int i = 0; i < tamanho_local; i++) {
        resultados_local[i] = metodo_newton(vetor_local[i]);
    }

    MPI_Gather(resultados_local, tamanho_local, MPI_DOUBLE, resultados, tamanho_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank_processo == 0) {
        tempo_final = MPI_Wtime();
        double tempo_execucao = tempo_final - tempo_inicial;

        // Imprime os resultados
        for (int i = 0; i < TAMANHO; i++) {
            printf("Raiz encontrada para o elemento %d: %lf\n", i, resultados[i]);
        }

        printf("Tempo de execução: %lf segundos\n", tempo_execucao);

        free(vetor);
        free(resultados);
    }

    free(vetor_local);
    free(resultados_local);

    MPI_Finalize();

    return 0;
}
