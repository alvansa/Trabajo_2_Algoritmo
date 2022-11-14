/*
Nombres: Valentina Rodríguez Muñoz y Álvaro Soto Albornoz 
Profesor: Ruben Carvajal Schiaffino
IDE: Visual Studio Code 1.73.0
SO: Ubuntu 20.04.5
Fecha: 13 de Noviembre del 2022
Descripcion del programa: Programa que trabaja con una lista enlazada de forma distribuida a travez de hilos
los hilos pueden tomar roles de actualizador o lector
la lectura maxima esta definida por el usuario
solo puede haber un actualizador a la vez
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


sem_t sem;
pthread_mutex_t escritor;
pthread_mutex_t lector;
int lee = 0; //variable para verificar si un proceso puede leer la lista 0 = si , 1 = no

//Estructura ocupada para enviar el mensaje al hilo
struct Message
{
    int id, iter;
};

struct Nodo
{
    int data;
    int rep;
    struct Nodo *sig;
};

struct Nodo *head = NULL;

//Funcion para mostrar la lista
void mostrar_lista(struct Nodo *head)
{

    while (head != NULL)
    {
        printf(" %d / %d -> ", head->data, head->rep);
        head = head->sig;
    }

    printf("||- ");
}

//Funcion de busqueda de numero
//recibe el nuemero a buscar , tipo de busqueda 1 para ingresar un numero, 0 solo para buscar, cabeza de la lista
//id del proceso que esta buscando el numero
//Si se encuentra el nuemero envia el proceso que lo busco el numero y la cantidad de veces que aparece
// Si le numero no se encuentra envia el proceso que lo busco y el numero
int busqueda(int num, int tipo, struct Nodo *head, int id)
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
                printf("\nEl proceso %d encontro el numero %d %d veces\n", id, aux->data, aux->rep);
            }
            return 1;
        }
        else
            aux = aux->sig;
    }
    if (!aux)
    {
        if (tipo == 0)
            printf("El proceso %d, busco %d y Not Found\n", id, num);
        return 0;
    }
}

//Funcion que elimina un nodo
struct Nodo *eliminar_nodo(int num, struct Nodo *head)
{
    struct Nodo *actual, *previo;

    if (!head)
        return head;
    while (head && head->data == num)
        head = head->sig;
    actual = head;
    previo = NULL;

    while (actual)
    {
        if (actual->data == num)
        {
            previo->sig = actual->sig;
            free(actual);
        }
        else
            previo = actual;
        actual = actual->sig;
    }
    return head;
}

//Funcion que elimina todos los datos mayores a un numero en la lista
void eliminar_mayor(int num, struct Nodo **head)
{
    struct Nodo *actual = *head;
    struct Nodo *previo = NULL;
    struct Nodo *temp;

    while (actual != NULL)
    {
        if (actual->data > num)
        {
            temp = actual;
            actual = actual->sig;
            if (previo == NULL)
                *head = actual;
            else
                previo->sig = actual;
            free(temp);
        }
        else
        {
            previo = actual;
            actual = actual->sig;
        }
    }
}

//Funcion que elimina todos los datos menores a un numero en la lista
struct Nodo *eliminar_menor(int num, struct Nodo *head)
{
    struct Nodo *actual, *previo;

    if (!head)
        return head;
    while (head && head->data < num)
    {
        actual = head;
        head = head->sig;
        free(actual);
    }

    actual = head;
    previo = NULL;
    return head;
}

//Funcion que agrega un nodo a la lista, recibe el numero a ingresar y la cabeza de la lista
struct Nodo *agregar_nodo_ordenado(int numero, struct Nodo *head)
{

    int num = numero;
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    pthread_rwlock_t *p;

    if (busqueda(num, 1, head, 0) == 1)
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

void Usage(char *message)
{

    printf("\nUsage: %s u c i ", message);
    printf("\nu: Cantidad de usuarios \n\n");
    printf("c: Capacidad de lectura concurrente \n\n");
    printf("i: Iteraciones \n\n");
}

//Funcion para simular los usuarios en los hilos
void *usuarios(void *total)
{

    int rol, Insert_Delete, Mayo_igual_men, i, random, j;
    struct Message *m;
    int rep = 20;

    m = (struct Message *)total;

    for (i = 0; i < m->iter; i++)
    {
        rol = rand() % 2;

        switch (rol)
        {
        case 0: // escritor
            // jamau
            Insert_Delete = rand() % 2;
            switch (Insert_Delete)
            {
            case 0: // Insertar
                pthread_mutex_lock(&escritor);
                pthread_mutex_lock(&lector);
                lee = 1;

                random = rand();
                head = agregar_nodo_ordenado(random, head);
                lee = 0;

                pthread_mutex_unlock(&lector);
                pthread_mutex_unlock(&escritor);
                break;

            case 1: // Eliminar nodos
                if (head == NULL)
                {
                    printf("\nLa lista se encuentra vacia, el programa terminara\n");
                    break;
                }

                Mayo_igual_men = rand() % 3;

                random = rand();
                switch (Mayo_igual_men)
                {
                case 0: // Eliminar nodos menores a
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    head = eliminar_menor(random, head);
                    lee = 0;
//hi
                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                case 1: // Eliminar el nodo
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    head = eliminar_nodo(random, head);
                    lee = 0;

                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                case 2: // Eliminar los ndoos mayores a
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    eliminar_mayor(random, &head);
                    lee = 0;

                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                }
                break;
            }

            break;
        case 1: // lector
            if(head == NULL)
                break;
            if (lee == 0)
            {
                sem_wait(&sem);
                random = rand();
                j = busqueda(random, 0, head, m->id);
                //fflush(stdout);
                sem_post(&sem);
            }
            else
            {
                pthread_mutex_lock(&lector);
                lee = 0;
                pthread_mutex_unlock(&lector);
            }

            break;
        }
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    pthread_t *threads;
    pthread_attr_t attribute;
    struct Message **mensaje;
    int i, cant, random, rep, usu, lec_conc, iter;

    if (argc == 4)
    {
        mensaje = calloc(1, sizeof(struct Message *));
        rep = 20;
        usu = atoi(argv[1]);
        lec_conc = atoi(argv[2]);
        iter = atoi(argv[3]);

        threads = calloc(usu, sizeof(pthread_t));
        for (i = 0; i < usu; i++)
            mensaje[i] = calloc(1, sizeof(struct Message));

        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

        pthread_mutex_init(&escritor, NULL);
        pthread_mutex_init(&lector, NULL);

        sem_init(&sem, 0, lec_conc);

        for (i = 0; i < usu; i++)
        {
            mensaje[i]->id = i;
            mensaje[i]->iter = iter;
            pthread_create(&threads[i], &attribute, usuarios, (void *)mensaje[i]);
        }
        for (i = 0; i < usu; i++)
            pthread_join(threads[i], NULL);

        sem_destroy(&sem);
    }
    else
    {
        Usage(argv[0]);
    }

    return 0;
}
