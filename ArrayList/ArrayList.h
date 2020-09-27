// By Fabio Nardelli
// lista implementata mediante vettore dinamico

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class ArrayList;

template <typename T>
std::ostream &operator<<(std::ostream &, const ArrayList<T> &);

template <typename T>
class ArrayList {
    friend std::ostream &operator<< <>(std::ostream &, const ArrayList<T> &);
public:
    typedef int Position;

    // costruttore di default
    ArrayList(int c = defaultCapacity) : size(0) { create(c); }
    ArrayList(const ArrayList<T> &); // costruttore per copia
    ~ArrayList() { delete [] array; } // distruttore
    ArrayList<T> &operator=(const ArrayList<T> &);

    // operatori
    // restituisce true se la lista e' vuota, false altrimenti
    bool isEmpty() const { return size == 0; }
    void print() const; // stampa la lista
    void pushBack(T);  // inserisce un elemento in coda
    void pushFront(T); // inserisce un elemento in testa
    // inserisce un elemento nella posizione precedente quella indicata
    void insert(int, T);
    void popBack(); // rimuove l'elemento in coda
    void popFront(); // rimuove l'elemento in testa
    // rimuove l'elemento precedente la posizione indicata
    void remove(int);
    // restituisce la posizione dell'elemento successivo
    Position next(Position) const;
    // restituisce la posizione dell'elemento precedente
    Position previous(Position) const;
    // cerca la posizione di un elemento.
    // Utile per le funzioni insert e remove
    Position search(T) const;
    // restituisce la posizione del primo elemento
    Position begin() const { return 0; }
    // restituisce la posizione dell'ultimo elemento
    Position end() const { return (size != 0 ? size - 1 : 0); }
    // restituisce "true" se la posizione indicata si riferisce
    // alla fine della lista, "false" altrimenti
    bool finished(Position pos) const { return pos == size || pos == -1; }
    const T &read(Position) const; // legge l'elemento nella posizione indicata
    void write(Position, T); // scrive nell'elemento nella posizione indicata
    // operatore di uguaglianza ==
    bool operator==(const ArrayList<T> &) const;

private:
    T *array; // puntatore usato per la rappresentazione dell'array che contiene la lista
    int size; // numero di elementi contenuti nell'array, e' la
    // dimensione della lista come appare all'utente
    int capacity; // dimensione dell'array

    static const int defaultCapacity = 16;
    static const float minLoadFactor;
    void create(int); // crea una lista vuota
};

// costante che rappresenta il fattore di carico minimo al di sotto del quale
// la dimensione del vettore viene dimezzata
template <typename T>
const float ArrayList<T>::minLoadFactor = .25;


/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const ArrayList<T> &l) 
{
    l.print();
    return output; // consente cout << a << b << c
}

// stampa la lista
template <typename T>
void ArrayList<T>::print() const
{
    if (isEmpty()) {
        std::cout << "\n List is empty!\n";
        return;
    }
    std::cout << "\n [";

    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(3) << array[i] << " ";
        // va a capo ogni 5 elementi, tranne dopo l'ultimo
        if ((i + 1) % 5 == 0 && i != size - 1)
            std::cout << "\n ";
    }
    std::cout << " ]\n";
} 

// funzione di inizializzazione
template <typename T>
void ArrayList<T>::create(int c) 
{
    if (c > 0)
        capacity = c;

    array = new T[capacity];
}

// costruttore per copia
template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T> &l) 
{
    capacity = l.capacity;
    size = l.size;
    array = new T[capacity];

    for (int i = 0; i < size; ++i)
        array[i] = l.array[i];
}

// overload dell'operatore di assegnazione =
template <typename T>
ArrayList<T> &ArrayList<T>::operator=(const ArrayList<T> &l) 
{
    if (&l != this) { // evita l'autoassegnazione

        if (capacity != l.capacity) { // se i due array hanno capacita'
            delete [] array;          // diversa, dealloca quello di sinistra
            capacity = l.capacity;    // e alloca un nuovo array di capacita'
            array = new T[capacity];  // uguale a quello da copiare
        }

        size = l.size;

        // copia gli elementi
        for (int i = 0; i < size; ++i)
            array[i] = l.array[i];
    }

    return *this; // consente (a = b = c)
}

// inserisce un elemento in coda
template <typename T>
void ArrayList<T>::pushBack(T item) 
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

// inserisce un elemento in testa
template <typename T>
void ArrayList<T>::pushFront(T item) 
{
    if (size < capacity) { // se c'e' spazio nell'array
        // sposta tutti gli elementi in avanti di uno
        for (int i = size; i > 0; --i)
            array[i] = array[i - 1];
        // e aggiunge il nuovo elemento in prima posizione
        array[0] = item;
    } else { // altrimenti alloca un nuovo array piu' grande
        capacity *= 2;
        T *temp = new T[capacity];
        // copia gli elementi dal vecchio array al nuovo
        for (int i = size; i > 0; --i)
            temp[i] = array[i - 1];
        delete [] array;
        temp[0] = item;
        array = temp;
    }
    ++size;
}

// inserisce un elemento nella posizione indicata. Gli elementi successivi
// vengono spostati in avanti di un posto
template <typename T>
void ArrayList<T>::insert(Position pos, T item) 
{
    if (((pos < 0 || pos >= size) && size > 0)
            || (pos < 0 && size == 0)) {
        std::cerr << "Cannot insert: Position must be 0 - (size - 1)\n";
        return;
    }

    if (size < capacity) { // se c'e' spazio nell'array
        // sposta tutti gli elementi dalla posizione pos
        // in poi in avanti di uno
        for (int i = size; i > pos; --i)
            array[i] = array[i - 1];
        // e aggiunge il nuovo elemento alla posizione pos
        array[pos] = item;
    } else { // altrimenti alloca un nuovo array piu' grande
        capacity *= 2;
        T *temp = new T[capacity];
        // copia gli elementi dal vecchio array al nuovo...
        for (int i = size; i > pos; --i)
            temp[i] = array[i - 1];
        temp[pos] = item; // ...inserendo il nuovo elemento
        if (pos > 0)
            for (int i = pos - 1; i >= 0; --i)
                temp[i] = array[i];

        delete [] array;
        array = temp;
    }
    ++size;
}

// rimuove l'elemento in coda
template <typename T>
void ArrayList<T>::popBack() 
{
    if (isEmpty()) {
        std::cerr << "Cannot remove: List is empty!";
        return;
    }

    --size;
    // se il fattore di carico diventa <= 1/4
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

// rimuove l'elemento in testa
template <typename T>
void ArrayList<T>::popFront() 
{
    if (isEmpty()) {
        std::cerr << "Cannot pop: List is empty!";
        return;
    }

    // scala tutti gli elementi a sinistra di una posizione
    for (int i = 1; i < size; ++i)
        array[i - 1] = array[i];
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

// rimuove l'elemento alla posizione indicata
template <typename T>
void ArrayList<T>::remove(Position pos) 
{
    if (isEmpty()) {
        std::cerr << "Cannot remove: List is empty!";
        return;
    }
    if (pos < 0 || pos >= size) {
        std::cerr << "Cannot remove: Position must be 0 - (size - 1)\n";
        return;
    }

    // se l'elemento da rimuovere e' il primo
    if (pos == 0)
        popFront();
    // se l'elemento da rimuovere e' l'ultimo
    else if (pos == size - 1)
        popBack();
    else { // se l'elemento da rimuovere e' interno
        for (int i = pos; i + 1 < size; ++i)
            array[i] = array[i + 1];
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
}

// restituisce la posizione dell'elemento successivo
// se pos < size restituisce pos = size, che e' fuori range ma e' necessario 
// per consentire di iterare sulla lista (altrimenti si avrebbe un loop infinito).
// Si puo' effettuare il controllo sui limiti con la funzione finished().
// Le altre funzioni controllano la validita' della posizione, per cui non 
// c'e' rischio di modificare o accedere ad aree di memoria fuori range. 
template <typename T>
typename ArrayList<T>::Position ArrayList<T>::next(Position pos) const 
{
    if (pos >= 0 && pos < size)
        return ++pos;
    else
        return pos;
}

// restituisce la posizione dell'elemento precedente
// se pos == 0 restituisce pos = -1, che e' fuori range ma e' necessario 
// per consentire di iterare sulla lista (altrimenti si avrebbe un loop infinito).
// Si puo' effettuare il controllo sui limiti con la funzione finished().
// Le altre funzioni controllano la validita' della posizione, per cui non 
// c'e' rischio di modificare o accedere ad aree di memoria fuori range.  
template <typename T>
typename ArrayList<T>::Position ArrayList<T>::previous(Position pos) const 
{
    if (pos >= 0 && pos < size)
        return --pos;
    else
        return pos;
}

// cerca la prima occorrenza di un dato. Se lo trova,
// restituisce la posizione dell'elemento corrispondente, 
// altrimenti restituisce -1
template <typename T>
typename ArrayList<T>::Position ArrayList<T>::search(T item) const 
{
    // se la lista e' vuota, termina la ricerca
    if (isEmpty())
        return -1;
    int i;
    for (i = 0; i < size && array[i] != item; ++i);
    if (array[i] == item)
        return i;
    return -1;
}

// legge il dato contenuto nell'elemento indicato
template <typename T>
const T &ArrayList<T>::read(Position pos) const 
{
    if (isEmpty())
        throw std::domain_error("List is empty!");

    if (pos < 0 || pos >= size)
        throw std::out_of_range("Invalid position! "
                "Position must be 0 - (size - 1)");

    return array[pos];
}

// scrive nell'elemento indicato
template <typename T>
void ArrayList<T>::write(Position pos, T item) 
{
    if (pos < 0 || pos >= size) {
        std::cerr << "Invalid position! "
                << "Position must be 0 - (size - 1)";
        return;
    } else {
        array[pos] = item;
    }
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool ArrayList<T>::operator==(const ArrayList<T> &l) const 
{
    // se le liste sono entrambe vuote, allora sono uguali
    if (isEmpty() && l.isEmpty())
        return true;

    // se solo una delle due e' vuota, allora sono diverse
    // (il caso di entrambe le liste vuote e' gestito sopra)
    if (isEmpty() || l.isEmpty())
        return false;

    // se le liste hanno dimensioni diversa, allora sono diverse
    if (size != l.size)
        return false;

    // controllo rapido sul primo e l'ultimo elemento di ciascuna lista
    if (array[0] != l.array[0]
                            || array[size - 1] != l.array[size - 1])
        return false;

    // controllo esaustivo su ciascun elemento dal secondo al penultimo
    // (il primo e l'ultimo sono gia' stati controllati sopra)
    int i = 1;
    while (i < size - 1) {
        if (array[i] != l.array[i])
            return false;

        ++i;
    }

    return true; // se l'esecuzione arriva qui, le due liste sono uguali
}

#endif
