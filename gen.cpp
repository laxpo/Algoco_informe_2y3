#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ALPHABET_SIZE = 26;

// Función para generar valores aleatorios en un archivo
void generar_valores_aleatorios(const string& nombre_archivo, int filas, int columnas) {
    ofstream file(nombre_archivo);
    if (!file) {
        cerr << "Error al abrir el archivo " << nombre_archivo << endl;
        return;
    }
    
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            int valor = rand() % 10 + 1;  // Genera un valor entre 1 y 10
            file << valor;
            if (j < columnas - 1) {
                file << " ";
            }
        }
        file << endl;
    }
    file.close();
    cout << "Archivo " << nombre_archivo << " generado." << endl;
}

int main() {
    srand(time(0));  // Inicializar la semilla para valores aleatorios

    // Generar el archivo de costos de inserción (1 fila, 26 columnas)
    generar_valores_aleatorios("cost_insert.txt", 1, ALPHABET_SIZE);

    // Generar el archivo de costos de eliminación (1 fila, 26 columnas)
    generar_valores_aleatorios("cost_delete.txt", 1, ALPHABET_SIZE);

    // Generar el archivo de costos de sustitución (26 filas, 26 columnas)
    generar_valores_aleatorios("cost_replace.txt", ALPHABET_SIZE, ALPHABET_SIZE);

    // Generar el archivo de costos de transposición (26 filas, 26 columnas)
    generar_valores_aleatorios("cost_transpose.txt", ALPHABET_SIZE, ALPHABET_SIZE);

    return 0;
}
