
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int LECTORES = 0;

struct Nodo
{
    int data;
    int rep;
    struct Nodo *sig;
};

struct Message
{
    int id_sem, value;
};

struct Nodo *head = NULL;

void mostrar_lista(struct Nodo *head)
{

    while (head != NULL)
    {
        printf(" %d / %d -> ", head->data, head->rep);
        head = head->sig;
    }

    printf("||- ");
}

int busqueda(int num, int tipo)
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

struct Nodo *agregar_nodo_ordenado(int numero)
{

    int num = numero;
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    pthread_rwlock_t *p;

    if (busqueda(num, 1) == 1)
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
    printf("\nHI\n");
    int eleccion_1, eleccion_2,i,random,j;
    struct Message *m;
    int rep = 20;
    pthread_rwlock_t rwlock_lista; 

    m = (struct Message *) total ;
    for (i = 0; i < m->value; i++)
    {
        //eleccion_1 = rand() % 2;
        eleccion_1 = 1;

        switch (eleccion_1)
        {
        case 0: // escritor
            break;
        case 1: // lector
            pthread_rwlock_rdlock(&rwlock_lista);
            random = rand() % rep;
            j = busqueda(random,0);
            pthread_rwlock_unlock(&rwlock_lista);
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
    lec_conc = atoi(argc[2]);
    mensaje->value = atoi(argc[3]);

    threads = calloc(usu, sizeof(pthread_t));

    for(int j = 0; j < 100; j++){
        random = rand() % 50;
        head = agregar_nodo_ordenado(random);
    }
    mostrar_lista(head);

    for (i = 0; i < usu; i++)
    {
        pthread_create(&threads[i], NULL, usuarios, (void *) mensaje);
    }


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