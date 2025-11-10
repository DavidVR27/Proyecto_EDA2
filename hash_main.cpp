#include "Grafo.h"
#include "TablaHash.h"
#include <queue>
#include <climits>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;


void dijkstra(const Grafo& g, int origen, const string& nombreArchivo, const string& tipoVehiculo) {
    int n = g.obtenerNumNodos();
    vector<int> dist(n + 1, INT_MAX);
    vector<int> previo(n + 1, -1);
    dist[origen] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, origen});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& arista : g.obtenerAdyacentes(u)) {
            int v = arista.destino;
            int peso = arista.peso;
            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                previo[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    ofstream archivo(nombreArchivo);
    archivo << "vehiculo,origen,destino,ruta,distancia\n";

    cout << "\n--- RUTAS " << tipoVehiculo << " ---\n";
    for (int destino = 1; destino <= n; destino++) {
        if (destino == origen) continue;
        if (dist[destino] == INT_MAX) continue;

        vector<int> ruta;
        for (int v = destino; v != -1; v = previo[v])
            ruta.push_back(v);
        reverse(ruta.begin(), ruta.end());


        cout << tipoVehiculo << ": ";
        for (size_t i = 0; i < ruta.size(); i++) {
            cout << ruta[i];
            if (i < ruta.size() - 1) cout << " -> ";
        }
        cout << " (Distancia: " << dist[destino] << ")\n";


        archivo << tipoVehiculo << "," << origen << "," << destino << ",\"";
        for (size_t i = 0; i < ruta.size(); i++) {
            archivo << ruta[i];
            if (i < ruta.size() - 1) archivo << "-";
        }
        archivo << "\"," << dist[destino] << "\n";
    }

    archivo.close();
    cout << "Rutas de " << tipoVehiculo << " guardadas en " << nombreArchivo << "\n";
}

int main() {
    Grafo g;

    auto inicioCarga = chrono::high_resolution_clock::now();
    g.cargarDesdeCSV("grafo.csv");
    auto finCarga = chrono::high_resolution_clock::now();
    auto tiempoCarga = chrono::duration_cast<chrono::milliseconds>(finCarga - inicioCarga).count();
    cout << "\nTiempo de carga del grafo: " << tiempoCarga << " ms\n";

    g.mostrarGrafo();


    TablaHash vehiculos;
    vehiculos.insertar("AMB001", "Ambulancia", "Disponible", 5);
    vehiculos.insertar("BOM001", "Bombero", "En misión", 1);
    vehiculos.insertar("PAT001", "Patrulla", "Disponible", 10);

    vehiculos.mostrar();

    // Obtener los vehículos registrados
    Vehiculo* vAmbulancia = vehiculos.buscar("AMB001");
    Vehiculo* vBombero = vehiculos.buscar("BOM001");
    Vehiculo* vPatrulla = vehiculos.buscar("PAT001");

    if (vAmbulancia)
        cout << "\nambulancia " << vAmbulancia->tipo << " ubicada en nodo "
             << vAmbulancia->nodoActual << " lista para cálculo de rutas.\n";
    if (vBombero)
        cout << "bombero " << vBombero->tipo << " ubicada en nodo "
             << vBombero->nodoActual << endl;
    if (vPatrulla)
        cout << "patrulla " << vPatrulla->tipo << " ubicada en nodo "
             << vPatrulla->nodoActual << endl;


    auto inicioDijkstra = chrono::high_resolution_clock::now();

    if (vBombero)
        dijkstra(g, vBombero->nodoActual, "rutas_bomberos.csv", vBombero->tipo);
    if (vAmbulancia)
        dijkstra(g, vAmbulancia->nodoActual, "rutas_ambulancia.csv", vAmbulancia->tipo);
    if (vPatrulla)
        dijkstra(g, vPatrulla->nodoActual, "rutas_patrulla.csv", vPatrulla->tipo);

    auto finDijkstra = chrono::high_resolution_clock::now();
    auto tiempoDijkstra = chrono::duration_cast<chrono::milliseconds>(finDijkstra - inicioDijkstra).count();

    cout << "\nTiempo total de Dijkstra: " << tiempoDijkstra << " ms\n";
    cout << "Cálculo de rutas completado para todos los vehículos.\n";

    return 0;
}

