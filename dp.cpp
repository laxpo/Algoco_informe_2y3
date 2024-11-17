#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int ALPHABET_SIZE = 26;
vector<int> cost_insert(ALPHABET_SIZE);
vector<int> cost_delete(ALPHABET_SIZE);
vector<vector<int>> cost_replace(ALPHABET_SIZE, vector<int>(ALPHABET_SIZE));
vector<vector<int>> cost_transpose(ALPHABET_SIZE, vector<int>(ALPHABET_SIZE));

// Función para cargar los costos desde archivos
void cargar_costos() {
    ifstream file;

    // Leer costos de inserción
    file.open("cost_insert.txt");
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        file >> cost_insert[i];
    }
    file.close();

    // Leer costos de eliminación
    file.open("cost_delete.txt");
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        file >> cost_delete[i];
    }
    file.close();

    // Leer costos de sustitución
    file.open("cost_replace.txt");
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            file >> cost_replace[i][j];
        }
    }
    file.close();

    // Leer costos de transposición
    file.open("cost_transpose.txt");
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            file >> cost_transpose[i][j];
        }
    }
    file.close();
}

// Funciones de costo utilizando los vectores y matrices globales
int costo_sub(char a, char b) {
    return cost_replace[a - 'a'][b - 'a'];
}

int costo_ins(char b) {
    return cost_insert[b - 'a'];
}

int costo_del(char a) {
    return cost_delete[a - 'a'];
}

int costo_trans(char a, char b) {
    return cost_transpose[a - 'a'][b - 'a'];
}

// Algoritmo de programación dinámica bottom-up para la distancia mínima de edición extendida
int distancia_minima_bottom_up(const string& S1, const string& S2, vector<string>& operaciones) {
    int n = S1.size();
    int m = S2.size();

    // Crear la matriz dp de tamaño (n+1) x (m+1)
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    vector<vector<string>> movimientos(n + 1, vector<string>(m + 1, ""));

    // Inicializar los casos base
    for (int i = 1; i <= n; ++i) {
        dp[i][0] = dp[i - 1][0] + costo_del(S1[i - 1]);
        movimientos[i][0] = "Eliminar " + string(1, S1[i - 1]);
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = dp[0][j - 1] + costo_ins(S2[j - 1]);
        movimientos[0][j] = "Insertar " + string(1, S2[j - 1]);
    }

    // Llenar la matriz dp de manera bottom-up
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int costo_sustitucion = (S1[i - 1] == S2[j - 1]) ? dp[i - 1][j - 1] : dp[i - 1][j - 1] + costo_sub(S1[i - 1], S2[j - 1]);
            int costo_insercion = dp[i][j - 1] + costo_ins(S2[j - 1]);
            int costo_eliminacion = dp[i - 1][j] + costo_del(S1[i - 1]);

            // Considerar transposición si es aplicable
            int costo_transposicion = INT_MAX;
            if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
                costo_transposicion = dp[i - 2][j - 2] + costo_trans(S1[i - 2], S1[i - 1]);
            }

            // Seleccionar el costo mínimo entre todas las opciones y guardar el movimiento correspondiente
            if (costo_sustitucion <= costo_insercion && costo_sustitucion <= costo_eliminacion && costo_sustitucion <= costo_transposicion) {
                dp[i][j] = costo_sustitucion;
                movimientos[i][j] = (S1[i - 1] == S2[j - 1]) ? "Coincidir" : "Sustituir " + string(1, S1[i - 1]) + " por " + string(1, S2[j - 1]);
            } else if (costo_insercion <= costo_eliminacion && costo_insercion <= costo_transposicion) {
                dp[i][j] = costo_insercion;
                movimientos[i][j] = "Insertar " + string(1, S2[j - 1]);
            } else if (costo_eliminacion <= costo_transposicion) {
                dp[i][j] = costo_eliminacion;
                movimientos[i][j] = "Eliminar " + string(1, S1[i - 1]);
            } else {
                dp[i][j] = costo_transposicion;
                movimientos[i][j] = "Transponer " + string(1, S1[i - 2]) + " y " + string(1, S1[i - 1]);
            }
        }
    }

    // Reconstruir los movimientos desde dp[n][m] hacia atrás
    int i = n, j = m;
    while (i > 0 || j > 0) {
        operaciones.push_back(movimientos[i][j]);
        if (movimientos[i][j].find("Sustituir") == 0 || movimientos[i][j] == "Coincidir") {
            i--; j--;
        } else if (movimientos[i][j].find("Insertar") == 0) {
            j--;
        } else if (movimientos[i][j].find("Eliminar") == 0) {
            i--;
        } else {  // Transponer
            i -= 2; j -= 2;
        }
    }
    reverse(operaciones.begin(), operaciones.end());
    return dp[n][m];
}

int main() {
    cargar_costos();

    string S1, S2;
    cout << "Ingrese la primera cadena: ";
    getline(cin, S1);
    cout << "Ingrese la segunda cadena: ";
    getline(cin, S2);

    vector<string> operaciones;
    auto start = high_resolution_clock::now();

    int distancia = distancia_minima_bottom_up(S1, S2, operaciones);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Distancia mínima de edición (costo): " << distancia << endl;
    cout << "Operaciones realizadas:" << endl;
    for (const auto& operacion : operaciones) {
        cout << "- " << operacion << endl;
    }
    cout << "Tiempo de ejecución: " << duration.count() << " ms" << endl;

    return 0;
}
