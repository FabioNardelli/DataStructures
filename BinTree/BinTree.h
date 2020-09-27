// By Fabio Nardelli
// realizzazione di albero binario mediante puntatori

#ifndef BINTREE_H
#define BINTREE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Node.h"
#include "../LinkedQueue/LinkedQueue.h"

template <typename T>
class BinTree;

template <typename T>
std::ostream &operator<<(std::ostream &, BinTree<T> &);

template <typename T>
class BinTree {
	friend std::ostream &operator<< <>(std::ostream &, BinTree<T> &);
public:	
	BinTree() { create(); } // costruttore di default
	BinTree(const BinTree<T> &); // costruttore per copia
	~BinTree() { deleteSubTree(rootPtr); } // distruttore
	BinTree<T> &operator=(const BinTree<T> &);

	// operatori	
	// crea un albero vuoto
	void create() { rootPtr = 0; } 
	// restituisce true se l'albero e' vuoto, false altrimenti
	bool isEmpty() const { return rootPtr == 0; } 
	// restituisce il nodo radice dell'albero
	Node<T> *root() const { return rootPtr; }
	// restituisce il nodo genitore del nodo indicato
	Node<T> *parent(Node<T> *n) const { return n->parent; }
	// restituisce il figlio sinistro del nodo indicato
	Node<T> *leftChild(Node<T> *n) const { return n->left; }
	// restituisce il figlio destro del nodo indicato
	Node<T> *rightChild(Node<T> *n) const { return n->right; }
	// restituisce true se il nodo indicato non ha un figlio sinistro,
	// false altrimenti
	bool emptyLeft(Node<T> *n) const { return n->left == 0; }
	// restituisce true se il nodo indicato non ha un figlio destro,
	// false altrimenti
	bool emptyRight(Node<T> *n) const { return n->right == 0; }
	// inserisce un nodo come radice dell'albero
	void insertRoot(Node<T> *);
	// aggiunge un figlio sinistro al nodo indicato
	void insertLeft(Node<T> *); 
	// aggiunge un figlio destro al nodo indicato
	void insertRight(Node<T> *);
	// cancella il sottoalbero avente radice nel nodo indicato
	void deleteSubTree(Node<T> *);
	// legge il dato contenuto nel nodo indicato
	T read(Node<T> *) const;
	// scrive nel nodo indicato
	void write(T, Node<T> *);
	// consente di costruire un albero dal basso, cioe' a partire dalle foglie
	void buildTree(BinTree<T> &, BinTree<T> &);
	// inserisce un nodo nel primo posto libero, mediante BFS
	void insert(T);
	// funzioni di servizio
	void printSubTree(Node<T> *) const;
	void print() const;
	int height(Node<T> *) const; // restituisce l'altezza del nodo
	int count(Node<T> *) const;  // restituisce il numero dei nodi discendenti
		                         // del nodo indicato	
	// algoritmi di visita
	// visite in profondita' (DFS)
	void preOrder(Node<T> *) const;
	void inOrder(Node<T> *) const;
	void postOrder(Node<T> *) const;
	// visita in ampiezza (BFS)
	void bfs(Node<T> *) const;
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
std::ostream &operator<<(std::ostream &output, BinTree<T> &t) 
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
// nuovo nodo; dopodiche', se source ha un figlio sinistro, alloca memoria 
// per un nuovo nodo figlio sinistro di copy e copia il valore dal figlio
// corrispondente di source. Ripete lo stesso per un eventuale figlio
// destro di source. Infine chiama ricorsivamente la procedura sui figli
// sinistro e destro di source. Il controllo (*copy == 0) e' necessario
// per evitare che, nelle chiamate successive alla prima, si tenti di
// allocare memoria dove e' gia' presente un nodo. 
// Alla prima chiamata, e' necessario che la condizione *copy == 0 sia
// soddisfatta, cioe' che l'albero copia sia vuoto, affinche' la copia
// avvenga correttamente. E' responsabilita' del chiamante accertarsene! 
template <typename T>
void BinTree<T>::copyTree(Node<T> **copy, const Node<T> *source)
{
	if (source != 0) {
		// *copy == 0 solo alla prima chiamata,
		// quando l'albero copia e' vuoto.
		if (*copy == 0)  
			*copy = new Node<T>(source->value); 
		                       // usa il costruttore per impostare dato e parent
		if (source->left != 0) // Node<T>(dato, parent)
			(*copy)->left = new Node<T>(source->left->value, *copy);
		
		if (source->right != 0) // idem
			(*copy)->right = new Node<T>(source->right->value, *copy);
		
		copyTree(&(*copy)->left, source->left);
		copyTree(&(*copy)->right, source->right);
	}
}

// costruttore per copia
template <typename T>
BinTree<T>::BinTree(const BinTree<T> &t)
{
	if (!t.isEmpty()) 
		copyTree(&rootPtr, t.root());
	else	
		rootPtr = 0;
}

// overload dell'operatore di assegnazione =
template <typename T>
BinTree<T> &BinTree<T>::operator=(const BinTree<T> &t)
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
void BinTree<T>::insertRoot(Node<T> *n)
{
	if (isEmpty()) {
		rootPtr = n;
		rootPtr->setLeft(0);
		rootPtr->setRight(0);
		rootPtr->setParent(0);
	}
}

// aggiunge un figlio sinistro al nodo indicato
template <typename T>
void BinTree<T>::insertLeft(Node<T> *n) 
{
	if (emptyLeft(n)) {
		n->setLeft(new Node<T>);
		n->getLeft()->setParent(n);
		n->getLeft()->setLeft(0);
		n->getLeft()->setRight(0);
	}
}

// aggiunge un figlio destro al nodo indicato
template <typename T>
void BinTree<T>::insertRight(Node<T> *n) 
{
	if (emptyRight(n)) {
		n->setRight(new Node<T>);
		n->getRight()->setParent(n);
		n->getRight()->setLeft(0);
		n->getRight()->setRight(0);
	}
}

// cancella il sottoalbero avente radice nel nodo indicato
template <typename T>
void BinTree<T>::deleteSubTree(Node<T> *n) 
{
	if (n != 0) {
		if (!emptyLeft(n))
			deleteSubTree(leftChild(n));
		if (!emptyRight(n))
			deleteSubTree(rightChild(n));
		if (n != rootPtr) {
			Node<T> *p = parent(n);
			if (leftChild(p) == n)
				p->setLeft(0);
			else
				p->setRight(0);
			delete n;
		} else { 
			rootPtr = 0;
		}	
	}
}

// legge l'etichetta del nodo indicato
template <typename T>
T BinTree<T>::read(Node<T> *n) const 
{
	if (n == 0)
		throw std::invalid_argument("Invalid node!");
	
	return n->getValue();
}

// scrive nell'etichetta del nodo indicato
template <typename T>
void BinTree<T>::write(T item, Node<T> *n) 
{
	if (n != 0)
		n->setValue(item);
}

// stampa il sottoalbero di radice nel nodo indicato (pre-visita)
template <typename T>
void BinTree<T>::printSubTree(Node<T> *n) const 
{
	if (n != 0) {
		std::cout << "[" << std::setw(3) << n->getValue() << "  ";
		if (!emptyLeft(n))
			printSubTree(n->getLeft());
		else 
			std::cout << " NIL ";
		if (!emptyRight(n))
			printSubTree(n->getRight());
		else 
			std::cout << " NIL ";
		std::cout << "]";
	}
}

// stampa l'albero
template <typename T>
void BinTree<T>::print() const 
{
	if (!isEmpty()) {
		std::cout << "\n\n [";
		printSubTree(rootPtr);
		std::cout << "]\n";
	} else {
		std::cout << "\n Tree is empty!\n";
	}
}

// mostra l'altezza del nodo indicato
template <typename T>
int BinTree<T>::height(Node<T> *n) const 
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
int BinTree<T>::count(Node<T> *n) const 
{
	if (n == 0) 
		return 0;
	// effettua una post-visita
	int leftCount = count(n->getLeft());
	int rightCount = count(n->getRight());
	return leftCount + rightCount + 1;
}

// visita (e stampa) l'albero in pre-ordine
template <typename T>
void BinTree<T>::preOrder(Node<T> *n) const
{
	if (n != 0) {
		std::cout << n->getValue() << "\n";
		preOrder(n->getLeft());
		preOrder(n->getRight());
	}
}

// visita (e stampa) l'albero in ordine
template <typename T>
void BinTree<T>::inOrder(Node<T> *n) const
{
	if (n != 0) {
		inOrder(n->getLeft());
		std::cout << n->getValue() << "\n";
		inOrder(n->getRight());
	}
}

// visita (e stampa) l'albero in post-ordine
template <typename T>
void BinTree<T>::postOrder(Node<T> *n) const
{
	if (n != 0) {
		postOrder(n->getLeft());
		postOrder(n->getRight());
		std::cout << n->getValue() << "\n";
	}
}

// visita (e stampa) l'albero in ampiezza
// Algoritmo iterativo che fa uso di una coda
template <typename T>
void BinTree<T>::bfs(Node<T> *n) const
{
	if (n != 0) {
		Node<T> *current;
		LinkedQueue<Node<T> *> q;
		q.enqueue(n);
		while (!q.isEmpty()) {
			current = q.top();
			std::cout << read(current) << "\n";
			q.dequeue();
			if (!emptyLeft(current))
				q.enqueue(leftChild(current));
			if (!emptyRight(current))
				q.enqueue(rightChild(current));
		}
	}
}

// crea un albero binario formato da una radice avente per
// figlio sinistro e destro rispettivamente i due sottoalberi
// forniti in input 
template <typename T>
void BinTree<T>::buildTree(BinTree<T> &t1, BinTree<T> &t2)
{
	if (isEmpty()) {
		rootPtr = new Node<T>; // alloca memoria per la radice
		if (t1.root() != 0) {
			rootPtr->setLeft(t1.root());
			t1.root()->setParent(rootPtr);
		}	
		if (t2.root() != 0) {
			t2.root()->setParent(rootPtr);
			rootPtr->setRight(t2.root());
		}
	}
}

// inserisce un nodo nel primo posto libero, visitando
// l'albero in ampiezza.
template <typename T>
void BinTree<T>::insert(T item)
{
	if (isEmpty()) {
		rootPtr = new Node<T>(item);
	} else {
		Node<T> *current = rootPtr;
		LinkedQueue<Node<T> *> q;
		q.enqueue(current);
		while(!q.isEmpty()) {
			current = q.top();
			q.dequeue();
			if (emptyLeft(current)) {
				current->left = new Node<T>(item, current);
				return;
			} else {
				q.enqueue(current->left);
			}
			
			if (emptyRight(current)) {
				current->right = new Node<T>(item, current);
				return;
			} else {
				q.enqueue(current->right);
			}
		}
	}
}

#endif