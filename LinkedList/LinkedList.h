// By Fabio Nardelli
// lista doppiamente concatenata con sentinella

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class LinkedList;

template <typename T>
std::ostream &operator<<(std::ostream &, const LinkedList<T> &);

template <typename T>
class LinkedList {
    friend std::ostream &operator<< <>(std::ostream &, const LinkedList<T> &);
private:
    struct Node;
public:
    typedef Node * Position;

    LinkedList() { create(); } // costruttore di default
    LinkedList(const LinkedList<T> &); // costruttore per copia
    ~LinkedList(); // distruttore
    LinkedList<T> &operator=(const LinkedList<T> &);

    // operatori
    // restituisce "true" se la lista e' vuota, "false" altrimenti
    bool isEmpty() const { return head == head->next; }
    void print() const; // stampa la lista
    void pushBack(T); // inserisce un nodo in coda
    void pushFront(T); // inserisce un nodo in testa
    // inserisce un elemento nella posizione precedente quella indicata
    void insert(Position, T);
    void popBack(); // rimuove l'elemento in coda
    void popFront(); // rimuove l'elemento in testa
    // rimuove l'elemento precedente la posizione indicata
    void remove(Position);
    // sposta il puntatore al nodo successivo
    Position next(Position pos) const { return pos->next; }
    // sposta il puntatore al nodo precedente
    Position previous(Position pos) const { return pos->previous; }
    Position search(T) const;
    // restituisce la posizione del primo nodo
    Position begin() const { return head->next; }
    // restituisce la posizione dell'ultimo nodo
    Position end() const { return head->previous; }
    // restituisce "true" se la posizione indicata si riferisce
    // alla fine della lista (sentinella), "false" altrimenti
    bool finished(Position pos) const { return pos == head; }
    // legge il dato contenuto nel nodo indicato
    const T &read(Position) const;
    // scrive nel nodo indicato
    void write(Position, T);
    bool operator==(const LinkedList<T> &) const;
private:

    // rappresenta un generico nodo
    struct Node {
        T value; // dato contenuto nel nodo
        Node *next; // puntatore al nodo successivo
        Node *previous; // puntatore al nodo precedente
    };

    Position head; // puntatore al primo nodo (sentinella) della lista
    void create(); // crea una lista vuota
};

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const LinkedList<T> &l) 
{
    l.print();
    return output; // consente cout << a << b << c
}

// crea una lista vuota. E' chiamata dal costruttore di default
template <typename T>
void LinkedList<T>::create()
{
    // crea una lista vuota, munita di sentinella (nodo head)
    head = new Node;
    head->next = head;
    head->previous = head;
}

// costruttore per copia
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &l) 
{
    head = new Node;
    head->next = head;
    head->previous = head;

    Position current = l.begin();
    while (!l.finished(current)) {
        pushBack(l.read(current));
        current = current->next;
    }
}

// distruttore
template <typename T>
LinkedList<T>::~LinkedList() 
{
    while (!isEmpty())
        popFront();

    delete head;
}

// stampa la lista
template <typename T>
void LinkedList<T>::print() const
{
    if (isEmpty()) {
        std::cout << "\n List is empty!\n";
        return;
    }

    Position current = begin();
    std::cout << "\n [";
    int i = 0;
    while (!finished(current)) {

        std::cout << std::setw(3) << current->value << " ";
        // va a capo ogni 5 elementi, tranne dopo l'ultimo
        if ((i + 1) % 5 == 0 && current != end())
            std::cout << "\n  ";
        current = current->next;
        ++i;
    }
    std::cout << " ]\n";
} 

// inserisce un nodo in coda
template <typename T>
void LinkedList<T>::pushBack(T item) 
{
    // crea un nuovo nodo
    Position newNode = new Node;
    newNode->value = item;
    // collega il nuovo nodo alla lista
    if (isEmpty()) { // se la lista e' vuota
        head->next = newNode;
        head->previous = newNode;
        newNode->next = head;
        newNode->previous = head;
    } else {
        newNode->previous = head->previous;
        newNode->next = head;
        head->previous->next = newNode;
        head->previous = newNode;
    }
}

// inserisce un nodo in testa
template <typename T>
void LinkedList<T>::pushFront(T item) 
{
    // crea un nuovo nodo
    Position newNode = new Node;
    newNode->value = item;
    // collega il nuovo nodo alla lista
    if (isEmpty()) { // se la lista e' vuota
        head->next = newNode;
        head->previous = newNode;
        newNode->next = head;
        newNode->previous = head;
    } else {
        newNode->previous = head;
        newNode->next = head->next;
        head->next->previous = newNode;
        head->next = newNode;
    }
}

// inserisce un nodo nella posizione precedente a quella indicata
template <typename T>
void LinkedList<T>::insert(Position pos, T item) 
{
    // crea un nuovo nodo
    Position newNode = new Node;
    newNode->value = item;
    // collega il nuovo nodo alla lista
    newNode->previous = pos->previous;
    newNode->next = pos;
    newNode->previous->next = newNode;
    newNode->next->previous = newNode;
}

// rimuove il nodo in coda
template <typename T>
void LinkedList<T>::popBack() 
{
    if (!isEmpty()) {
        Position current = head->previous;
        head->previous = head->previous->previous;
        head->previous->next = head;
        delete current;
    }
}

// rimuove il nodo in testa
template <typename T>
void LinkedList<T>::popFront() 
{
    if (!isEmpty()) {
        Position current = head->next;
        head->next = head->next->next;
        head->next->previous = head;
        delete current;
    }
}

// rimuove il nodo alla posizione indicata
template <typename T>
void LinkedList<T>::remove(Position pos) 
{
    if (!isEmpty() && !finished(pos)) {
        pos->previous->next = pos->next;
        pos->next->previous = pos->previous;
        delete pos;
    }
}

// cerca la prima occorrenza di un dato. Se lo trova,
// restituisce la posizione del nodo corrispondente, 
// altrimenti restituisce 0
template <typename T>
typename LinkedList<T>::Position LinkedList<T>::search(T item) const 
{
    // se la lista e' vuota, termina la ricerca
    if (isEmpty())
        return 0;

    // puntatore per scandire la lista
    Position current = head->next;

    while (current->value != item && current != end())
        current = current->next;
    if (current->value == item)
        return current;

    return 0;
}

// legge il dato contenuto nel nodo indicato
template <typename T>
const T &LinkedList<T>::read(Position pos) const 
{
    if (isEmpty())
        throw std::domain_error("List is empty!");

    return pos->value;
}

// scrive nel nodo indicato
template <typename T>
void LinkedList<T>::write(Position pos, T item) 
{
    if (!isEmpty())
        pos->value = item;
}

// overload dell'operatore di assegnazione =
template <typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &l) 
{
    if (&l != this) { // evita l'auto-assegnazione

        // cancella i nodi di questa lista
        //Positioncurrent = head->next;
        while (!isEmpty())
            popFront();

        // copia la lista l in questa lista
        Position current = l.begin();
        while (!l.finished(current)) {
            pushBack(current->value);
            current = current->next;
        }
    }

    return *this; // consente a = b = c
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &l) const 
{
    // se le liste sono entrambe vuote, allora sono uguali
    if (isEmpty() && l.isEmpty())
        return true;

    // se solo una delle due e' vuota, allora sono diverse
    // (il caso di entrambe le liste vuote e' gestito sopra)
    if (isEmpty() || l.isEmpty())
        return false;

    // controllo rapido sul primo e l'ultimo elemento di ciascuna lista
    if (head->next->value != l.head->next->value
            || head->previous->value != l.head->previous->value)
        return false;

    // controllo esaustivo su ciascun elemento
    Position thisListPtr = begin();
    Position otherListPtr = l.begin();
    while (thisListPtr->value == otherListPtr->value
            && thisListPtr != head && otherListPtr != l.head) {
        thisListPtr = thisListPtr->next;
        otherListPtr = otherListPtr->next;
    }

    if (thisListPtr->value != otherListPtr->value)
        return false;

    return true;
}

#endif
