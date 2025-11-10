#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

#define INF 9999999
#define MAX 1000 


// Estructura para las aristas
struct Arista {
    int destino;
    int peso;
    Arista* siguiente;
};

// Grafo con lista de adyacencia
struct Grafo {
    Arista* lista[MAX];
    int numNodos;
};

void inicializarGrafo(Grafo &g, int n) {
    g.numNodos = n;
    for (int i = 0; i < n; i++)
        g.lista[i] = nullptr;
}

void agregarArista(Grafo &g, int origen, int destino, int peso) {
    Arista* nuevo = new Arista;
    nuevo->destino = destino;
    nuevo->peso = peso;
    nuevo->siguiente = g.lista[origen];
    g.lista[origen] = nuevo;
}

// Nodo y estructura del Min Heap
struct NodoHeap {
    int vertice;
    int distancia;
};

struct MinHeap {
    NodoHeap* arr;
    int capacidad;
    int tamano;
    int* posicion;

    MinHeap(int cap) {
        capacidad = cap;
        tamano = 0;
        arr = new NodoHeap[cap];
        posicion = new int[cap];
        for (int i = 0; i < cap; i++)
            posicion[i] = -1;   // inicialización segura
    }

    ~MinHeap() {
        delete[] arr;
        delete[] posicion;
    }

    int padre(int i) { return (i - 1) / 2; }
    int izq(int i) { return 2 * i + 1; }
    int der(int i) { return 2 * i + 2; }

    void intercambiar(int i, int j) {
        NodoHeap temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;

        posicion[arr[i].vertice] = i;
        posicion[arr[j].vertice] = j;
    }

    // Insertar nuevo nodo en el heap
    void insertar(NodoHeap nodo) {
        int i = tamano++;
        arr[i] = nodo;
        posicion[nodo.vertice] = i;

        while (i > 0 && arr[padre(i)].distancia > arr[i].distancia) {
            intercambiar(i, padre(i));
            i = padre(i);
        }
    }

    // Restablecer orden hacia abajo
    void heapifyDown(int i) {
        int menor = i;
        int l = izq(i);
        int r = der(i);

        if (l < tamano && arr[l].distancia < arr[menor].distancia)
            menor = l;
        if (r < tamano && arr[r].distancia < arr[menor].distancia)
            menor = r;

        if (menor != i) {
            intercambiar(i, menor);
            heapifyDown(menor);
        }
    }

    // Extraer el nodo mínimo (distancia menor)
    NodoHeap extraerMin() {
        if (tamano <= 0)
            return {-1, INF};
        if (tamano == 1) {
            tamano--;
            return arr[0];
        }

        NodoHeap raiz = arr[0];
        arr[0] = arr[tamano - 1];
        posicion[arr[0].vertice] = 0;

        // Marcar vértice extraído como inválido
        posicion[raiz.vertice] = -1;

        tamano--;
        heapifyDown(0);
        return raiz;
    }

    // Actualizar valor (decreaseKey)
    void actualizarValor(int vertice, int nuevaDistancia) {
        int i = posicion[vertice];

        // Evita error si el vértice ya no está en el heap
        if (i < 0 || i >= tamano)
            return;

        arr[i].distancia = nuevaDistancia;

        while (i > 0 && arr[i].distancia < arr[padre(i)].distancia) {
            intercambiar(i, padre(i));
            i = padre(i);
        }
    }

    bool vacio() { return tamano == 0; }
};

// Algoritmo de Dijkstra
void dijkstra(Grafo &g, int inicio) {
    int dist[MAX];
    bool visitado[MAX];

    for (int i = 0; i < g.numNodos; i++) {
        dist[i] = INF;
        visitado[i] = false;
    }

    MinHeap heap(g.numNodos);
    dist[inicio] = 0;

    for (int i = 0; i < g.numNodos; i++)
        heap.insertar({i, dist[i]});

    while (!heap.vacio()) {
        NodoHeap minNodo = heap.extraerMin();
        int u = minNodo.vertice;

        if (u == -1 || visitado[u])
            continue;

        visitado[u] = true;

        // recorrer adyacentes
        Arista* actual = g.lista[u];
        while (actual != nullptr) {
            int v = actual->destino;
            int peso = actual->peso;

            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                heap.actualizarValor(v, dist[v]);
            }
            actual = actual->siguiente;
        }
    }

    cout << "Distancias mínimas desde el nodo " << inicio << ":\n";
    for (int i = 0; i < g.numNodos; i++) {
        cout << "Nodo " << i << " -> ";
        if (dist[i] == INF) cout << "INF";
        else cout << dist[i];
        cout << endl;
    }
}

// Lectura del archivo CSV
void leerCSV(Grafo &g, const string& archivo) {
    ifstream file(archivo);
    string linea;

    if (!file.is_open()) {
        cerr << "Error al abrir " << archivo << endl;
        return;
    }

    getline(file, linea); // saltar encabezado

    int origen, destino, peso;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string s1, s2, s3;
        getline(ss, s1, ',');
        getline(ss, s2, ',');
        getline(ss, s3, ',');

        origen = stoi(s1);
        destino = stoi(s2);
        peso = stoi(s3);

        agregarArista(g, origen, destino, peso);
        agregarArista(g, destino, origen, peso); // no dirigido
    }
    file.close();
}

// Programa principal
int main() {
    Grafo g;
    inicializarGrafo(g, 10);   // número de nodos del CSV (puedes ajustar)

    leerCSV(g, "grafo.csv");

    cout << "Algoritmo de Dijkstra con Min Heap manual\n";
    dijkstra(g, 0);   // comienza desde el nodo 0

    return 0;
}

