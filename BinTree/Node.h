// By Fabio Nardelli
// classe usata per rappresentare un nodo di un albero binario

#ifndef NODE_H
#define NODE_H

template <typename T>
class BinTree;

template <typename T>
class Node {
    friend class BinTree<T>;
public:
    // costruttore di default
    Node() : parent(0), left(0), right(0) { }

    // costruttore con argomenti: prende in input il dato da
    // inserire nel nodo, e (eventualmente) i valori dei puntatori
    // parent, left e right, di default impostati a 0. Ad esempio,
    // e' utile per impostare il parent direttamente alla creazione
    // del nodo, evitando cosi' una "doppia" assegnazione
    // (in realta' inizializzazione + assegnazione)
    Node(T item, Node<T> *p = 0, Node<T> *l = 0, Node<T> *r = 0)
        : value(item), parent(p), left(l), right(r) { }

    // funzioni per ottenere l'etichetta contenuta nel nodo, il
    // figlio sinistro, il figlio destro e il genitore
    T getValue() const { return value; }
    Node<T> *getLeft() const { return left; }
    Node<T> *getRight() const { return right; }
    Node<T> *getParent() const { return parent; }

    // funzioni per impostare l'etichetta contenuta nel nodo, il
    // figlio sinistro, il figlio destro e il genitore
    void setValue(T item) { value = item; }
    void setLeft(Node<T> *n) { left = n; }
    void setRight(Node<T> *n) { right = n; }
    void setParent(Node<T> *n) { parent = n; }
private:
    T value; // dato contenuto nel nodo (etichetta)
    Node<T> *parent; // puntatore al genitore
    Node<T> *left; // puntatore al figlio sinistro
    Node<T> *right; // puntatore al figlio destro
};

#endif