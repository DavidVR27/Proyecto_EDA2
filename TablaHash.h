#ifndef TablaHash.h
#define TablaHash.h

#include <iostream>
#include <string>
using namespace std;

struct Vehiculo {
    string id;
    string tipo;
    string estado;
    int nodoActual;
    Vehiculo* siguiente;
};

class TablaHash {
private:
    int capacidad;
    int elementos;
    Vehiculo** tabla;

    // Función hash simple (suma ponderada de caracteres)
    int funcionHash(const string& clave) const {
        unsigned long hash = 0;
        for (char c : clave)
            hash = (hash * 31 + c) % capacidad;
        return hash;
    }

    float factorCarga() const {
        return (float)elementos / capacidad;
    }

    // Rehash: duplica el tamaño de la tabla y reubica elementos
    void rehash() {
        int viejaCap = capacidad;
        capacidad *= 2;

        Vehiculo** nuevaTabla = new Vehiculo*[capacidad];
        for (int i = 0; i < capacidad; i++)
            nuevaTabla[i] = nullptr;

        for (int i = 0; i < viejaCap; i++) {
            Vehiculo* actual = tabla[i];
            while (actual) {
                Vehiculo* siguiente = actual->siguiente;
                int nuevoIndice = funcionHash(actual->id);
                actual->siguiente = nuevaTabla[nuevoIndice];
                nuevaTabla[nuevoIndice] = actual;
                actual = siguiente;
            }
        }

        delete[] tabla;
        tabla = nuevaTabla;
        cout << "Rehashing completado. Nueva capacidad: " << capacidad << endl;
    }

public:
    TablaHash(int cap = 10) {
        capacidad = cap;
        elementos = 0;
        tabla = new Vehiculo*[capacidad];
        for (int i = 0; i < capacidad; i++)
            tabla[i] = nullptr;
    }

    // Inserta un nuevo vehículo
    void insertar(const string& id, const string& tipo, const string& estado, int nodo) {
        if (factorCarga() > 0.7)
            rehash();

        int indice = funcionHash(id);
        Vehiculo* nuevo = new Vehiculo{id, tipo, estado, nodo, nullptr};
        nuevo->siguiente = tabla[indice];
        tabla[indice] = nuevo;
        elementos++;
    }

    // Busca un vehículo por ID
    Vehiculo* buscar(const string& id) const {
        int indice = funcionHash(id);
        Vehiculo* temp = tabla[indice];
        while (temp) {
            if (temp->id == id)
                return temp;
            temp = temp->siguiente;
        }
        return nullptr;
    }

    // Mostrar todos los vehículos
    void mostrar() const {
        cout << "\n--- REGISTRO DE VEHÍCULOS ---\n";
        for (int i = 0; i < capacidad; i++) {
            cout << "[" << i << "]";
            Vehiculo* temp = tabla[i];
            while (temp) {
                cout << " -> (" << temp->id << ", " << temp->tipo
                     << ", " << temp->estado << ", nodo " << temp->nodoActual << ")";
                temp = temp->siguiente;
            }
            cout << endl;
        }
        cout << "Capacidad: " << capacidad
             << " | Elementos: " << elementos
             << " | Factor de carga: " << factorCarga() << endl;
    }
};
#endif
