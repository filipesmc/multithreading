#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<stdbool.h>

#define WORKERS_QTD 5
#define NAME_SIZE 255
pthread_t worker_thread[WORKERS_QTD];

void *write_file(void *param){

    char file[NAME_SIZE];
    char text_to_write[100];
    int counter = 0;
    int lenght = 0;
    short *thread_identifier = (short *)param;

    sprintf(file, "thread_%hi.text", *thread_identifier);
    FILE *file_ptr = fopen(file, "w");

    if(file_ptr == NULL){
        fprintf(stderr, "[+]Erro %d ao abrir arquivo\n", errno);
        exit(-1);
    }

    while(true){
        lenght = sprintf(text_to_write, "%d : Thread Número(identificador): %hi\n", counter++, *thread_identifier);
        fwrite(text_to_write, sizeof(char), lenght, file_ptr);
        fflush(file_ptr);
        sleep(2);
    }

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
            break;
        
        default:
            continue;
        }

    }   

    return 0;
}