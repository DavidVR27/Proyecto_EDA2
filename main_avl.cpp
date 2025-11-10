#include "Grafo.h"
#include <queue>
#include <climits>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;
#include "AVL.h"
#include <string>

// --- Registro de zonas atendidas con AVL ---
static void registrarDesdeCSV(const string& archivo, AVL& arbol) {
    ifstream in(archivo);
    if (!in.is_open()) {
        cerr << "[WARN] No se pudo abrir " << archivo << "\n";
        return;
    }
    string linea;
    while (getline(in, linea)) {
        if (linea.empty()) continue;
        // Espera formato "origen,destino,distancia" (ajustar si difiere)
        size_t c1 = linea.find(',');
        size_t c2 = (c1==string::npos)? string::npos : linea.find(',', c1+1);
        if (c1==string::npos || c2==string::npos) continue;
        int destino = stoi(linea.substr(c1+1, c2 - (c1+1)));
        arbol.insertar(destino, 1);
    }
}

static void exportarFrecuenciasCSV(const string& archivo, const AVL& arbol) {
    // limita a 100000 zonas para volcados grandes
    const int CAP = 100000;
    int* keys = new int[CAP];
    int* counts = new int[CAP];
    int n = arbol.dump(keys, counts, CAP);

    // ordenar por conteo descendente (burbuja simple para evitar std::sort si no se permite)
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){
            if (counts[j] > counts[i]) {
                int tc = counts[i]; counts[i]=counts[j]; counts[j]=tc;
                int tk = keys[i]; keys[i]=keys[j]; keys[j]=tk;
            }
        }
    }

    ofstream out(archivo);
    if (out.is_open()) {
        out << "zona,conteo\n";
        for (int i=0;i<n;i++) out << keys[i] << "," << counts[i] << "\n";
        out.close();
    }
    delete[] keys; delete[] counts;
}


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

    // Crear archivo CSV para las rutas
    ofstream archivo(nombreArchivo);
    archivo << "vehiculo,origen,destino,ruta,distancia\n";

    cout << "\n--- RUTAS " << tipoVehiculo << " ---\n";
    for (int destino = 1; destino <= n; destino++) {
        if (destino == origen) continue;
        if (dist[destino] == INT_MAX) continue; // Sin conexión

        vector<int> ruta;
        for (int v = destino; v != -1; v = previo[v])
            ruta.push_back(v);
        reverse(ruta.begin(), ruta.end());

        // Mostrar en consola
        cout << tipoVehiculo << ": ";
        for (size_t i = 0; i < ruta.size(); i++) {
            cout << ruta[i];
            if (i < ruta.size() - 1) cout << " -> ";
        }
        cout << " (Distancia: " << dist[destino] << ")\n";

        // Guardar en CSV
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

// ========================================================
// MAIN: Carga grafo y ejecuta Dijkstra para los 3 vehículos
// ========================================================
int main() {
    Grafo g;

    // Tiempo de carga del grafo
    auto inicioCarga = chrono::high_resolution_clock::now();
    g.cargarDesdeCSV("grafo.csv");
    auto finCarga = chrono::high_resolution_clock::now();
    auto tiempoCarga = chrono::duration_cast<chrono::milliseconds>(finCarga - inicioCarga).count();
    cout << "\nTiempo de carga del grafo: " << tiempoCarga << " ms\n";

    g.mostrarGrafo();

    // Nodos de depósito
    int depositoBomberos = 1;
    int depositoAmbulancia = 5;
    int depositoPatrulla = 10;

    // Medir tiempo de ejecución de Dijkstra
    auto inicioDijkstra = chrono::high_resolution_clock::now();

    dijkstra(g, depositoBomberos, "rutas_bomberos.csv", "Bomberos");
    dijkstra(g, depositoAmbulancia, "rutas_ambulancia.csv", "Ambulancia");
    dijkstra(g, depositoPatrulla, "rutas_patrulla.csv", "Patrulla");

    auto finDijkstra = chrono::high_resolution_clock::now();
    auto tiempoDijkstra = chrono::duration_cast<chrono::milliseconds>(finDijkstra - inicioDijkstra).count();

    cout << "\nTiempo total de Dijkstra: " << tiempoDijkstra << " ms\n";
    cout << "Cálculo de rutas completado para todos los vehículos.\n";

    
    // === Construir árbol de frecuencias de zonas atendidas ===
    AVL zonas;
    registrarDesdeCSV("rutas_bomberos.csv", zonas);
    registrarDesdeCSV("rutas_ambulancia.csv", zonas);
    registrarDesdeCSV("rutas_patrulla.csv", zonas);

    auto top = zonas.maximo();
    if (top.first != -1) {
        cout << "\nZona con mayor número de atenciones: " << top.first
             << " (" << top.second << " eventos)\n";
    } else {
        cout << "\nNo se registraron atenciones.\n";
    }

    // Exporta todas las frecuencias para el dashboard/informe
    exportarFrecuenciasCSV("frecuencias_zonas.csv", zonas);
    cout << "Frecuencias por zona exportadas a frecuencias_zonas.csv\n";
return 0;
}
