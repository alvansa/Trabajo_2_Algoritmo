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
        printf(" [%d / %d] -> ", head->data, head->rep);
        head = head->sig;
    }

    printf("||- ");
}

int busqueda(int num, int tipo, struct Nodo *head)
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

struct Nodo *agregar_nodo_ordenado(int num, struct Nodo *head)
{

    struct Nodo *newNodo, *aux;
    newNodo = (struct Nodo *)malloc(sizeof(struct Nodo));

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

void eliminar_mayor(int num, struct Nodo **head)
{
    struct Nodo *actual, *previo;
    actual = *head;
    previo = NULL;

 
    //Si la cabeza es mayor que el numero
    while(actual != NULL && actual->sig != NULL){
        if(actual->sig->data >= num){
            actual = actual -> sig;
        }else{
            previo = actual -> sig;
            actual->sig = previo->sig;
            free(previo);
        }

    }

    //mostrar_lista(*head);
    //head = actual;

    /*
    if (!head)
        return head;
    while (actual && actual->data < num)
        actual = actual ->sig;

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
            return head;
        }
    }
    //head = actual;
    return head;*/
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

    return head;
}

int main(int argv, char **argc)
{
    int rep, i, random, j;
    int lee, rol, I_D, D_opcion;

    // random = 1;
    srand(time(NULL));
    if (argv == 2)
    {
        rep = atoi(argc[1]);
        for (i = 0; i < rep; i++)
        {
            random = rand() % 100;
            head = agregar_nodo_ordenado(random, head);
        }
        mostrar_lista(head);
        printf("\nAntes del mayor\n");
        for (i = 0; i < 5 ; i++){
            random = rand() % 100;
            printf("\nrandom -> %d\n",random);
            eliminar_mayor(0, &head);
            mostrar_lista(head);
        }

            
        printf("\nAntes del menor\n");
        //head = eliminar_menor(0,head);
        printf("\nDespues\n");
        
    }
    else
    {
        printf("\n Mal uso del programa \n");
    }

    /*
    for (i = 0; i < 5; i++)
    {
        random = rand() % 20;
        printf("\nBuscar el numero %d\n", random);
        j = busqueda(random,0,head);

    }

    random = rand() % 20;
    head = eliminar_mayor(random,head);
    printf("\nSe han eliminado todos los datos mayores a %d, y la lista es:\n",random);
    mostrar_lista(head);


    head = eliminar_menor(random,head);
    printf("\nSe han eliminado todos los datos menores a %d, y la lista es:\n",random);
    mostrar_lista(head);

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

/*
for (i = 0; i < rep; i++)
        {
            rol = rand() % 2;
            switch (rol)
            {
            case 0: // escritor
                I_D = rand() % 2;
                switch (I_D)
                {
                case 0: // Insertar
                    random = rand();
                    head = agregar_nodo_ordenado(random, head);
                    printf("\nNodo ingresado -> %d\n", random);
                    break;

                case 1: // Eliminar
                    if( head == NULL)
                    {
                        printf("\nLa lista se encuentra vacia se terminara el programa\n");
                        return 0;
                    }
                    D_opcion = rand() % 3;
                    switch (D_opcion)
                    {
                    case 0: // menor
                        random = rand();
                        head = eliminar_menor(random, head);
                        printf("\nNodo ingresado -> %d\n", random);
                        lee = 0;
                        break;
                    case 1: // igual
                        random = rand();
                        head = eliminar_nodo(random, head);
                        printf("\nNodo ingresado -> %d\n", random);
                        break;
                    case 2: // mayor
                        random = rand();
                        head = eliminar_mayor(random, head);
                        printf("\nNodo ingresado -> %d\n", random);
                        break;
                    }
                    break;
                }
                break;
            case 1: // lector
                if (lee == 0)
                {
                    random = rand();
                    printf("\nEl proceso esta buscando el numero %d\n", random);
                    j = busqueda(random, 0, head);
                }
                else
                {
                    lee = 0;
                }
                break;
            }
        }
        mostrar_lista(head);
*/