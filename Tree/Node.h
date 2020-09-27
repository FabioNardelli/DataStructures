// By Fabio Nardelli
// classe usata per rappresentare un nodo di un albero n-ario
#ifndef NODE_H
#define NODE_H

template <typename T>
class Tree;

template <typename T>
class Node {
    friend class Tree<T>;
public:
    // costruttore di default
    Node() : parent(0), child(0), sibling(0) { }

    // costruttore con argomenti: prende in input il dato da
    // inserire nel nodo, e (eventualmente) i valori dei puntatori
    // parent, child e sibling, di default impostati a 0. Ad esempio,
    // e' utile per impostare il parent direttamente alla creazione
    // del nodo, evitando cosi' una "doppia" assegnazione
    // (in realta' inizializzazione + assegnazione)
    Node(T item, Node<T> *p = 0, Node<T> *c = 0, Node<T> *s = 0)
        : value(item), parent(p), child(c), sibling(s) { }

    // funzioni per ottenere l'etichetta contenuta nel nodo, il
    // figlio sinistro, il figlio destro e il genitore
    T getValue() const { return value; }
    Node<T> *getChild() const { return child; }
    Node<T> *getSibling() const { return sibling; }
    Node<T> *getParent() const { return parent; }

    // funzioni per impostare l'etichetta contenuta nel nodo, il
    // figlio sinistro, il figlio destro e il genitore
    void setValue(T item) { value = item; }
    void setChild(Node<T> *n) { child = n; }
    void setSibling(Node<T> *n) { sibling = n; }
    void setParent(Node<T> *n) { parent = n; }
private:
    T value; // dato contenuto nel nodo (etichetta)
    Node<T> *parent; // puntatore al genitore
    Node<T> *child; // puntatore al primo figlio (il piu' a sinistra)
    Node<T> *sibling; // puntatore al fratello successivo
};

#endif