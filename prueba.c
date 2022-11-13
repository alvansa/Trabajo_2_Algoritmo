
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int LECTORES = 0;
sem_t  sem;


struct Message
{
    int id_sem, value;
};

struct Nodo
{
    int data;
    int rep;
    struct Nodo *sig;
};

struct Nodo *head = NULL;

void mostrar_lista(struct Nodo *head)
{

    while (head != NULL)
    {
        printf(" %d / %d -> ", head->data, head->rep);
        head = head ->sig;
    }

    printf("||- ");
}

int busqueda(int num, int tipo,struct Nodo *head)
{
    struct Nodo *aux;
    aux = head;
    while (aux != NULL)
    {
        if (aux->data == num)
        {
            if (tipo == 1)
                aux->rep++;
            else
            {
                printf("\nSe ha encontrado el numero buscado %d // %d\n", aux->data, aux->rep);
            }
            return 1;
        }
        else
            aux = aux->sig;
    }
    if (!aux)
    {
        if (tipo == 0)
            printf("\nNot Found\n");
        return 0;
    }
}

struct Nodo *agregar_nodo_ordenado(int numero,struct Nodo *head)
{

    int num = numero;
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    pthread_rwlock_t *p;

    if (busqueda(num, 1,head) == 1)
    {
        // printf("\nEncontre el dato -> %d\n",num);
        return head;
    }

    else
    {
        newNodo->data = num;
        newNodo->sig = head;
        newNodo->rep = 1;

        if (!head || newNodo->data < head->data)
        {
            head = newNodo;
            // head -> sig = NULL;
        }
        else
        {
            aux = head->sig;

            while (aux && aux->data < newNodo->data)
            {
                newNodo->sig = aux;
                aux = aux->sig;
            }
            newNodo->sig->sig = newNodo;
            newNodo->sig = aux;
        }

        return head;
    }
}




void *usuarios(void *total)
{
    
    int eleccion_1, eleccion_2,i,random,j;
    struct Message *m;
    int rep = 20;
    pthread_rwlock_t rwlock_lista; 

    m = (struct Message *) total ;
    printf("\nProceso -> %d\n",m->id_sem);
    for (i = 0; i < m->value; i++)
    {
        //eleccion_1 = rand() % 2;
        eleccion_1 = 1;

        switch (eleccion_1)
        {
        case 0: // escritor

            break;
        case 1: // lector
            sem_wait(&sem);
            random = rand() % 50;
            printf("\nEl proceso %d esta buscando el numero %d\n",m->id_sem,random);
            j = busqueda(random,0,head);
            fflush(stdout);
            sem_post(&sem);
            break;
        }
    }
}

// ./t2.exe usuarios capacidad iteraciones
int main(int argv, char **argc)
{
    srand(0);
    pthread_t *threads;
    pthread_attr_t attribute;
    struct Message **mensaje;
    int i, cant, random, rep, usu, lec_conc,iter;

    mensaje = calloc(1, sizeof(struct Message));
    rep = 20;
    usu = atoi(argc[1]);
    lec_conc = atoi(argc[2]);
    iter = atoi(argc[3]);

    threads = calloc(usu, sizeof(pthread_t));
    for(i = 0 ; i < usu ; i++) 
        mensaje[i] = calloc(1,sizeof(struct Message));

    for(int j = 0; j < 100; j++){
        random = rand() % 50;
        head = agregar_nodo_ordenado(random,head);
    }
    //mostrar_lista(head);

    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

    sem_init(&sem,0,lec_conc);

    for (i = 0; i < usu; i++)
    {
        mensaje[i] -> id_sem = i;
        mensaje[i] -> value = iter;
        pthread_create(&threads[i], NULL, usuarios, (void *) mensaje[i]);
    }
    for(i = 0; i < usu; i++)
        pthread_join(threads[i],NULL);

    sem_destroy(&sem);
    return 0;
}

//  thread_rwlock_rdlock(p);
// busqueda(x)
// busqueda(y)
// thread_rwlock_unlock(p);

//  thread_rwlock_wrlock(p);
// agregar_nodo_ordenado(x1)
// thread_rwlock_unlock(p);


// cada proceso debe recibir -numero de iteraciones