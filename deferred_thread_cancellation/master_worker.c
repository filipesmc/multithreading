/*
  Deferred cancellation permite que nós escolhemos em qual ponto de execução
  a thread pode ser cancelada. No código que utilizei no cancelamento assincrono
  ha um problema de "variants/invariants" pois apesar de tratarmos por exemplo
  o vazamento de memória, a thread faz chamada de sistema (fwrite) e sem o
  devido tratamento pode corromper as estruturas internas usadas pela syscall.
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<stdbool.h>

#define WORKERS_QTD 5
#define NAME_SIZE 255
pthread_t worker_thread[WORKERS_QTD];

void free_memory(void* ptr){
    if(ptr != NULL){
        free(ptr);
    }
}

void close_file_descriptor(void* file_ptr){
    fclose((FILE *)file_ptr);
}

void *write_file(void *param){

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0); 
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, 0); 

    char file[NAME_SIZE];
    char text_to_write[100];
    int counter = 0;
    int lenght = 0;
    short *thread_identifier = (short *)param;

    pthread_cleanup_push(free_memory, (void *)thread_identifier);

    sprintf(file, "file_%hi.text", *thread_identifier);
    FILE *file_ptr = fopen(file, "w");
    
    if(file_ptr == NULL){
        fprintf(stderr, "[+]Erro %d ao abrir arquivo\n", errno);
        pthread_exit(0);
    }

    pthread_cleanup_push(close_file_descriptor, (void *)file_ptr);

    while(true){
        lenght = sprintf(text_to_write, "%d : Thread Número(identificador): %hi\n", counter++, *thread_identifier);
        fwrite(text_to_write, sizeof(char), lenght, file_ptr);
        fflush(file_ptr);
        sleep(2);
        /* Onde é seguro terminar essa thread. Dessa forma garantimos que a thread não será terminada
            em qualquer ponto aleatório. */
        pthread_testcancel();
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return 0;
}

int main(void){

    short *thread_identifier = 0;
 
    for(unsigned int i = 0; i < WORKERS_QTD; ++i){
        thread_identifier = malloc(1 * sizeof(*thread_identifier /* boa prática caso precise mudar o tipo */));
        *thread_identifier = i;
        pthread_create(&worker_thread[i], NULL, write_file, (void *)thread_identifier);
    }

    unsigned short choice = 0;
    unsigned short thread_number;

    while(true){

        printf("1 - Cancelar thread.\n");
        scanf("%hu", &choice);
        printf("Digite o número da worker thread [0 ~ %d]: \n", WORKERS_QTD -1);
        scanf("%hu", &thread_number);
        if(thread_number < 0 || thread_number >= WORKERS_QTD){
            fprintf(stderr, "Número de thread inválido!\n");
            exit(-1);
        }

        switch (choice)
        {
        case 1: 
            pthread_cancel(worker_thread[thread_number]);
            break;
        
        default:
            continue;
        }

    }   

    return 0;
}