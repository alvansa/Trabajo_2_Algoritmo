#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int MAX_LECTORES = 0;



struct Message
{
    int id_sem, value;
};

void *usuarios(void *total)
{
    printf("\nHI\n");
    int eleccion_1, eleccion_2,i,random,j;
    struct Message *m;
    pthread_rwlock_t rwlock_lector;
    pthread_rwlock_t rwlock_escritor; 

    m = (struct Message *) total ;
    for (i = 0; i < m->value; i++)
    {
        //eleccion_1 = rand() % 2;
        eleccion_1 = 1;

        switch (eleccion_1)
        {
        case 0: // escritor
            pthread_rwlock_lock(&rwlock_escritor);
            //cosas pasan
            pthread_rwlock_unlock(&rwlock_escritor);
            break;
        case 1: // lector
            if(lectores_actuales < MAX_LECTORES){
                pthread_rwlock_rdlock(&rwlock_lector);
            random = rand();
            pthread_rwlock_unlock(&rwlock_lector);
            }
            
            break;
        }
    }
}

// ./t2.exe usuarios capacidad iteraciones
int main(int argv, char **argc)
{

    pthread_t *threads;
    struct Message *mensaje;
    int i, cant, random, rep, usu, lec_conc;

    mensaje = calloc(1,sizeof(struct Message));
    rep = 20;
    usu = atoi(argc[1]);
    MAX_LECTORES = atoi(argc[2]);
    mensaje->value = atoi(argc[3]);

    threads = calloc(usu, sizeof(pthread_t));



    for (i = 0; i < usu; i++)
    {
        pthread_create(&threads[i], NULL, usuarios, (void *) mensaje);
    }


    return 0;
}