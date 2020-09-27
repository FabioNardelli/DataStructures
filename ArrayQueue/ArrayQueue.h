// By Fabio Nardelli
// coda FIFO implementata mediante vettore circolare

#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class ArrayQueue;

template <typename T>
std::ostream &operator<<(std::ostream &, const ArrayQueue<T> &);

template <typename T>
class ArrayQueue {
    friend std::ostream &operator<< <>(std::ostream &, const ArrayQueue<T> &);
public:
    ArrayQueue(int c = defaultCapacity) : head(0), size(0) { create(c); }
    ArrayQueue(const ArrayQueue<T> &); // costruttore per copia
    ~ArrayQueue() { delete [] array; } // distruttore
    ArrayQueue<T> &operator=(const ArrayQueue<T> &);

    // operatori
    // restituisce "true" se la coda e' vuota, "false" altrimenti
    bool isEmpty() const { return size == 0; }
    void print() const; // stampa la coda
    void enqueue(T); // inserisce un elemento in coda
    void dequeue(); // elimina l'elemento da piu' tempo in coda
    T top() const; // legge l'elemento da piu' tempo in coda
    // operatore di uguaglianza
    bool operator==(const ArrayQueue<T> &) const;

private:
    T *array; // puntatore per rappresentare l'array circolare che contiene la coda
    int head; // posizione dell'elemento di testa
    int size; // numero di elementi contenuti nell'array, e' la
    // dimensione della coda come appare all'utente
    int capacity; // dimensione dell'array
    static const int defaultCapacity = 16; // dimensione di default
    void create(int); // funzione di inizializzazione
};

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const ArrayQueue<T> &q) 
{
    q.print();
    return output; // consente cout << a << b << c
}

// stampa la coda
template <typename T>
void ArrayQueue<T>::print() const
{
    if (isEmpty()) {
        std::cout << "\n Queue is empty!\n";
        return;
    }

    std::cout << "\n [";

    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(3) << array[(i + head) % capacity] << " ";
        // va a capo ogni 5 elementi, tranne dopo l'ultimo
        if ((i + 1) % 5 == 0 && i != size - 1)
            std::cout << "\n  ";
    }
    std::cout << " ]\n";
} 

// funzione di inizializzazione
template <typename T>
void ArrayQueue<T>::create(int c) 
{
    if (c > 0)
        capacity = c;

    array = new T[capacity];
}

// costruttore per copia
template <typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue<T> &q)
{
    capacity = q.capacity;
    size = q.size;
    head = 0;
    array = new T[capacity];

    // copia gli elementi (escludendo quelli cancellati)
    for (int i = 0; i < size; ++i)
        array[i] = q.array[(i + q.head) % capacity];
}

// overload dell'operatore di assegnazione =
template <typename T>
ArrayQueue<T> &ArrayQueue<T>::operator=(const ArrayQueue<T> &q)
{
    if (&q != this) { // evita l'autoassegnazione
        if (capacity != q.capacity) { // se i due array hanno capacita' diversa,
            delete [] array;       	  // dealloca quello di sinistra e alloca
            capacity = q.capacity; 	  // un nuovo array di capacita' uguale
            array = new T[capacity];  // a quello da copiare
        }

        size = q.size;
        head = 0;

        // copia gli elementi (escludendo quelli cancellati)
        for (int i = 0; i < size; ++i)
            array[i] = q.array[(i + q.head) % capacity];
    }

    return *this; // consente (a = b = c)
}

// inserisce un elemento in coda
template <typename T>
void ArrayQueue<T>::enqueue(T item)
{
    if (size == capacity) {
        std::cerr << "Cannot enqueue: Queue is full!\n";
        return;
    }

    array[(head + size) % capacity] = item;
    ++size;
}

// rimuove l'elemento in testa alla coda
template <typename T>
void ArrayQueue<T>::dequeue()
{
    if (isEmpty()) {
        std::cerr << "Cannot dequeue: Queue is empty!\n";
        return;
    }

    head = (head + 1) % capacity;
    --size;
}

// legge il dato contenuto nell'elemento di testa
template <typename T>
T ArrayQueue<T>::top() const
{
    if (isEmpty())
        throw std::domain_error("Queue is empty!");

    return array[head];
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool ArrayQueue<T>::operator==(const ArrayQueue<T> &q) const
{
    // se le code sono entrambe vuote, allora sono uguali
    if (isEmpty() && q.isEmpty())
        return true;

    // se solo una delle due e' vuota, allora sono diverse
    // (il caso di entrambe le code vuote e' gestito sopra)
    if (isEmpty() || q.isEmpty())
        return false;

    // se le code hanno dimensioni diversa, allora sono diverse
    if (size != q.size)
        return false;

    // controllo esaustivo su ciascun elemento
    for (int i = 0; i < size; ++i) {

        if (array[(i + head) % capacity] !=
                q.array[(i + q.head) % q.capacity])
            return false;
    }

    return true;
}

#endif