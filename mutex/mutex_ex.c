#include<pthread.h>
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include<stdlib.h>
#include<errno.h>


int array[] = {1, 2, 3, 4, 5};
pthread_mutex_t array_mutex;

void print_arr(){

    size_t size = sizeof(array)/sizeof(array[0]);
    unsigned int i = 0;

    for( ; i < size - 1; ++i){
        printf("%d\n", array[i]);
    }
    printf("%d\n", array[i]);
}

static void* change_elements(void *param){ /* função que faz uma write operation, fazendo um swap entre o primeiro e ultimo elemento do array */

    int arr_size = sizeof(array)/sizeof(array[0]);
    int temp = 0;

    do{
        pthread_mutex_lock(&array_mutex);
        temp = array[0];
        array[0] = array[arr_size - 1];
        array[arr_size - 1] = temp;
        print_arr();
        pthread_mutex_unlock(&array_mutex);
    }while(true);
    
}

static void* sum_elements(void *param){
    unsigned int sum;
    size_t arr_size = sizeof(array)/sizeof(array[0]);
    
    do{
        sum = 0;
        pthread_mutex_lock(&array_mutex);
        for(unsigned int i = 0; i < arr_size - 1; ++i){
            sum += array[i];
        }
        printf("Soma: %d\n", sum);
        print_arr();
        pthread_mutex_unlock(&array_mutex);
    }while(true);
    
}

void create_write_thread(){
    pthread_t handle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&handle, &attr, change_elements, NULL);
}

void create_sum_thread(){
    pthread_t handle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE /*é o valor default, mas custa nada deixar explicito*/);
    pthread_create(&handle, &attr, sum_elements, NULL);
}


int main(void){
    pthread_mutex_init(&array_mutex, NULL);
    create_write_thread();
    create_sum_thread();
    pthread_exit(0);
    return 0;
}