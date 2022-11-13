
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

sem_t sem;
pthread_mutex_t escritor;
pthread_mutex_t lector;
int lee = 0;

struct Message
{
    int id_sem, value, usu;
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
        head = head->sig;
    }

    printf("||- ");
}

int busqueda(int num, int tipo, struct Nodo *head)
{

    struct Nodo *aux;
    aux = head;
    if (tipo == 0)
        printf("\nSe busca el numero %d ", num);
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
            printf(" Not Found\n");
        return 0;
    }
}

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

struct Nodo *eliminar_mayor(int num, struct Nodo *head)
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
        if (actual->data > num)
        {
            previo = actual;
            actual = actual->sig;
            free(previo);
        }
        else
        {
            previo = actual;
            actual = actual->sig;
        }
    }
    head = actual;
    return head;
}

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

struct Nodo *agregar_nodo_ordenado(int numero, struct Nodo *head)
{

    int num = numero;
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    pthread_rwlock_t *p;

    if (busqueda(num, 1, head) == 1)
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

    int rol, I_D, D_opcion, i, random, j;
    struct Message *m;
    int rep = 20;
    pthread_rwlock_t rwlock_lista;

    m = (struct Message *)total;
    printf("\nProceso -> %d\n", m->id_sem);
    for (i = 0; i < m->value; i++)
    {
        rol = rand() % 2;

        switch (rol)
        {
        case 0: // escritor
            // jamau
            I_D = rand() % 2;
            switch (I_D)
            {
            case 0: // Insertar
                pthread_mutex_lock(&escritor);
                pthread_mutex_lock(&lector);
                lee = 1;

                random = rand();
                head = agregar_nodo_ordenado(random, head);
                printf("\nNodo ingresado -> %d\n", random);
                lee = 0;

                pthread_mutex_unlock(&lector);
                pthread_mutex_unlock(&escritor);
                break;

            case 1: // Eliminar
                if(head == NULL){
                    printf("\nLa lista se encuentra vacia, el programa terminara\n");
                    return 0;
                }
                    
                D_opcion = rand() % 3;
                switch (D_opcion)
                {
                case 0: // menor
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    random = rand();
                    head = eliminar_menor(random, head);
                    printf("\nNodo ingresado -> %d\n", random);
                    lee = 0;

                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                case 1: // igual
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    random = rand();
                    head = eliminar_nodo(random, head);
                    printf("\nNodo ingresado -> %d\n", random);
                    lee = 0;

                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                case 2: // mayor
                    pthread_mutex_lock(&escritor);
                    pthread_mutex_lock(&lector);
                    lee = 1;

                    random = rand();
                    head = eliminar_mayor(random, head);
                    printf("\nNodo ingresado -> %d\n", random);
                    lee = 0;

                    pthread_mutex_unlock(&lector);
                    pthread_mutex_unlock(&escritor);
                    break;
                }
                break;
            }

            break;
        case 1: // lector
            if (lee == 0)
            {
                sem_wait(&sem);
                random = rand();
                j = busqueda(random, 0, head);
                fflush(stdout);
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

// ./t2.exe usuarios capacidad iteraciones
int main(int argv, char **argc)
{
    srand(0);
    pthread_t *threads;
    pthread_attr_t attribute;
    struct Message **mensaje;
    int i, cant, random, rep, usu, lec_conc, iter;

    mensaje = calloc(1, sizeof(struct Message));
    rep = 20;
    usu = atoi(argc[1]);
    lec_conc = atoi(argc[2]);
    iter = atoi(argc[3]);

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
        mensaje[i]->id_sem = i;
        mensaje[i]->value = iter;
        mensaje[i]->usu = usu;
        pthread_create(&threads[i], NULL, usuarios, (void *)mensaje[i]);
    }
    for (i = 0; i < usu; i++)
        pthread_join(threads[i], NULL);

    mostrar_lista(head);
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