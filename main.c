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
        printf(" %d -> ", head->data);
        head = head->sig;
    }

    printf("||- ");
}

struct Nodo *agregar_nodo(int num)
{
    struct Nodo *newNodo;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    newNodo->data = num;
    newNodo->sig = head;

    head = newNodo;

    return head;
}

struct Nodo *agregar_nodo_ordenado(int num)
{
    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

    newNodo->data = num;
    newNodo->sig = head;

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

struct Nodo *eliminar_dato(int num)
{
    struct Nodo *aux, *aux2;

    aux = head;
    if (head == NULL)
    {
        printf("\nLista Vacia\n");
        return head;
    }
    else
    {
        if (aux->data == num)
        {
            head = head->sig;
            free(aux);
        }
        else
        {
            do
            {
                aux2 = aux;
                aux = aux->sig;
            } while (aux && aux->data != num);

            if (aux)
            {
                //do{
                    aux2->sig = aux->sig;
                    free(aux);
                //}while(aux2->sig->data != num);
                
            }
            else
            {
                printf("\nEl dato no se encuentra en la lista\n");
            }
        }
    }

    return head;
}


struct Nodo *eliminar_nodo(int num){
    struct Nodo *actual,*previo;
    
    if (!head)
        return head;
    // Until the head data is equal to the key move the head
    // pointer
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
   // if(actual == NULL)
        //printf("\nEl dato no se encuentra en la lista\n");
    return head;
}

struct Nodo *busqueda(int num){
    struct Nodo *aux;
    aux = head;
    while(aux != NULL)
    {   
        if(aux -> data == num){
            printf("\nSe ha encontrado el numero buscado %d\n",num);
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
    return 0;
}