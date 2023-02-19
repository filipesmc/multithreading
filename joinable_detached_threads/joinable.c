#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> 
#include<unistd.h>
#include<errno.h>   

pthread_t handle_1;
pthread_t handle_2;

void* calcule_square(void* value){
    
    int th_id = *(int *)value;
    free(value);
    
    int rc = 0;

    while(rc != th_id){
        printf("Thread %d executando...\n", th_id);
        sleep(1);
        rc++;
    }

    int* result = calloc(1, sizeof(int));
    *result = th_id * th_id;

    return (void *) result;
}

void thread_calcule_square_init(pthread_t *handle, int value){
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int* thread_param = calloc(1, sizeof(int));
    *thread_param = value;
    
    pthread_create(handle, &attr, calcule_square, (void *)thread_param);
}

int main(int argc, char **argv){

    thread_calcule_square_init(&handle_1, 2); 
    thread_calcule_square_init(&handle_2, 10); 

    void *thread_result2;
    void *thread_result3;
    
    printf("Main thread bloqueada aguardando pelo join da thread no. 2\n");
    pthread_join(handle_1, &thread_result2);

    if(thread_result2){

        printf("Resultado da thread 2 = %d\n", *(int *) thread_result2);
        free(thread_result2);
        thread_result2 = NULL;
    }

    printf("Main thread bloqueada aguardando pelo join da thread no. 10\n");
    pthread_join(handle_2, &thread_result3);

    if(thread_result3){

        printf("Resultado da thread 10 = %d\n", *(int *) thread_result3);
        free(thread_result3);
        thread_result3 = NULL;
    }

    printf("terminando programa\n");

    return 0;
}