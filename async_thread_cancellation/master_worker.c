/*
   Cancelamento de thread.
   - O.S pode ou não terminar ela imediatamente.
   - Tem dois tipos de cancelamento: deferida ou assincrona.
   - Na forma assincrona que é feito nesse exemplo, o pedido de cancelamento é enfileirado
     pelo sistema operacional. 
   - Tendo recebido o pedido de cancelamento, o sistema ira "procurar" por uma oportunidade 
     para terminar a thread.

    compilação: gcc-11 -std=c11 master_worker.c -o master_worker -lpthread (openSuse)
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
    printf("%s invoked...\n", __FUNCTION__);    
    if(ptr != NULL){
        free(ptr);
    }
}

void close_file_descriptor(void* file_ptr){
    printf("%s invoked...\n", __FUNCTION__);
    fclose((FILE *)file_ptr);
}

void *write_file(void *param){

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0); /* permite ela ser cancelada */
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0); /* modo de cancelamento */

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
        exit(-1);
    }

    pthread_cleanup_push(close_file_descriptor, (void *)file_ptr);

    while(true){
        lenght = sprintf(text_to_write, "%d : Thread Número(identificador): %hi\n", counter++, *thread_identifier);
        fwrite(text_to_write, sizeof(char), lenght, file_ptr);
        fflush(file_ptr);
        sleep(2);
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