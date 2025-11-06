#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Estructura que representa una arista: destino y peso (distancia)
struct Arista {
    int destino;
    int peso;
};

// Clase que representa el grafo mediante listas de adyacencia
class Grafo {
private:
    int numNodos;                             // número total de nodos
    vector<vector<Arista>> adyacencia;        // lista de adyacencia

public:
    Grafo(int n = 0) {
        numNodos = n;
        adyacencia.resize(n + 1); // +1 si los nodos empiezan en 1
    }

    // Agregar una arista al grafo (dirección origen -> destino)
    void agregarArista(int origen, int destino, int peso) {
        if (origen <= 0 || destino <= 0) return;
        if (adyacencia.size() <= max(origen, destino))
            adyacencia.resize(max(origen, destino) + 1);
        adyacencia[origen].push_back({destino, peso});
    }

    // Leer un grafo desde un archivo CSV
    void cargarDesdeCSV(const string& archivoCSV) {
        ifstream archivo(archivoCSV);
        if (!archivo.is_open()) {
            cout << "❌ Error al abrir el archivo CSV." << endl;
            return;
        }

        string linea;
        getline(archivo, linea); // Saltar cabecera

        int maxNodo = 0;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            // Detecta si usa ',' o ';'
            char separador = (linea.find(';') != string::npos) ? ';' : ',';
            stringstream ss(linea);
            string origenStr, destinoStr, pesoStr;
            getline(ss, origenStr, separador);
            getline(ss, destinoStr, separador);
            getline(ss, pesoStr, separador);

            if (origenStr.empty() || destinoStr.empty() || pesoStr.empty()) continue;

            int origen = stoi(origenStr);
            int destino = stoi(destinoStr);
            int peso = stoi(pesoStr);

            agregarArista(origen, destino, peso);

            if (origen > maxNodo) maxNodo = origen;
            if (destino > maxNodo) maxNodo = destino;
        }

        archivo.close();
        numNodos = adyacencia.size() - 1;
        cout << "✅ Grafo cargado correctamente con " << numNodos << " nodos.\n";
    }

    // Mostrar todo el grafo (para pruebas)
    void mostrarGrafo() const {
        int mostrados = 0;
        for (int i = 1; i <= numNodos; i++) {
            if (adyacencia[i].empty()) continue; // no mostrar nodos sin conexiones
            cout << "Nodo " << i << " -> ";
            for (auto& arista : adyacencia[i]) {
                cout << "(" << arista.destino << ", " << arista.peso << ") ";
            }
            cout << endl;
            mostrados++;
            if (mostrados >= 20) { // mostrar solo 20 primeros para no saturar
                cout << "...\n(Mostrando solo los primeros 20 nodos)\n";
                break;
            }
        }
    }

    // Obtener los adyacentes de un nodo (para Dijkstra)
    const vector<Arista>& obtenerAdyacentes(int nodo) const {
        return adyacencia[nodo];
    }

    int obtenerNumNodos() const {
        return numNodos;
    }
};

#endif
