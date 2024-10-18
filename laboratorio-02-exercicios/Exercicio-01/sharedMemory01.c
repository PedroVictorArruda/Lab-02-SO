#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  // Criando o segmento de memória compartilhada com o tamanho de um inteiro
  int segment_id = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);

  // Criando o ponteiro que vai apontar para o endereço de memória compartilhada
  int *sharedMemory;
  
  // Inicializando a memória compartilhada com o valor 0
  sharedMemory = (int *)shmat(segment_id, NULL, 0);
  *sharedMemory = 0;  // Inicializa o valor da memória compartilhada
  shmdt(sharedMemory); // Desanexa a memória compartilhada

  pid_t pid1 = fork(); // Criando o primeiro processo filho
  if (pid1 < 0) { // Erro na criação do processo
    printf("Erro ao criar o processo filho 1.\n");
    return 1;
  } else if (pid1 == 0) { // Primeiro processo filho
    // Anexando o segmento de memória compartilhada
    sharedMemory = (int *)shmat(segment_id, NULL, 0);
    // Incrementando o valor da memória compartilhada
    *sharedMemory += 1;
    // Desanexando a memória compartilhada
    shmdt(sharedMemory);
    // Encerrando o processo filho
    exit(0);
  }

  pid_t pid2 = fork(); // Criando o segundo processo filho
  if (pid2 < 0) { // Erro na criação do processo
    printf("Erro ao criar o processo filho 2.\n");
    return 1;
  } else if (pid2 == 0) { // Segundo processo filho
    // Anexando o segmento de memória compartilhada
    sharedMemory = (int *)shmat(segment_id, NULL, 0);
    // Incrementando o valor da memória compartilhada
    *sharedMemory += 2;
    // Desanexando a memória compartilhada
    shmdt(sharedMemory);
    // Encerrando o processo filho
    exit(0);
  }

  // Processo pai esperando que os dois filhos terminem
  wait(NULL); // Espera o primeiro processo filho
  wait(NULL); // Espera o segundo processo filho

  // Anexando o segmento de memória compartilhada
  sharedMemory = (int *)shmat(segment_id, NULL, 0);
  // Imprimindo o valor final da memória compartilhada
  printf("Valor final na memória compartilhada: %d\n", *sharedMemory);

  // Removendo o segmento de memória compartilhada
  shmctl(segment_id, IPC_RMID, NULL);

  return 0;
}
