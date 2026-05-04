#include "solucion1.hpp"
#include <iostream>

using namespace std;

void inicializarArreglo(arreglo &arr, int capacidadInicial) // configura el estado inicial de la estructura arreglo y además prepara el índice alfabético
{
    arr.capacidad = capacidadInicial; // capacidad máxima inicial antes de redimensionar
    arr.cantidad = 0; //inicio vacio
    arr.v = new unsigned char *[arr.capacidad]; // se reserva memoria dinámica para el arreglo de punteros a char (palabras), se utiliza un puntero doble porque es un arreglo que guardará arreglos de caracteres

    for (int i = 0; i < 256; i++) // se inicializa el arreglo de tamaño 256 (cubrir la tabla ascii), este índice guardará la posición donde comienza cada eltra en el arreglo v
    {
        arr.indice[i] = -1; //se inicializa en -1 para indica que no hay ninguna palabra registrada que comience con ese carácter ascii específico
    }
}

void liberarArreglo(arreglo &arr) //evita los memory leaks devolviendo toda la memoria ram solicitada dinámicamente con "new"
{
    for (int i = 0; i < arr.cantidad; i++)
    {
        delete[] arr.v[i]; //primeramente se libera cada palabra individualmente, luego se libera el arreglo de caracteres de la palabra i
    }
    delete[] arr.v; //liberar todo el arreglo principal de punteros
}

int compararStrings(unsigned char *s1, unsigned char *s2) // compara dos cadenadas de caracteres lexicográficamente (orden alfabético), retorna -1 si s1 va antes, 1 si s2 va antes, 0 sin son idénticas
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') //se recorren ambas cadenas hasta que alguna termine, es decir llegue al carácter nulo '\0'
    {
        if (s1[i] != s2[i]) //si encontramos una diferencia en la posicion actual de las cadenas
        {
            if (s1[i] < s2[i])
            {
                return -1; // s1 tiene un valor ascii menor, va antes en el diccionario
            }
            else
            {
                return 1; // s2 tiene un valor ascii menor, va antes en el diccionario
            }
        }
        i++; // si ambas letras son iguales avanza a la siguiente posición
    }
    //si el ciclo termina, significa que almenos una palabra llegó a su fin, si ambas terminaron al mismo tiempo, son la misma palabra exacta
    if(s1[i]=='\0' && s2[i]=='\0') return 0;
    if (s1[i]=='\0') return -1; //si s1 terminó primero, es un prefijo de s2, por ejemplo "mar" vs "marcos", s1 va antes
    return 1; //por descarte s2 es más corta y va antes

}    

void copiarString(unsigned char *destino, unsigned char *origen)//copia manualmente los caracteres de un arreglo a otro, se utiliza, se utiliza para no depender de la librería estándar <cstring>
{
    int i = 0;
    while (origen[i] != '\0')//se copia carácter por carácter hasta encontrar el fin de la cadena original
    {
        destino[i] = origen[i];
        i++;
    }
    destino[i] ='\0';//es vital agregar el carácter nulo al final para cerrar el nuevo string correctamente
}

int largoString(unsigned char *s)// calcula la cantidad de caracteres de una palabra
{
    int len = 0;
    while (s[len] != '\0')//aumenta el contador hasta que llega al fin de la cadena es decir llega a '\0'
    {
        len++;
    }
    return len;
}

void insertarPalabra(arreglo &arr, unsigned char *palabra)//inserta una palabra manteniendo el orden lexicográfico del arreglo, gestionando el crecimiento dinámico (overhead) y actualizando el índice alfabético auxiliar
{
    if (arr.cantidad == arr.capacidad)
    {
        arr.capacidad = arr.capacidad * 2; // si el arreglo se llena, creamos uno más grande con el doble de tamaño
        unsigned char **nuevoV = new unsigned char *[arr.capacidad];//se solicita un nuevo bloque de memoria con el doble de tamaño
        for (int i = 0; i < arr.cantidad; i++)
        {
            nuevoV[i] = arr.v[i]; // se traspasan todos los punteros de las palabras antiguas al nuevo arreglo
        }
        //liberamos el arreglo antiguo y se actualiza el puntero prinicipal
        delete[] arr.v;
        arr.v = nuevoV;
    }
    int i = arr.cantidad - 1;

    //encontrar la posición de la inserción, se mueven todos los elementos mayores hacia la derecha para hacer un espacio
    while (i >= 0 && compararStrings(arr.v[i], palabra) > 0)
    {
        arr.v[i + 1] = arr.v[i]; //desplazamiento a la derecha
        i--;
    }

    
    int largo = largoString(palabra);//se inserta la nueva palabra en el espacio encontrado (posición i + 1)

    arr.v[i + 1] = new unsigned char[largo + 1];//solicitamos memoria exacta para la nueva palabra(+1 para el '\0')
    copiarString(arr.v[i + 1], palabra);
    
    arr.cantidad++;//aumentamos el contador lógico de los elementos

    // Actualiza el índice alfabético
    unsigned char primeraLetra = palabra[0];
    //si es la primera vez que se ve la letra, o si la nueva palabra se insertó antes de la posción que teníamos registrada para esta letra, actualizamos el inicio
    if (arr.indice[primeraLetra] == -1 || arr.indice[primeraLetra] > (i + 1))
    {
        arr.indice[primeraLetra] = i + 1;
    }
    //como desplazamos elementos hacia la derecha, todos los índices de las letras que van después en el abecedario deben correrse una posición también
    for (int j = primeraLetra + 1; j < 256; j++)
    {
        if (arr.indice[j] != -1)
        {
            arr.indice[j]++;
        }
    }
}

bool buscarPalabra(arreglo &arr, unsigned char *palabra)// esta función determina si una palabra existe en al estructura utilizando una búsqueda binaria optimizada acotada por el índice alfabético
{
    if (arr.cantidad == 0)//si no hay palabras, es imposible encontrarla
    {
        return false;
    }
    unsigned char primeraLetra = palabra[0];//obtenemos el rango inicial utilizando nuestro índice de acceso O(1)
    int izq = arr.indice[primeraLetra];
    if (izq == -1)
    {
        return false;// si el índice marca -1, sabemos que no hay palabras con esa letra
    }
    //para buscar el límite derecho del rango, buscamos dónde empieza la siguiente letra registrada, por defecto asumimos que va hasta el final del arreglo
    int der = arr.cantidad - 1;
    for (int i = primeraLetra + 1; i < 256; i++)
    {
        if (arr.indice[i] != -1)
        {
            der = arr.indice[i] - 1;// el rango de nuestra letra termina justo antes de que empiece la siguiente 
            break;
        }
    }

    // búsqueda binaria estándar solo en el subarreglo acotado
    while (izq <= der)
    {   //calculamos el punto medio previniendo el desbordamiento de enteros
        int medio = izq + (der - izq) / 2;
        int cmp = compararStrings(arr.v[medio], palabra);

        if (cmp == 0)//lo encuentra
        {
            return true;
        }

        if (cmp < 0)// la palabra buscada es mayor lexicográficamente, descarta mitad izquierda
        {
            izq = medio + 1;
        }
        else// sino la palabra buscada es menor lexicográficamente, descarta mitad derecha
        {
            der = medio - 1;
        }
    }
    return false; // si el ciclo termina y los punteros se cruza, la palabra no existe
}
