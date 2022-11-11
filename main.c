#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

struct Nodo *agregar_nodo_ordenado(int num)
{

    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    if (busqueda(num, 1) == 1){
        //printf("\nEncontre el dato -> %d\n",num);
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

struct Nodo *eliminar_nodo(int num)
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

struct Nodo *eliminar_mayor(int num)
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
            previo->sig = NULL;
            free(actual);
        }
        else
            previo = actual;
        actual = actual->sig;
    }
    return head;
}

struct Nodo *eliminar_menor(int num)
{
    struct Nodo *actual, *previo;

    if (!head)
        return head;
    while (head && head->data < num)
        head = head->sig;

    actual = head;
    previo = NULL;
    /*
    while (actual) {
        if (actual->data < num){
            previo->sig = NULL;
            free(actual);
        }
        else
            previo = actual;
        actual = actual->sig;
    }*/
    return head;
}


int main(int argv, char **argc)
{
    int rep, i, random,j;

    // random = 1;
    srand(time(NULL));
    if (argv == 2)
    {
        rep = atoi(argc[1]);

        for (i = 0; i < rep; i++)
        {
            random = rand() % 20;
            head = agregar_nodo_ordenado(random);
            // random++;
            // printf("\n dato random =  %d  \n", random);
        }
        mostrar_lista(head);
    }
    else
    {
        printf("\n Mal uso del programa \n");
    }

    for (i = 0; i < 5; i++)
    {
        random = rand() % 20;
        printf("\nBuscar el numero %d\n", random);
        j = busqueda(random,0);

    }

    random = rand() % 20;
    head = eliminar_mayor(random);
    printf("\nSe han eliminado todos los datos mayores a %d, y la lista es:\n",random);
    mostrar_lista(head);
    

    head = eliminar_menor(random);
    printf("\nSe han eliminado todos los datos menores a %d, y la lista es:\n",random);
    mostrar_lista(head);
    /*
    random = rand() % 20;
    head = eliminar_mayor(random);
    random = rand() % 20;
    head = eliminar_nodo(random);
    */
    /*
    random = rand() % 20;
    head = eliminar_menor(random);
    printf("\nSe han eliminado todos los datos mayores a %d, y la lista es:\n",random);
    mostrar_lista(head);
    */

    // Busqueda
    // Insertar
    // Eliminar
    // Eliminar mayor
    // Eliminar menor
    //

    return 0;
}