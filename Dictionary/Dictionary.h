// By Fabio Nardelli
// Dizionario implementato mediante hash table con liste di trabocco (chaining).
// Al superamento delle soglie di fattore di carico maxLoadFactor e minLoadFactor,
// la tabella voene ristrutturata: nel primo caso viene allocato un vettore di
// dimensione doppia, nel secondo caso, di dimensione dimezzata, e tutti gli 
// elementi sono copiati dal vecchio al nuovo vettore ricalcolando i relativi
// valori hash.

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>
#include <stdexcept>
#include "../LinkedList/LinkedList.h"
#include "Hash.h"

template <typename K, typename V>
class Dictionary;

template <typename K, typename V>
std::ostream &operator<<(std::ostream &, const Dictionary<K, V> &);

template <typename K, typename V>
class Dictionary {
    friend std::ostream &operator<< <>(std::ostream &, const Dictionary<K, V> &);
public: 
    // costruttore di default
    Dictionary(std::size_t c = defaultCapacity)
        : size(0) { create(c); }
    // costruttore per copia
    Dictionary(const Dictionary<K, V> &d); 	
    // distruttore
    ~Dictionary() { delete [] table; }
    // operatore =
    Dictionary<K, V> &operator=(const Dictionary<K, V> &);

    // restituisce true se il dizionario e' vuoto, false altrimenti
    bool isEmpty() const { return size == 0; }
    // restituisce true se la chiave indicata e' presente nel dizionario,
    // false altrimenti
    bool belongs(const K&) const;
    // inserisce la coppia chiave-valore indicata nel dizionario
    void insert(K key, V value);
    // rimuove la coppia chiave-valore indicata dal dizionario
    void remove(K);
    // recupera il valore corrispondente alla chiave indicata
    const V *find(const K&) const;
    // restituisce il nuero di elementi contenuti nel dizionario
    std::size_t getSize() const { return size; }
    // stampa il dizionario
    void print() const;
    // stampa i valori contenuti nel dizionario
    void values() const;
    // stampa le chiavi trovato nel dizionario
    void keys() const;
    // restituisce true se almeno una chiave e' mappata al valore indicato
    bool containsValue(const V &) const;
private:

    // struct usata per rappresentare una coppia chiave-valore
    struct Pair {
        K key; // chiave
        V value; // valore
    };

    typedef LinkedList<Pair> PairList;
    typedef typename PairList::Position ListPosition;

    // vettore di liste di trabocco utilizzato per rappresentare
    // la tabella hash
    PairList *table;
    std::size_t size; // numero di elementi contenuti nel vettore
    std::size_t capacity; // dimensione massima del vettore
    // percio' il fattore di carico e' pari a size/capacity

    static const std::size_t defaultCapacity = 16;
    static const float maxLoadFactor;
    static const float minLoadFactor;
    void create(std::size_t); // funzione di inizializzazione
    void resize(std::size_t); // modifica la dimensione del vettore

    // funzione "wrapper": permette di chiamare funzioni hash esterne,
    // a patto che il loro tipo di ritorno sia unsigned int.
    // Poiche' la dimensione del vettore non e' necessariamente un numero
    // primo, e' sconsigliabile usare funzioni hash che abbiano questo requisito.
    // Poiche' il valore intero prodotto puo' essere molto grande,
    // per ricavare un indice valido le funzioni della classe Dictionary
    // calcolano il modulo tra questo valore e la dimensione del vettore stesso.
    // Attualmente chiama una funzione presente nel file Hash.h
    std::size_t hash(const K &k) const { return getHash<K>(k);}
};

// costante che rappresenta il massimo fattore di carico superato il quale
// la dimensione del vettore viene raddoppiata
template <typename K, typename V>
const float Dictionary<K, V>::maxLoadFactor = 2.0;
// costante che rappresenta il fattore di carico minimo al di sotto del quale
// la dimensione del vettore viene dimezzata
template <typename K, typename V>
const float Dictionary<K, V>::minLoadFactor = 0.125;


/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename K, typename V>
std::ostream &operator<<(std::ostream &output, const Dictionary<K, V> &d)
{
    d.print();
    return output;
}

// costruttore per copia
template <typename K, typename V>
Dictionary<K, V>::Dictionary(const Dictionary<K, V> &d)
{
    capacity = d.capacity;
    size = d.size;
    table = new PairList[capacity];

    for (std::size_t i = 0; i < capacity; ++i)
        table[i] = d.table[i];
}

// overload operatore di assegnamento =
template <typename K, typename V>
Dictionary<K, V> &Dictionary<K, V>::operator=(const Dictionary<K, V> &d)
{
    if (&d != this) { // evita l'autoassegnazione
        if (capacity != d.capacity) { // se i due array hanno dimensione
            delete [] table;          // diversa, dealloca quello di sinistra
            capacity = d.capacity;    // e alloca un nuovo array di capacita'
            // uguale a quello da copiare
            table = new PairList[capacity];
        }

        // copia gli elementi
        for (std::size_t i = 0; i < capacity; ++i)
            table[i] = d.table[i];

        size = d.size;  // setta il numero di elementi
    }

    return *this; // consente a = b = c
}

// alloca memoria per il vettore.
template <typename K, typename V>
void Dictionary<K, V>::create(unsigned int c)
{
    // crea un vettore di liste di trabocco di max capacity elementi
    if (c > 0)
        capacity = c;

    table = new PairList[capacity];
}

// verifica se la chiave indicata e' presente nel dizionario
template <typename K, typename V>
bool Dictionary<K, V>::belongs(const K &key) const
{
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Cannot find key: Dictionary is empty!\n";
        return false;
    }

    // calcola il valore hash per sapere quale lista potrebbe
    // contenere la chiave
    std::size_t h = hash(key) % capacity;
    ListPosition p = table[h].begin();

    // scandisce la lista alla ricerca della chiave
    while (!table[h].finished(p)) {
        if (table[h].read(p).key == key)
            return true;
        p = table[h].next(p);
    }

    return false; // se la chiave non e' presente
}

// utility function per modificare le dimensioni del vettore. Alloca un
// nuovo vettore di dimensioni newCapacity e copia il contenuto del vecchio
// vettore nel nuovo, dopodiche' cancella il vecchio vettore.
template <typename K, typename V>
void Dictionary<K, V>::resize(unsigned int newCapacity)
{
    if (newCapacity <= 0)
        throw std::invalid_argument("newCapacity must be > 0!");

    // alloca un nuovo vettore di dimensione newCapacity
    PairList *temp = new PairList[newCapacity];

    // ricalcola i valori hash di ciascun elemento del vettore
    // originale e copia tutti gli elementi nel nuovo vettore
    std::size_t h; // indice calcolato dalla funzione hash
    ListPosition pos;
    for (std::size_t i = 0; i < capacity; ++i) {
        if (!table[i].isEmpty()) {
            pos = table[i].begin();
            while (!table[i].finished(pos)) {
                h = hash(table[i].read(pos).key) % (newCapacity);
                temp[h].pushFront(table[i].read(pos));
                pos = table[i].next(pos);
            }
        }
    }
    capacity = newCapacity;
    delete [] table;
    table = temp;
}

// inserisce la coppia chiave-valore indicata nella tabella
// se e' gia' presente la stessa chiave, sovrascrive il valore associato
template <typename K, typename V>
void Dictionary<K, V>::insert(K key, V val)
{
    // calcola il valore hash per determinare la lista di trabocco
    // in cui inserire l'elemento
    std::size_t h = hash(key) % capacity;
    Pair element;
    element.key = key;
    element.value = val;

    // poiche' non sono ammessi duplicati di una stessa chiave,
    // cerca la chiave per vedere se e' gia' presente
    ListPosition p = table[h].begin();
    while (!table[h].finished(p)) {
        if (table[h].read(p).key == key) {
            // in tal caso sovrascrive l'elemento associato e termina
            table[h].write(p, element);
            return;
        }
        p = table[h].next(p);
    }

    // altrimenti aggiunge semplicemente l'elemento in testa alla lista
    // (in questo modo la ricerca dell'elemento inserito per ultimo nella
    // lista ha tempo costante)
    table[h].pushFront(element);
    ++size;

    // se il fattore di carico supera la soglia maxLoadFactor, raddoppia
    // le dimensioni del vettore
    if (static_cast<float>(size) / capacity > maxLoadFactor)
        resize(capacity * 2);
}

// rimuove la coppia chiave-valore indicata dalla tabella, se presente
template <typename K, typename V>
void Dictionary<K, V>::remove(K key)
{
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Cannot remove item: Dictionary is empty!\n";
        return;
    }

    // calcola il valore hash per determinare la lista di trabocco
    // che potrebbe contenere l'elemento
    std::size_t h = hash(key) % capacity;
    ListPosition p = table[h].begin();

    while (!table[h].finished(p)) {
        if (table[h].read(p).key == key) {
            table[h].remove(p);
            --size;
            // se il fattore di carico scende sotto la soglia minLoadFactor, dimezza
            // le dimensioni del vettore
            if (static_cast<float>(size) / capacity < minLoadFactor)
                resize(capacity / 2);

            return;
        }
        p = table[h].next(p);
    }
    // se l'elemento non e' presente
    std::cerr << "Cannot remove item: key not found!\n";
}

// recupera il valore corrispondente alla chiave indicata
template <typename K, typename V>
const V *Dictionary<K, V>::find(const K &key) const
{
    // calcola il valore hash per determinare la lista di trabocco
    // che potrebbe contenere l'elemento
    std::size_t h = hash(key) % capacity;
    ListPosition p = table[h].begin();
    if (!table[h].isEmpty()) {

        // scandisce la lista alla ricerca dell'elemento
        while (!table[h].finished(p) &&
                table[h].read(p).key != key) {
            p = table[h].next(p);
        }
        // se lo trova, restituisce un puntatore costante
        // all'oggetto
        if (table[h].read(p).key == key)
            return &table[h].read(p).value;
    }
    // altrimenti, restituisce NIL
    return 0;
}

// stampa il dizionario
template <typename K, typename V>
void Dictionary<K, V>::print() const
{	
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Dictionary is empty!\n";
        return;
    }

    ListPosition p; // per scandire una lista di trabocco
    Pair current; // elemento(chiave-valore) corrente
    std::cout << "KEYS : VALUES\n";
    for (std::size_t i = 0; i < capacity; ++i) {
        if (!table[i].isEmpty()) {
            for(p = table[i].begin(); !table[i].finished(p);
                    p = table[i].next(p)) {
                current = table[i].read(p);
                std::cout << current.key << " : " << current.value << "\n";
            }
        }
    }
}

// stampa i valori contenuti nel dizionario
template <typename K, typename V>
void Dictionary<K, V>::values() const
{	
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Dictionary is empty!\n";
        return;
    }

    ListPosition p; // per scandire una lista di trabocco
    V currentValue; // valore corrente

    for (std::size_t i = 0; i < capacity; ++i) {
        if (!table[i].isEmpty()) {
            for(p = table[i].begin(); !table[i].finished(p);
                    p = table[i].next(p)) {
                currentValue = table[i].read(p).value;
                std::cout << currentValue << "\n";
            }
        }
    }
}

// stampa le chiavi contenute nel dizionario
template <typename K, typename V>
void Dictionary<K, V>::keys() const
{	
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Dictionary is empty!\n";
        return;
    }

    ListPosition p; // per scandire una lista di trabocco
    K currentKey; // chiave corrente

    for (std::size_t i = 0; i < capacity; ++i) {
        if (!table[i].isEmpty()) {
            for(p = table[i].begin(); !table[i].finished(p);
                    p = table[i].next(p)) {
                currentKey = table[i].read(p).key;
                std::cout << currentKey << "\n";
            }
        }
    }
}

// restituisce true se almeno una chiave e' mappata al valore indicato
template <typename K, typename V>
bool Dictionary<K, V>::containsValue(const V &val) const
{	
    if (isEmpty()) { // controlla se il dizionario e' vuoto
        std::cerr << "Dictionary is empty!\n";
        return false;
    }

    ListPosition p; // per scandire una lista di trabocco

    for (std::size_t i = 0; i < capacity; ++i) {
        if (!table[i].isEmpty()) {
            for(p = table[i].begin(); !table[i].finished(p);
                    p = table[i].next(p)) {
                if	(table[i].read(p).value == val)
                    return true;
            }
        }
    }

    return false; // se il valore non e' contenuto
}

#endif
