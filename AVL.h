#ifndef AVL_H
#define AVL_H

#include <utility>
using namespace std;

struct NodoAVL {
    int key, count, height;
    NodoAVL *left, *right;
    NodoAVL(int k, int c=1): key(k), count(c), height(1), left(nullptr), right(nullptr) {}
};

class AVL {
    NodoAVL* root = nullptr;
    NodoAVL* maxPtr = nullptr; 

    static int h(NodoAVL* n){ return n? n->height:0; }
    static int max2(int a, int b){ return a>b? a:b; }
    static void upd(NodoAVL* n){ n->height = 1 + max2(h(n->left), h(n->right)); }
    static int bf(NodoAVL* n){ return n? h(n->left)-h(n->right):0; }

    static NodoAVL* rotL(NodoAVL* x){
        NodoAVL* y = x->right; x->right = y->left; y->left = x; upd(x); upd(y); return y;
    }
    static NodoAVL* rotR(NodoAVL* y){
        NodoAVL* x = y->left;  y->left  = x->right; x->right = y; upd(y); upd(x); return x;
    }
    static NodoAVL* balance(NodoAVL* n){
        upd(n);
        int b = bf(n);
        if (b > 1) { if (bf(n->left) < 0) n->left = rotL(n->left); return rotR(n); }
        if (b < -1){ if (bf(n->right)> 0) n->right= rotR(n->right); return rotL(n); }
        return n;
    }

    void touchMax(NodoAVL* n){ if (!maxPtr || n->count > maxPtr->count) maxPtr = n; }

    NodoAVL* _insert(NodoAVL* n, int k, int delta){
        if(!n){ NodoAVL* nn = new NodoAVL(k, delta); touchMax(nn); return nn; }
        if(k < n->key) n->left  = _insert(n->left,  k, delta);
        else if(k > n->key) n->right = _insert(n->right, k, delta);
        else { n->count += delta; touchMax(n); return n; }
        return balance(n);
    }

    int _buscar(NodoAVL* n, int k) const {
        if(!n) return 0;
        if(k==n->key) return n->count;
        return k<n->key ? _buscar(n->left,k) : _buscar(n->right,k);
    }

    void _dump(NodoAVL* n, int* keys, int* counts, int& idx, int cap) const {
        if(!n || idx>=cap) return;
        _dump(n->left, keys, counts, idx, cap);
        if(idx<cap){ keys[idx]=n->key; counts[idx]=n->count; idx++; }
        _dump(n->right, keys, counts, idx, cap);
    }

    int _size(NodoAVL* n) const {
        if(!n) return 0;
        return 1 + _size(n->left) + _size(n->right);
    }

public:
    void insertar(int zonaId, int delta=1){ root = _insert(root, zonaId, delta); }
    int  buscar(int zonaId) const { return _buscar(root, zonaId); }
    pair<int,int> maximo() const {
        if(!maxPtr) return {-1,0};
        return {maxPtr->key, maxPtr->count};
    }
    int size() const { return _size(root); }

    // Vuelca el arbol en arreglos (para luego ordenar en main si se desea topK)
    // Retorna el número de elementos copiados (hasta cap).
    int dump(int* keys, int* counts, int cap) const {
        int idx=0; _dump(root, keys, counts, idx, cap); return idx;
    }
};
#endif
