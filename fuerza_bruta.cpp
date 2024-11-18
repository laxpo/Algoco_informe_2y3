#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace chrono;

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

// Algoritmo de fuerza bruta para distancia mínima de edición extendida
int distancia_minima(const string& S1, const string& S2, int i, int j, vector<string>& operaciones) {
    if (i == S1.size()) {
        int costo = 0;
        for (int k = j; k < S2.size(); ++k) {
            costo += costo_ins(S2[k]);
            operaciones.push_back("Insertar " + string(1, S2[k]));
        }
        return costo;
    }
    if (j == S2.size()) {
        int costo = 0;
        for (int k = i; k < S1.size(); ++k) {
            costo += costo_del(S1[k]);
            operaciones.push_back("Eliminar " + string(1, S1[k]));
        }
        return costo;
    }

    // Opciones para sustitución, inserción, eliminación y transposición
    int costo_sust = INT_MAX, costo_ins_op = INT_MAX, costo_del_op = INT_MAX, costo_trans_op = INT_MAX;
    vector<string> oper_sust, oper_ins, oper_del, oper_trans;

    // Sustitución
    oper_sust = operaciones;
    if (S1[i] != S2[j]) {
        oper_sust.push_back("Sustituir " + string(1, S1[i]) + " por " + string(1, S2[j]));
        costo_sust = costo_sub(S1[i], S2[j]) + distancia_minima(S1, S2, i + 1, j + 1, oper_sust);
    } else {
        costo_sust = distancia_minima(S1, S2, i + 1, j + 1, oper_sust);
    }

    // Inserción
    oper_ins = operaciones;
    oper_ins.push_back("Insertar " + string(1, S2[j]));
    costo_ins_op = costo_ins(S2[j]) + distancia_minima(S1, S2, i, j + 1, oper_ins);

    // Eliminación
    oper_del = operaciones;
    oper_del.push_back("Eliminar " + string(1, S1[i]));
    costo_del_op = costo_del(S1[i]) + distancia_minima(S1, S2, i + 1, j, oper_del);

    // Transposición (si aplica)
    if (i + 1 < S1.size() && j + 1 < S2.size() && S1[i] == S2[j + 1] && S1[i + 1] == S2[j]) {
        oper_trans = operaciones;
        oper_trans.push_back("Transponer " + string(1, S1[i]) + " y " + string(1, S1[i + 1]));
        costo_trans_op = costo_trans(S1[i], S1[i + 1]) + distancia_minima(S1, S2, i + 2, j + 2, oper_trans);
    }

    // Elegir la operación de menor costo
    int min_costo = min({costo_sust, costo_ins_op, costo_del_op, costo_trans_op});
    if (min_costo == costo_sust) {
        operaciones = oper_sust;
    } else if (min_costo == costo_ins_op) {
        operaciones = oper_ins;
    } else if (min_costo == costo_del_op) {
        operaciones = oper_del;
    } else {
        operaciones = oper_trans;
    }

    return min_costo;
}

int main() {
    cargar_costos();

    string S1, S2;
    cout << "Ingrese la primera cadena: ";
    getline(cin, S1);
    cout << "Ingrese la segunda cadena: ";
    getline(cin, S2);

    // Medir el tiempo de ejecución
    auto start = high_resolution_clock::now();

    vector<string> operaciones;
    int distancia = distancia_minima(S1, S2, 0, 0, operaciones);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Distancia mínima de edición (costo): " << distancia << endl;
    cout << "Operaciones realizadas:" << endl;
    for (const auto& operacion : operaciones) {
        cout << "- " << operacion << endl;
    }

    cout << "Tiempo de ejecución: " << duration.count() << " microsegundos" << endl;

    return 0;
}
