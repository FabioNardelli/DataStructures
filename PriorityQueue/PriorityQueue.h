// By Fabio Nardelli
// coda con priorita' di tipo min-priority-queue basata su heap 

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class PriorityQueue;

template <typename T>
std::ostream &operator<<(std::ostream &, const PriorityQueue<T> &);

template <typename T>
class PriorityQueue {
	friend std::ostream &operator<< <>(std::ostream &, const PriorityQueue<T> &);
public:
	// costruttore di default
	PriorityQueue(int c = defaultCapacity) : size(0) { create(c); } 	
	// costruttore per copia
	PriorityQueue(const PriorityQueue<T> &);
	// distruttore
	~PriorityQueue() { delete [] heap; }
	// overload dell'operatore di assegnamento =
	PriorityQueue<T> &operator=(const PriorityQueue<T> &);

	// operatori
	
	// restituisce true se la coda e' vuota, false altrimenti
	bool isEmpty() const { return size == 0; }
	// restituisce il numero di elementi contenuti nella coda
	int getSize() const { return size; }
	// inserisce un elemento nella coda
	void insert(T, int);
	// restituisce l'elemento con la priorita' minima
	const T&min() const;
	// cancella l'elemento con la priorita' minima
	void deleteMin();
	// diminuisce la priorita' dell'elemento indicato portandola a p
	void decrease(const T &, int p);
	// stampa il contenuto della coda
	void print() const;
private:

	struct PriorityItem { // struttura per rappresentare un elemento
		T value;          // dello heap 
		int priority;
		int pos;		
	};
    
	PriorityItem *heap; // puntatore usato per rappresentare uno heap
	static const int defaultCapacity = 16;
	int capacity; // numero massimo di elementi memorizzabili
	int size; // numero di elementi correnti
	
	void create(int); // funzione di inizializzazione
	
	// restituisce l'indice del genitore del nodo corrente
	int parent(int n) const { return (n - 1)  / 2; }
	// restituisce l'indice del figlio sinistro del nodo corrente
	int left(int n) const { return 2 * n + 1; }
	// restituisce l'indice del figlio destro del nodo corrente
	int right(int n) const { return 2 * n + 2; }
	
	// utility function per scambiare due elementi dello heap
	void swap(int, int);
	// ripristina la proprieta' min-heap dopo la cancellazione del minimo
	void minHeapRestore(int);
	
	// funzione per modificare le dimensioni dello heap
	void resize(int);
	
	// soglia di fattore di carico per dimezzare il vettore
	static const float minLoadFactor;
};

template <typename T>
const float PriorityQueue<T>::minLoadFactor = .25;


/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const PriorityQueue<T> &h)
{
    h.print();
	return output; // consente a << b << c
}

// crea una coda di priorita' vuota
template <typename T>
void PriorityQueue<T>::create(int c)
{
	if (c > 0)
		capacity = c;
	
	heap = new PriorityItem[capacity];
}

// costruttore per copia
template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T> &h)
{
	capacity = h.capacity;
	size = h.size;
	heap = new PriorityItem[capacity];
	
	for (int i = 0; i < capacity; ++i)
		heap[i] = h.heap[i];
}

// overload dell'operatore di assegnamento =
template <typename T>
PriorityQueue<T> &PriorityQueue<T>::operator=(const PriorityQueue<T> &h)
{
	if (&h != this) { // evita l'autoassegnazione
		if (capacity != h.capacity) { // se i due array hanno capacita'
			delete [] heap;           // diversa, dealloca quello di sinistra
			capacity = h.capacity;    // e alloca un nuovo array di capacita'
                                      // uguale a quello da copiare 			
			heap = new PriorityItem[capacity]; 
		}
		
		size = h.size;  // setta il numero di elementi
		
		// copia gli elementi
		for (int i = 0; i < size; ++i)
			heap[i] = h.heap[i];
	}
	
	return *this; // consente a = b = c
}

// utility function per scambiare due elementi dello heap
template <typename T>
void PriorityQueue<T>::swap(int i, int j)
{
	PriorityItem temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
	
	// necessarie per consentire la modifica della priorita'
	heap[i].pos = i; 
	heap[j].pos = j;
}

// utility function per modificare le dimensioni del vettore. Alloca un
// nuovo vettore di dimensioni newCapacity e copia il contenuto del vecchio
// vettore nel nuovo, dopodiche' cancella il vecchio vettore.
template <typename T>
void PriorityQueue<T>::resize(int newCapacity)
{
	if (newCapacity <= 0)
	    throw std::invalid_argument("newCapacity must be > 0!");	
	
	PriorityItem *temp = new PriorityItem[newCapacity];
	for (int i = 0; i < size; ++i)
		temp[i] = heap[i];
	
	capacity = newCapacity;
	delete [] heap;
	heap = temp;
}

// inserisce un elemento nella coda
template <typename T>
void PriorityQueue<T>::insert(T value, int p)
{
	// se non c'e' spazio nel vettore, raddoppia le dimensioni
	if (size == capacity)  
		resize(capacity * 2);
	
	heap[size].value = value;
	heap[size].priority = p;
	heap[size].pos = size;
		
	++size;
		
	int i = size - 1;
	while (i > 0 && heap[i].priority < heap[parent(i)].priority) {
		swap(i, parent(i));
		i = parent(i);
	}
}

// restituisce il minimo (senza cancellarlo)
template <typename T>
const T&PriorityQueue<T>::min() const
{
	if (isEmpty())
		throw std::domain_error("Queue is Empty!");
	
	return heap[0].value;
}

// ripristina la proprieta' min-heap
template <typename T>
void PriorityQueue<T>::minHeapRestore(int i)
{
	// indice dell'elemento con la priorita' piu' bassa
	int min = i;  // inizialmente indica il nodo passato per parametro
	
	// se esiste un figlio sinistro e la sua priorita' e' minore di quella
	// del genitore, min indica il figlio sinistro
	if (left(i) < size && heap[left(i)].priority < heap[min].priority)
		min = left(i);
	
	// se esiste un figlio destro e la sua priorita' e' minore di quella
	// del minimo corrente(potrebbe essere il genitore o il fratello sinistro),
	// min indica il figlio destro
	if (right(i) < size && heap[right(i)].priority < heap[min].priority)
		min = right(i);
	
	if (i != min) {          // se i != min, scambia il nodo di indice i 
		swap(i, min);        // con il figlio con la priorita' piu' bassa 
		minHeapRestore(min); // (quello con indice min), e chiama ricorsivamente 
	}                        // la procedura sul nuovo nodo di indice min
}

// cancella l'elemento con la priorita' piu' bassa (la radice)
template <typename T>
void PriorityQueue<T>::deleteMin()
{
	if (isEmpty()) {
		std::cerr << "Cannot delete: Queue is empty!\n";
		return;
	}
	
	heap[0] = heap[size - 1];
	--size;
	minHeapRestore(0);
	
	// se il fattore di carico raggiunge la soglia minLoadFactor 
	// (di default 1/4), dimezza le dimensioni del vettore
	if (static_cast<float>(size) / capacity <= minLoadFactor) 
	    resize(capacity / 2);	
}

// diminuisce la priorita' dell'elemento indicato portandola a p
template <typename T>
void PriorityQueue<T>::decrease(const T &value, int p)
{
	// cerca l'elemento al quale diminuire la priorita'
	int i = 0;
	while (heap[i].value != value && i < size) { ++i; }
	
	if (heap[i].priority <= p)  // la nuova priorita' deve essere
		return;                 // piu' bassa della vecchia
	
	heap[i].priority = p;
	
	// ripristina la proprieta' min-heap
	i = heap[i].pos;
	while (i > 0 && heap[i].priority < heap[parent(i)].priority) {
		swap(i, parent(i));
		i = parent(i);
	}
}

// stampa la coda. Gli elementi sono mostrati nel formato (valore, priorita')
template <typename T>
void PriorityQueue<T>::print() const
{
	if (isEmpty()) {
		std::cerr << "Queue is empty!\n";
		return;
	}
	
	std::cout << "\n [";
	
	for (int i = 0; i < size; ++i) {
		std::cout << std::setw(3) << "(" << heap[i].value 
			<< "," << heap[i].priority << ") ";
		// va a capo ogni 5 elementi, tranne dopo l'ultimo
		if ((i + 1) % 5 == 0 && i != size - 1) 
			std::cout << "\n  ";
	}
	std::cout << " ]\n";
}

#endif