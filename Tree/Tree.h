// By Fabio Nardelli
// realizzazione di albero n-ario mediante puntatori

#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Node.h"
#include "../LinkedQueue/LinkedQueue.h"

template <typename T>
class Tree;

template <typename T>
std::ostream &operator<<(std::ostream &, Tree<T> &);

template <typename T>
class Tree {
    friend std::ostream &operator<< <>(std::ostream &, Tree<T> &);
public:	
    Tree() { create(); } // costruttore di default
    Tree(const Tree<T> &); // costruttore per copia
    ~Tree() { deleteSubTree(rootPtr); } // distruttore
    Tree<T> &operator=(const Tree<T> &);

    // operatori
    // crea un albero vuoto
    void create() { rootPtr = 0; }
    // restituisce true se l'albero e' vuoto, false altrimenti
    bool isEmpty() const { return rootPtr == 0; }
    // restituisce il nodo radice dell'albero
    Node<T> *root() const { return rootPtr; }
    // restituisce il nodo genitore del nodo indicato
    Node<T> *parent(Node<T> *n) const { return n->parent; }
    // restituisce il primo figlio (il piu' a sinistra) del nodo indicato
    Node<T> *firstChild(Node<T> *n) const { return n->child; }
    // restituisce il fratello successivo del nodo indicato
    Node<T> *nextSibling(Node<T> *n) const { return n->sibling; }
    // restituisce true se il nodo indicato non ha fratelli successivi,
    // false altrimenti
    bool lastSibling(Node<T> *n) const { return n->sibling == 0; }
    // restituisce true se il nodo indicato e' una foglia (non ha figli)
    bool leaf(Node<T> *n) const { return n->child == 0; }
    // inserisce un nodo come radice dell'albero
    void insertRoot(Node<T> *);
    // aggiunge un nodo come primo figlio del nodo indicato.
    // Il nodo e' inizializzato con l'etichetta fornita come argomento.
    void addAsChild(Node<T> *, T);
    // inserisce un sottoalbero come primo figlio del nodo indicato
    void insertChild(Node<T> *, Node<T> *);
    // aggiunge un nodo come primo fratello del nodo indicato.
    // Il nodo e' inizializzato con l'etichetta fornita come argomento.
    void addAsSibling(Node<T> *, T);
    // inserisce un sottoalbero come primo fratello del nodo indicato
    void insertSibling(Node<T> *, Node<T> *);
    // cancella il sottoalbero avente radice nel nodo indicato
    void deleteSubTree(Node<T> *);
    // legge il dato contenuto nel nodo indicato
    T read(Node<T> *) const;
    // scrive nel nodo indicato
    void write(T, Node<T> *);

    // funzioni di servizio
    void printSubTree(Node<T> *) const;
    void print() const;
    int height(Node<T> *) const; // restituisce l'altezza del nodo
    int count(Node<T> *) const;	 // restituisce il numero dei nodi discendenti
                                 // del nodo indicato
    // algoritmi di visita
    // visite in profondita' (DFS)
    void preOrder(Node<T> *) const;
    void inOrder(Node<T> *, int) const;
    void postOrder(Node<T> *) const;
    // visita in ampiezza (BFS)
    void bfs(Node<T> *) const;
    bool belongs(Node<T> *) const;
private:
    Node<T> *rootPtr; // puntatore alla radice dell'albero

    // utility function per copiare un albero. Usata dal costruttore
    // per copia e dall'operatore =
    void copyTree(Node<T> **, const Node<T> *);
};

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, Tree<T> &t) 
{
    t.print();
    return output; // consente a << b << c
}

// Funzione ausiliaria per la copia di un albero. E' utilizzata dal 
// costruttore per copia e dall'operatore di assegnazione =.
// Prende in input un puntatore a un nodo dell'albero copia, copy
// (passato per riferimento), e un puntatore a un nodo dell'albero             
// da copiare, source. Se quest'ultimo e' diverso da NIL (0), procede
// in questo modo: se copy e' uguale a NIL (l'albero copia e' vuoto),  
// crea un nuovo nodo e copia il valore contenuto nel nodo source nel 
// nuovo nodo; dopodiche', se source ha un primogenito, alloca memoria 
// per un nuovo nodo primo figlio di copy e copia il valore dal figlio
// corrispondente di source. Ripete lo stesso per un eventuale fratello
// di source. Infine chiama ricorsivamente la procedura sul primogenito
// e sul fratello source. Il controllo (*copy == 0) e' necessario
// per evitare che, nelle chiamate successive alla prima, si tenti di
// allocare memoria dove e' gia' presente un nodo. 
// Alla prima chiamata, e' necessario che la condizione *copy == 0 sia
// soddisfatta, cioe' che l'albero copia sia vuoto, affinche' la copia
// avvenga correttamente. E' responsabilita' del chiamante accertarsene! 
template <typename T>
void Tree<T>::copyTree(Node<T> **copy, const Node<T> *source)
{
    if (source != 0) {
        // *copy == 0 solo alla prima chiamata,
        // quando l'albero copia e' vuoto.
        if (*copy == 0)
            *copy = new Node<T>(source->value);
        // usa il costruttore per impostare dato e parent
        if (source->child != 0) // Node<T>(dato, parent)
            (*copy)->child = new Node<T>(source->child->value, *copy);

        if (source->sibling != 0) // idem
            (*copy)->sibling = new Node<T>(source->sibling->value, (*copy)->parent);

        copyTree(&(*copy)->child, source->child);
        copyTree(&(*copy)->sibling, source->sibling);
    }
}

// costruttore per copia
template <typename T>
Tree<T>::Tree(const Tree<T> &t)
{
    if (!t.isEmpty())
        copyTree(&rootPtr, t.root());
    else
        rootPtr = 0;
}

// overload dell'operatore di assegnazione =
template <typename T>
Tree<T> &Tree<T>::operator=(const Tree<T> &t)
{
    if (&t != this) { // evita l'autoassegnazione
        if (!isEmpty())
            deleteSubTree(rootPtr); // cancella l'albero a sinistra di =
        copyTree(&rootPtr, t.root());
    }
    return *this;
}

// inserisce il nodo indicato come radice dell'albero.
// L'albero dev'essere vuoto
template <typename T>
void Tree<T>::insertRoot(Node<T> *n)
{
    if (rootPtr == 0) {
        rootPtr = n;
        rootPtr->setParent(0);
        rootPtr->setChild(0);
        rootPtr->setSibling(0);
    }
}

// aggiunge un nodo come primo figlio del nodo indicato. 
// Il secondo argomento e' l'etichetta del nodo.
template <typename T>
void Tree<T>::addAsChild(Node<T> *n, T item) 
{
    if (n != 0 && !isEmpty()) {
        Node<T> *newNode = new Node<T>(item);
        // se n ha gia' almeno un figlio, collega il puntatore
        // al prossimo fratello di newNode al primo figlio di n
        if (n->child != 0)
            newNode->setSibling(n->child);

        n->child = newNode;
        newNode->setParent(n);
    }
}

// inserisce un sottoalbero come primo figlio (il piu' a sinistra)
// del nodo indicato
template <typename T>
void Tree<T>::insertChild(Node<T> *n, Node<T> *subTree) 
{
    if (n != 0 && !isEmpty() && subTree != 0) {
        // se n ha gia' almeno un figlio, collega il puntatore
        // al prossimo fratello di subTree al primo figlio di n
        if (n->child != 0)
            subTree->setSibling(n->child);

        n->child = subTree;
        subTree->setParent(n);
    }
}

// aggiunge un nodo come prossimo fratello (immediatamente a destra)
// del nodo indicato. Il secondo argomento e' l'etichetta del nodo.
template <typename T>
void Tree<T>::addAsSibling(Node<T> *n, T item) 
{
    if (n != rootPtr && !isEmpty()) {
        Node<T> *newNode = new Node<T>(item);
        // se n ha almeno un fratello, collega il puntatore
        // al prossimo fratello di newNode al primo fratello di n
        if (n->sibling != 0)
            newNode->setSibling(n->sibling);

        n->sibling = newNode;
        newNode->setParent(n->parent);
    }
}

// iserisce un sottoalbero come prossimo fratello (immediatamente a destra)
// del nodo indicato
template <typename T>
void Tree<T>::insertSibling(Node<T> *n, Node<T> *subTree) 
{
    if (n != rootPtr && !isEmpty() && subTree != 0) {
        // se n ha almeno un fratello, collega il puntatore
        // al prossimo fratello di t.root() al primo fratello di n
        if (n->sibling != 0)
            subTree->setSibling(n->sibling);

        n->sibling = subTree;
        subTree->setParent(n->parent);
    }
}

// cancella il sottoalbero avente radice nel nodo indicato
template <typename T>
void Tree<T>::deleteSubTree(Node<T> *n) 
{
    if (n != 0) {
        Node<T> *current = n->child, *tmp;
        while (current != 0) {
            tmp = current->sibling;
            deleteSubTree(current);
            current = tmp;
        }
        if (n != rootPtr) {             // Se n non e' la radice, allora ha un genitore.
            Node<T> *p = parent(n);     // Fa puntare p al genitore di n e:
            if (firstChild(p) == n) { 	// se n e' il primo figlio di p,
                                        // imposta il puntatore al primo figlio di p al fratello
                p->child = n->sibling; 	// successivo di n (NIL se n e' figlio unico).
            } else {                    // Se invece n non e' il primo figlio di p, fa puntare p
                p = p->child;           // al suo primo figlio, current al fratello
                current = p->sibling;   // successivo di quest'ultimo e,
                while (current != n) {  // fintantoche' current != n, fa puntare
                    p = current;        // p a current e current al prossimo fratello.
                    current = current->sibling; // Quando current == n, vuol dire
                }                       // che p punta al fratello precedente di n; allora fa puntare
                p->sibling = n->sibling;// il puntatore al prossimo fratello di p
            }                           // al fratell successivo a n (NIL se non ha
            delete n;                   // un fratello successivo)
        } else {
            rootPtr = 0;
        }
    }
}

// legge l'etichetta del nodo indicato
template <typename T>
T Tree<T>::read(Node<T> *n) const 
{
    if (n == 0)
        throw std::invalid_argument("Invalid node!");

    return n->getValue();
}

// scrive nell'etichetta del nodo indicato
template <typename T>
void Tree<T>::write(T item, Node<T> *n) 
{
    if (n != 0)
        n->setValue(item);
}

// stampa il sottoalbero di radice nel nodo indicato (pre-visita)
template <typename T>
void Tree<T>::printSubTree(Node<T> *n) const 
{
    if (n != 0) {
        std::cout << n->value << "  ";
        if (n->child != 0)
            printSubTree(n->child);

        if (n->sibling != 0)
            printSubTree(n->sibling);
    }
}

// stampa l'albero (in pre ordine)
template <typename T>
void Tree<T>::print() const 
{
    if (rootPtr != 0) {
        std::cout << "\n\n [";
        printSubTree(rootPtr);
        std::cout << "]\n";
    } else {
        std::cout << "\n Tree is empty!\n";
    }
}

// mostra l'altezza del nodo indicato
template <typename T>
int Tree<T>::height(Node<T> *n) const 
{
    if (n == 0)
        throw std::invalid_argument("Invalid node!");

    int level = 0;
    while (parent(n) != 0) {
        n = parent(n);
        ++level;
    }
    return level;
}

// restituisce il numero di discendenti del nodo indicato
template <typename T>
int Tree<T>::count(Node<T> *n) const 
{
    if (n == 0)
        return 0;

    // effettua una post-visita
    int c = 1; // se non e' vuoto, l'albero ha almeno un nodo
    Node<T> *current = n->child;
    while (current != 0) {
        c += count(current);
        current = current->sibling;
    }
    return c;
}

// visita (e stampa) l'albero in pre-ordine
template <typename T>
void Tree<T>::preOrder(Node<T> *n) const
{
    if (n != 0) {
        std::cout << n->value << "\n";
        Node<T> *current = n->child;
        while (current != 0) {
            preOrder(current);
            current = current->sibling;
        }
    }
}

// visita (e stampa) l'albero in ordine.
// il parametro i determina 
template <typename T>
void Tree<T>::inOrder(Node<T> *n, int i) const
{
    if (n != 0) {
        Node<T> *current = n->child;
        int k = 0;
        // visita i primi i figli del nodo n
        while (current != 0 && k < i) {
            ++k;
            inOrder(current, i);
            current = current->sibling;
        }
        std::cout << n->value << "\n";
        // visita i restanti figli del nodo n
        while (current != 0) {
            inOrder(current, i);
            current = current->sibling;
        }
    }
}

// visita (e stampa) l'albero in post-ordine
template <typename T>
void Tree<T>::postOrder(Node<T> *n) const
{
    if (n != 0) {
        Node<T> *current = n->child;
        while (current != 0) {
            postOrder(current);
            current = current->sibling;
        }
        std::cout << n->value << "\n";
    }
}

// visita (e stampa) l'albero in ampiezza
// Algoritmo iterativo che fa uso di una coda
template <typename T>
void Tree<T>::bfs(Node<T> *n) const
{
    if (n != 0) {
        Node<T> *current;
        LinkedQueue<Node<T> *> q;
        q.enqueue(n);
        while (!q.isEmpty()) {
            current = q.top();
            std::cout << current->value << "\n";
            q.dequeue();
            current = current->child;
            while (current != 0) {
                q.enqueue(current);
                current = current->sibling;
            }
        }
    }
}

// indica se il nodo fornito in input appartiene all'albero
template <typename T>
bool Tree<T>::belongs(Node<T> *n) const
{
    if (n == rootPtr) // se n e' la radice, ovviamente appartiene
        return true;

    // se n == NIL o n->parent == NIL
    // e n non e' la radice, allora non appartiene
    if (n == 0 || n->parent == 0)
        return false;

    return belongs(n->parent); // chiamata ricorsiva su n->parent
}

#endif
