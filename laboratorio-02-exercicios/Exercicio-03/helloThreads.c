#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

// Variável global que define o número de threads
int numberOfThreads = 5;

// Função que as threads vão usar para dar um olá
void *printHello(void *tid) {
    printf("Olá, eu sou a thread %d\n", (int)(size_t) tid);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    pthread_t threads[numberOfThreads];
    
    for (int i = 0; i < numberOfThreads; i++) {
        // Criando uma thread e passando para ela a função printHello
        int status = pthread_create(&threads[i], NULL, printHello, (void *)(size_t) i);

        // Se o status não for 0, houve um erro na criação da thread
        if (status != 0) {
            printf("Erro na criação da thread %d. Código de erro %d.\n", i, status);
            return 1;
        }
    }
    
    // Faz a thread principal dormir por um tempo para garantir que as threads tenham tempo de concluir
    sleep(1);

    return 0;
}
