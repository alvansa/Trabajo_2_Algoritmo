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
        printf(" %d / %d -> ", head->data,head->rep);
        head = head->sig;
    }

    printf("||- ");
}


struct Nodo *agregar_nodo_ordenado(int num)
{
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    newNodo->data = num;
    newNodo->sig = head;
    newNodo -> rep = 1;

    if (!head || newNodo->data < head->data)
    {
        head = newNodo;
        // head -> sig = NULL;
    }
    else
    {
        aux = head->sig;

        while (aux && aux->data <= newNodo->data)
        {
            if(aux -> data == num)
                newNodo -> rep ++;
            newNodo->sig = aux;
            aux = aux->sig;
        }
        newNodo->sig->sig = newNodo;
        newNodo->sig = aux;
    }

    return head;
}


struct Nodo *eliminar_nodo(int num){
    struct Nodo *actual,*previo;
    
    if (!head)
        return head;
    while (head && head->data == num)
        head = head->sig;
    actual = head;
    previo = NULL;

    while (actual) {
        if (actual->data == num){
            previo->sig = actual->sig;
            free(actual);
        }
        else
            previo = actual;
        actual = actual->sig;
    }
    return head;
}

struct Nodo *eliminar_mayor(int num){
    struct Nodo *actual,*previo;

    if (!head)
        return head;
    while (head && head->data == num)
        head = head->sig;

    actual = head;
    previo = NULL;

    while (actual) {
        if (actual->data > num){
            previo->sig = NULL;
            free(actual);
        }
        else
            previo = actual;
        actual = actual->sig;
    }
    return head;
    
}


struct Nodo *eliminar_menor(int num){
    struct Nodo *actual,*previo;

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

struct Nodo *busqueda(int num){
    struct Nodo *aux;
    aux = head;
    while(aux != NULL)
    {   
        if(aux -> data == num){
            printf("\nSe ha encontrado el numero buscado %d // %d\n",aux->data,aux->rep);
            return head;
        }else
        aux = aux->sig;
    }
    if (!aux){
        printf("\nNot Found\n");
    }

    return head;
}

int main(int argv, char **argc)
{
    int rep, i, random;

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

    
    for (i = 0; i < 5; i++){
        random = rand() % 20;
        printf("\nBuscar el numero %d\n",random);
        head = busqueda(random);
    } 
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