#ifndef SOLUCION1_HPP
#define SOLUCION1_HPP

struct arreglo{
    unsigned char** v; //arreglo de punteros a las palabras
    int capacidad; //capacidad total actual
    int cantidad; //Cúantas palabras hay guardadas en el arreglo
    int indice[256]; //para saber dónde empieza cada letra
};
//funciones que manipulan el arreglo
void inicializarArreglo(arreglo &arr, int capacidadInicial);
void liberarArreglo(arreglo &arr);
int compararStrings(unsigned char * s1, unsigned char* s2);
void insertarPalabra(arreglo &arr, unsigned char * palabra);
bool buscarPalabra(arreglo &arr, unsigned char* palabra);

#endif