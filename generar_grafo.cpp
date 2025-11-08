#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
using namespace std;

int main() {
    srand(time(0)); // Inicializa el generador aleatorio

    const int N = 1000;   // Cantidad de nodos
    const int CONEXIONES_MIN = 2;  // Conexiones mínimas por nodo
    const int CONEXIONES_MAX = 5;  // Conexiones máximas por nodo

    ofstream archivo("grafo.csv");
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo grafo.csv" << endl;
        return 1;
    }

    archivo << "origen,destino,peso\n"; // Cabecera del CSV

    // Para cada nodo, generamos entre 2 y 5 conexiones aleatorias
    for (int i = 1; i <= N; i++) {
        int numConexiones = CONEXIONES_MIN + rand() % (CONEXIONES_MAX - CONEXIONES_MIN + 1);
        set<int> destinosUsados; // Evita duplicar destinos para el mismo nodo

        for (int j = 0; j < numConexiones; j++) {
            int destino;
            do {
                destino = rand() % N + 1;
            } while (destino == i || destinosUsados.count(destino)); // Evita bucles y duplicados

            destinosUsados.insert(destino);
            int peso = 50 + rand() % 951; // Peso aleatorio entre 50 y 1000

            // Grafo no orientado
            archivo << i << "," << destino << "," << peso << "\n";
            archivo << destino << "," << i << "," << peso << "\n";
        }
    }

    archivo.close();
    cout << "Archivo grafo.csv generado correctamente con " << N << " nodos (grafo no orientado)" << endl;
    return 0;
}
