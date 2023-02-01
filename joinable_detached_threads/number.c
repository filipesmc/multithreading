#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void* reduce_number(void* param){

    int p = *(int *)param;
    free(param);
    p--;
    int* result_param = malloc(sizeof(int));
    *result_param = p;
    return (void *) result_param;

}

void init_thread(pthread_t* handle, int thread_param){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int* param = malloc(sizeof(int));
    *param = thread_param;

    /*Parametros de threads precisam ser alocados na heap ou ser variáveis globais*/
    if (pthread_create(handle, &attr, reduce_number, (void *)param) != 0){
        printf("erro\n");
        exit(-1);
    }

}

int main(int argc, char **argv){

    int number_of_threads = 0;
    printf("Enter the number:\n");
    scanf("%d", &number_of_threads);

    pthread_t thr_arr[number_of_threads];
    
    void* result = NULL;
    
    for(unsigned int i = 0; i < number_of_threads; ++i){
        init_thread(&thr_arr[i], number_of_threads - i);
    }

    for(unsigned int i = 0; i < number_of_threads; ++i){
        
        pthread_join(thr_arr[i], &result);
        
        if(result){
            printf("%d\n", *(int *)result);
            free(result);
            result = NULL;
        }
    }

    return 0;
}