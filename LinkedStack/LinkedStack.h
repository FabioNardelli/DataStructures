// By Fabio Nardelli
// coda LIFO basata su una lista doppiamente concatenata con sentinella

#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class LinkedStack;

template <typename T>
std::ostream &operator<<(std::ostream &, const LinkedStack<T> &);

template <typename T>
class LinkedStack {
    friend std::ostream &operator<< <>(std::ostream &, const LinkedStack<T> &);
public:
    LinkedStack() { create(); } // costruttore di default
    LinkedStack(const LinkedStack<T> &); // costruttore per copia
    ~LinkedStack(); // distruttore
    LinkedStack<T> &operator=(const LinkedStack<T> &);

    // operatori

    // restituisce true se la pila e' vuota, false altrimenti
    bool isEmpty() const { return head == head->next; }
    void print() const; // stampa la pila
    void push(T); // inserisce un elemento in cima alla pila
    void pop(); // estrae l'elemento in cima alla pila
    T top() const; // legge l'elemento in cima alla pila
    // operatore di uguaglianza
    bool operator==(const LinkedStack<T> &) const;
private:

    // rappresenta un generico nodo
    struct Node {
        T value; // dato contenuto nel nodo
        Node *next; // puntatore al nodo successivo
        Node *previous; // puntatore al nodo precedente
    };

    Node *head; // puntatore al primo nodo (sentinella) della pila
    void create(); // crea una pila vuota
};

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const LinkedStack<T> &s) 
{
    s.print();
    return output; // consente cout << a << b << c
}

// utility function chiamata dal costruttore di default
template <typename T>
void LinkedStack<T>::create()
{
    // crea una pila vuota, munita di sentinella (nodo head)
    head = new Node;
    head->next = head;
    head->previous = head;
}

// costruttore per copia
template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T> &s) 
{
    head = new Node;
    head->next = head;
    head->previous = head;

    Node *current = s.head->next;
    while (current != s.head) {
        push(current->value);
        current = current->next;
    }
}

// distruttore
template <typename T>
LinkedStack<T>::~LinkedStack() 
{
    while (!isEmpty())
        pop();

    delete head;
}

// stampa la pila
template <typename T>
void LinkedStack<T>::print() const
{
    if (isEmpty()) {
        std::cout << "\n Stack is empty!\n";
        return;
    }

    Node *current = head->next;
    std::cout << "\n [";
    int i = 0;
    while (current != head) {

        std::cout << std::setw(3) << current->value << " ";
        // va a capo ogni 5 elementi, tranne dopo l'ultimo
        if ((i + 1) % 5 == 0 && current != head)
            std::cout << "\n  ";
        current = current->next;
        ++i;
    }
    std::cout << " ]  <-- top\n";
} 

// inserisce un nodo in cima alla pila
template <typename T>
void LinkedStack<T>::push(T item)
{
    // crea un nuovo nodo
    Node *newNode = new Node;
    newNode->value = item;
    // collega il nuovo nodo alla pila
    if (isEmpty()) { // se la pila e' vuota
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

// rimuove il nodo in cima alla pila
template <typename T>
void LinkedStack<T>::pop()
{
    if (!isEmpty()) {
        Node *current = head->previous;
        head->previous = head->previous->previous;
        head->previous->next = head;
        delete current;
    }
}

// legge il primo elemento della pila
template <typename T>
T LinkedStack<T>::top() const
{
    if (isEmpty())
        throw std::domain_error("Stack is empty!");

    return head->previous->value;
}

// overload dell'operatore di assegnazione =
template <typename T>
LinkedStack<T> &LinkedStack<T>::operator=(const LinkedStack<T> &s) 
{
    if (&s != this) { // evita l'auto-assegnazione

        // cancella i nodi di questa pila
        while (!isEmpty())
            pop();

        // copia la pila s in questa pila
        Node *current = s.head->next;
        while (current != s.head) {
            push(current->value);
            current = current->next;
        }
    }

    return *this; // consente a = b = c
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool LinkedStack<T>::operator==(const LinkedStack<T> &s) const 
{
    // se le pile sono entrambe vuote, allora sono uguali
    if (isEmpty() && s.isEmpty())
        return true;

    // se solo una delle due e' vuota, allora sono diverse
    // (il caso di entrambe le code vuote e' gestito sopra)
    if (isEmpty() || s.isEmpty())
        return false;

    // controllo rapido sul primo e l'ultimo elemento di ciascuna pila
    if (head->next->value != s.head->next->value
            || head->previous->value != s.head->previous->value)
        return false;

    // controllo esaustivo su ciascun elemento
    Node *thisStackPtr = head->next;
    Node *otherStackPtr = s.head->next;
    while (thisStackPtr->value == otherStackPtr->value
            && thisStackPtr != head && otherStackPtr != s.head) {
        thisStackPtr = thisStackPtr->next;
        otherStackPtr = otherStackPtr->next;
    }

    if (thisStackPtr->value != otherStackPtr->value)
        return false;

    return true;
}

#endif
