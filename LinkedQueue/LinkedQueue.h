// By Fabio Nardelli
// coda FIFO basata su lista doppiamente concatenata con sentinella

#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class LinkedQueue;

template <typename T>
std::ostream &operator<<(std::ostream &, const LinkedQueue<T> &);

template <typename T>
class LinkedQueue {
	friend std::ostream &operator<< <>(std::ostream &, const LinkedQueue<T> &);
public:
	LinkedQueue() { create(); } // costruttore di default
	LinkedQueue(const LinkedQueue<T> &); // costruttore per copia
	~LinkedQueue(); // distruttore
	LinkedQueue<T> &operator=(const LinkedQueue<T> &);

	// operatori
	
	// restituisce "true" se la coda e' vuota, "false" altrimenti
	bool isEmpty() const { return head == head->next; }
	void print() const; // stampa la coda
	void enqueue(T); // inserisce un elemento in coda
	void dequeue(); // rimuove l'elemento da piu' tempo in coda
	T top() const; // restituisce l'elemento da piu' tempo in coda
	bool operator==(const LinkedQueue<T> &) const;
private:

	// rappresenta un generico nodo
	struct Node { 
		T value; // dato contenuto nel nodo
		Node *next; // puntatore al nodo successivo
		Node *previous; // puntatore al nodo precedente
	};

	Node *head; // puntatore al primo nodo (sentinella) della coda
	void create(); // crea una coda vuota
};

/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// overload dell'operatore di inserimento <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const LinkedQueue<T> &q) 
{
	q.print();
	return output; // consente cout << a << b << c
}

// utility function chiamata dal costruttore di default
template <typename T>
void LinkedQueue<T>::create()
{
	// crea una coda vuota, munita di sentinella (nodo head)
	head = new Node;
	head->next = head;
	head->previous = head;
}

// costruttore per copia
template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T> &q) 
{
	head = new Node;
	head->next = head;
	head->previous = head;
	
	Node *current = q.head->next;
	while (current != q.head) {
		enqueue(current->value);
		current = current->next;
	}
}

// distruttore
template <typename T>
LinkedQueue<T>::~LinkedQueue() 
{
	while (!isEmpty())
		dequeue();

	delete head;
}

// stampa la coda
template <typename T>
void LinkedQueue<T>::print() const
{
	if (isEmpty()) {
		std::cout << "\n Queue is empty!\n";
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
	std::cout << " ]\n";
} 

// inserisce un nodo in coda
template <typename T>
void LinkedQueue<T>::enqueue(T item) 
{
	// crea un nuovo nodo
	Node *newNode = new Node;
	newNode->value = item;
	// collega il nuovo nodo alla coda
	if (isEmpty()) { // se la coda e' vuota
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

// rimuove il nodo da piu' tempo in coda (FIFO)
template <typename T>
void LinkedQueue<T>::dequeue() 
{
	if (!isEmpty()) {
		Node *current = head->next;
		head->next = head->next->next;
		head->next->previous = head;
		delete current;
	}
}

// legge l'elemento da piu' tempo in coda
template <typename T>
T LinkedQueue<T>::top() const {
	if (isEmpty())
		throw std::domain_error("Queue is empty!");
	
	return head->next->value;
}

// overload dell'operatore di assegnazione =
template <typename T>
LinkedQueue<T> &LinkedQueue<T>::operator=(const LinkedQueue<T> &q) 
{
	if (&q != this) { // evita l'auto-assegnazione
	
		// cancella i nodi di questa coda
		while (!isEmpty())
			dequeue();

		// copia la coda q in questa coda
		Node *current = q.head->next;
		while (current != q.head) {
			enqueue(current->value);
			current = current->next;
		}	
	}
	
	return *this; // consente a = b = c
}

// overload dell'operatore di uguaglianza ==
template <typename T>
bool LinkedQueue<T>::operator==(const LinkedQueue<T> &q) const 
{
	// se le code sono entrambe vuote, allora sono uguali
	if (isEmpty() && q.isEmpty())
		return true;
	
	// se solo una delle due e' vuota, allora sono diverse
	// (il caso di entrambe le code vuote e' gestito sopra)
	if (isEmpty() || q.isEmpty())
		return false;
	
	// controllo rapido sul primo e l'ultimo elemento di ciascuna coda
	if (head->next->value != q.head->next->value
		|| head->previous->value != q.head->previous->value)
		return false;
	
	// controllo esaustivo su ciascun elemento
	Node *thisQueuePtr = head->next;
	Node *otherQueuePtr = q.head->next;
	while (thisQueuePtr->value == otherQueuePtr->value
		&& thisQueuePtr != head && otherQueuePtr != q.head) {
		thisQueuePtr = thisQueuePtr->next;
		otherQueuePtr = otherQueuePtr->next;
	}
	
	if (thisQueuePtr->value != otherQueuePtr->value)
		return false;
	
	return true;
}

#endif