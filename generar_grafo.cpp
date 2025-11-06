#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
using namespace std;

int main() {
    srand(time(0)); // inicializa el generador aleatorio

    const int N = 1000;   // cantidad de nodos
    const int CONEXIONES_MIN = 2;  // conexiones mínimas por nodo
    const int CONEXIONES_MAX = 5;  // conexiones máximas por nodo

    ofstream archivo("grafo.csv");
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo grafo.csv" << endl;
        return 1;
    }

    archivo << "origen,destino,peso\n"; // cabecera

    // Para cada nodo, generamos entre 2 y 5 conexiones aleatorias
    for (int i = 1; i <= N; i++) {
        int numConexiones = CONEXIONES_MIN + rand() % (CONEXIONES_MAX - CONEXIONES_MIN + 1);
        set<int> destinosUsados; // evitar duplicar destinos

        for (int j = 0; j < numConexiones; j++) {
            int destino;
            do {
                destino = rand() % N + 1;
            } while (destino == i || destinosUsados.count(destino));
            destinosUsados.insert(destino);

            int peso = 50 + rand() % 951; // peso aleatorio entre 50 y 1000
            archivo << i << "," << destino << "," << peso << "\n";
        }
    }

    archivo.close();
    cout << "Archivo grafo.csv generado correctamente con " << N << " nodos." << endl;
    return 0;
}
