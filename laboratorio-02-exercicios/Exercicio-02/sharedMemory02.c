#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

// Variável global para o número de elementos da sequência de Fibonacci
int n = 20;

void generate_fibonacci(int *shared_memory) {
    // Geração da sequência de Fibonacci
    shared_memory[0] = 0;
    if (n > 1) {
        shared_memory[1] = 1;
        for (int i = 2; i < n; i++) {
            shared_memory[i] = shared_memory[i - 1] + shared_memory[i - 2];
        }
    }
}

int main(int argc, char **argv) {
    // Criando o segmento de memoria compartilhada com o tamanho suficiente para armazenar n inteiros
    int segment_id = shmget(IPC_PRIVATE, n * sizeof(int), S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        printf("Erro ao criar memória compartilhada.\n");
        return 1;
    }

    // Criando o ponteiro que vai apontar para o endereço de memória compartilhada
    int *shared_memory = (int *)shmat(segment_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        printf("Erro ao anexar memória compartilhada.\n");
        return 1;
    }

    pid_t pid = fork(); // Função usada para criar um novo processo
    if (pid < 0) { // Erro na criação do processo
        printf("Sou o processo %d e não consegui criar um novo processo.\n", getpid());
        return 1;
    } else if (pid == 0) { // Processo filho
        // Gerando a sequência de Fibonacci
        generate_fibonacci(shared_memory);

        // Desanexando a memória compartilhada no processo filho
        shmdt(shared_memory);

        // Encerrando o processo filho
        exit(0);
    } else { // Processo pai
        // Fazendo o pai esperar que o filho termine de gerar a sequência
        wait(NULL);

        // Imprimindo a sequência de Fibonacci gerada pelo processo filho
        printf("Sequência de Fibonacci com %d elementos:\n", n);
        for (int i = 0; i < n; i++) {
            printf("%d ", shared_memory[i]);
        }
        printf("\n");

        // Desanexando e removendo o segmento de memória compartilhada
        shmdt(shared_memory);
        shmctl(segment_id, IPC_RMID, NULL);
    }

    return 0;
}
