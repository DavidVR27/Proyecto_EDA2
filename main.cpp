#include <iostream>
#include <chrono>
#include "Grafo.h"

using namespace std;
using namespace std::chrono;

int main() {
    Grafo g;

    // Medir tiempo de carga
    auto inicio = high_resolution_clock::now();
    g.cargarDesdeCSV("grafo.csv");
    auto fin = high_resolution_clock::now();

    auto duracion = duration_cast<milliseconds>(fin - inicio).count();
    cout << "\nTiempo de carga del grafo: " << duracion << " ms" << endl;

    cout << "\n--- Grafo cargado ---\n";
    g.mostrarGrafo();

    // Análisis del grafo
    int totalAristas = 0;
    for (int i = 1; i <= g.obtenerNumNodos(); i++)
        totalAristas += g.obtenerAdyacentes(i).size();

    double promedio = (double)totalAristas / g.obtenerNumNodos();

    cout << "\n Numero total de aristas: " << totalAristas << endl;
    cout << "Promedio de conexiones por nodo: " << promedio << endl;

    return 0;
}
