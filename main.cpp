#include <iostream>
#include <fstream>
#include <ctime> 
#include "Solucion1.hpp"

using namespace std;

int main() {
    arreglo miEstructura;
    inicializarArreglo(miEstructura, 100); // espacio inicial de 100
    
    ifstream archivoD1("D1.txt");
    if (!archivoD1.is_open()) {
        cout << "No se pudo abrir el archivo D1.txt" << endl;
        return 1;
    }

    char buffer[256]; // Arreglo temporal para leer cada línea
    
    cout << "Cargando palabras en el arreglo" << endl;
    
    // medir el tiempo
    clock_t inicio = clock();
    
    while (archivoD1 >> buffer) {
        // castear la tarea a unsigned char*
        unsigned char* palabraCasteada = (unsigned char*)buffer;
        insertarPalabra(miEstructura, palabraCasteada);
    }
    
    clock_t fin = clock();
    double tiempoTotal = double(fin - inicio) / CLOCKS_PER_SEC;
    
    archivoD1.close();

    cout << "Palabras guardadas: " << miEstructura.cantidad << endl;
    cout << "Tiempo de construccion: " << tiempoTotal << " segundos." << endl;

    // Probar la búsqueda
    unsigned char testPalabra[] = "abandon"; 
    if (buscarPalabra(miEstructura, testPalabra)) {
        cout << "Palabra encontrada" << endl;
    } else {
        cout << "Palabra no encontrada" << endl;
    }

    liberarArreglo(miEstructura);
    return 0;
}