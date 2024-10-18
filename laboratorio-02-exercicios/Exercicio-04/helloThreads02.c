#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 10  // Número de termos da sequência de Fibonacci

// Variáveis globais
int fibonacci[MAX_TERMS]; // Array para armazenar a sequência
int n; // Número de termos a serem gerados

// Função que gera a sequência de Fibonacci
void *gerarFibonacci(void *arg) {
    if (n >= 1) fibonacci[0] = 0; // Primeiro termo
    if (n >= 2) fibonacci[1] = 1; // Segundo termo

    for (int i = 2; i < n; i++) {
        fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2]; // Calcula os termos
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    printf("Digite o número de termos da sequência de Fibonacci (máximo %d): ", MAX_TERMS);
    scanf("%d", &n);

    if (n > MAX_TERMS) {
        printf("Número de termos excede o máximo permitido.\n");
        return 1;
    }

    pthread_t thread;
    
    // Criando a thread para gerar a sequência de Fibonacci
    int status = pthread_create(&thread, NULL, gerarFibonacci, NULL);
    if (status != 0) {
        printf("Erro na criação da thread. Código de erro: %d\n", status);
        return 1;
    }

    // Aguardando a thread terminar
    pthread_join(thread, NULL);

    // Imprimindo a sequência de Fibonacci
    printf("Sequência de Fibonacci com %d termos:\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n");

    return 0;
}
