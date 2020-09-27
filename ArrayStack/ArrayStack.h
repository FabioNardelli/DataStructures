// By Fabio Nardelli
// pila LIFO implementata mediante un vettore dinamico

#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class ArrayStack;

template <typename T>
std::ostream &operator<<(std::ostream &, const ArrayStack<T> &);

template <typename T>
class ArrayStack {
    friend std::ostream &operator<< <>(std::ostream &, const ArrayStack<T> &);
public:
    // costruttore di default
    ArrayStack(int c = defaultCapacity) : size(0) { create(c); }
    ArrayStack(const ArrayStack<T> &); // costruttore per copia
    ~ArrayStack() { delete [] array; } // distruttore
    ArrayStack<T> &operator=(const ArrayStack<T> &);

    // operatori
    // restituisce "true" se la pila e' vuota, "false" altrimenti
    bool isEmpty() const { return size == 0; }
    void print() const; // stampa la pila
    void push(T); // inserisce un elemento in cima alla pila
    void pop();
    T top() const; // legge il primo elemento (cima) della pila
    bool operator==(const ArrayStack<T> &) const;

private:
    T *array; // puntatore usato per la rappresentazione dell'array che contiene la pila
    int size; // numero di elementi contenuti nell'array, e' la
    // dimensione della pila come appare all'utente
    int capacity; // dimensione dell'array

    static const int defaultCapacity = 16;
    static const float minLoadFactor;

    void create(int); // crea una pila vuota
};

// costante che rappresenta il fattore di carico minimo al di sotto del quale
// la dimensione del vettore viene dimezzata
template <typename T>
const float ArrayStack<T>::minLoadFactor = .25;

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const ArrayStack<T> &s) 
{
    s.print();
    return output; // consente cout << a << b << c
}

// stampa la pila
template <typename T>
void ArrayStack<T>::print() const
{
    if (isEmpty()) {
        std::cout << "\n Stack is empty!\n";
        return;
    }
    std::cout << "\n [";

    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(3) << array[i] << " ";
        // va a capo ogni 5 elementi, tranne dopo l'ultimo
        if ((i + 1) % 5 == 0 && i != size - 1)
            std::cout << "\n ";
    }
    std::cout << " ]  <-- top\n";
} 

// funzione di inizializzazione
template <typename T>
void ArrayStack<T>::create(int c) 
{
    if (c > 0)
        capacity = c;

    array = new T[capacity];
}

// costruttore per copia
template <typename T>
ArrayStack<T>::ArrayStack(const ArrayStack<T> &s) 
{
    capacity = s.capacity;
    size = s.size;
    array = new T[capacity];

    for (int i = 0; i < size; ++i)
        array[i] = s.array[i];
}

// overload dell'operatore di assegnazione =
template <typename T>
ArrayStack<T> &ArrayStack<T>::operator=(const ArrayStack<T> &s) 
{
    if (&s != this) { // evita l'autoassegnazione

        if (capacity != s.capacity) { // se i due array hanno capacita'
            delete [] array;          // diversa, dealloca quello di sinistra
            capacity = s.capacity;    // e alloca un nuovo array di capacita'
            array = new T[capacity];  // uguale a quello da copiare
        }

        size = s.size;

        // copia gli elementi
        for (int i = 0; i < s.size; ++i)
            array[i] = s.array[i];
    }

    return *this; // consente il cascading (a = b = c)
}

// inserisce un elemento in cima alla pila
template <typename T>
void ArrayStack<T>::push(T item)
{
    if (size < capacity) {  // se c'e' spazio nell'array
        array[size] = item; // aggiunge semplicemente l'elemento
    } else { // altrimenti alloca un nuovo array piu' grande
        capacity *= 2;
        T *temp = new T[capacity];
        // copia gli elementi dal vecchio array al nuovo
        for (int i = 0; i < size; ++i)
            temp[i] = array[i];
        delete [] array;
        temp[size] = item;
        array = temp;
    }
    ++size;
}

// rimuove l'elemento in cima alla pila
template <typename T>
void ArrayStack<T>::pop()
{
    if (isEmpty()) {
        std::cerr << "Cannot pop: Stack is empty!\n";
        return;
    }

    --size;
    // se il fattore di carico diventa <= minLoadFactor
    // dimezza la capacita' dell'array
    if (static_cast<float>(size) / capacity <= minLoadFactor) {
        capacity /= 2;
        T *temp = new T[capacity];
        // copia gli elementi dal vecchio array al nuovo
        for (int i = 0; i < size; ++i)
            temp[i] = array[i];
        delete [] array;
        array = temp;
    }
}

// legge il primo elemento
template <typename T>
T ArrayStack<T>::top() const
{
    if (isEmpty())
        throw std::domain_error("Stack is empty!");

    return array[size - 1];
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool ArrayStack<T>::operator==(const ArrayStack<T> &s) const
{
    // se le pile sono entrambe vuote, allora sono uguali
    if (isEmpty() && s.isEmpty())
        return true;

    // se solo una delle due e' vuota, allora sono diverse
    // (il caso di entrambe le pile vuote e' gestito sopra)
    if (isEmpty() || s.isEmpty())
        return false;

    // se le pile hanno dimensioni diversa, allora sono diverse
    if (size != s.size)
        return false;

    // controllo rapido sul primo e l'ultimo elemento di ciascuna pila
    if (array[0] != s.array[0]
                            || array[size - 1] != s.array[size - 1])
        return false;

    // controllo esaustivo su ciascun elemento dal secondo al penultimo
    // (il primo e l'ultimo sono gia' stati controllati sopra)
    int i = 1;
    while (i < size - 1) {
        if (array[i] != s.array[i])
            return false;

        ++i;
    }

    return true; // se l'esecuzione arriva qui, le due pile sono uguali
}

#endif
