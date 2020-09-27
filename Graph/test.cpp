#include <iostream>
#include <string>
#include "Graph.h"
using std::cout;
using std::string;

void printNodeList(const LinkedList<Node<int>*> &);

int main()
{
	Graph<int, int> g; // crea un grafo vuoto
	
	// crea una serie di nodi
	Node<int> n1, n2(2), n3(3), n4(4), n5(5), n6(6);
	
	cout << "\n************** TEST CLASSE Graph **************\n\n"
		<< "Grafo g appena creato\n";
	cout << "num. nodi: " << g.numNodes() << "\nnum. archi: " << g.numEdges();
	g.isEmpty() ? cout << "\nIl grafo e' vuoto\n" : 
		cout << "\nIl grafo non e' vuoto\n";
	
	// inserisce i nodi nel grafo
	g.addNode(n1);
	g.addNode(n2);
	g.addNode(n3);
	g.addNode(n4);

	// crea degli archi fra i nodi, con peso uguale a 0
	g.addEdge(n1, n2, 0);               
	g.addEdge(n1, n3, 0);                
	g.addEdge(n1, n4, 0);               
	g.addEdge(n2, n3, 0);              
	g.addEdge(n3, n1, 0);
	g.addEdge(n4, n2, 0);
	g.addEdge(n4, n3, 0);
	
	
	cout << "\nAggiunti nodi e archi al grafo:\ngrafo orientato con 4 nodi"
		<< " con etichette nessuna, 2, 3 e 4\ne archi (1,2), (1,3), (1,4),"
		<< " (2,3), (3,1), (4,2), (4,3)\n";
	cout << "num. nodi: " << g.numNodes() << "\nnum. archi: " << g.numEdges();
	
	// test funzione hasNode
	cout << "\n\nFUNZIONE hasNode";
	g.hasNode(n1) ? cout << "\nn1 appartiene al grafo\n" : 
		cout << "\nn1 non appartiene al grafo\n";
	g.hasNode(n5) ? cout << "n5 appartiene al grafo\n" : 
		cout << "n5 non appartiene al grafo\n";
	
    // test funzione hasEdge	
	cout << "\nFUNZIONE hasEdge";
	g.hasEdge(n2, n3) ? cout << "\nl'arco (n2,n3) appartiene al grafo\n" : 
		cout << "\nl'arco (n2, n3) non appartiene al grafo\n";
	g.hasEdge(n2, n1) ? cout << "l'arco (n2,n1) appartiene al grafo\n" : 
		cout << "l'arco (n2,n1) non appartiene al grafo\n";
	
	// test funzione writeNode
	cout << "\nFUNZIONE writeNode\nscritto valore '1' in n1";
	g.writeNode(n1, 1);
	
	// test funzione readNode
	cout << "\n\nFUNZIONE readNode\n";
	cout << "etichetta nodo n1: " << g.readNode(n1) << "\n";
	
	// test funzione readEdge
	cout << "\nFUNZIONE readEdge\n";
	cout << "peso arco (n1,n2): " << g.readEdge(n1, n2) << "\n";
	
	// test funzione neighbors
	cout << "\nFUNZIONE neighbors\n";
	LinkedList<Node<int>*> l = g.neighbors(n1);
	cout << "nodi adiacenti a n1:\n";
	
	printNodeList(l);
	
	// test funzione getNodes
	cout << "\nFUNZIONE getNodes\n";
	l = g.getNodes();
	cout << "nodi nel grafo:\n";
	
	printNodeList(l);
	
	// test funzione writeEdge
	cout << "\nFUNZIONE writeEdge\nscritto peso '10' nell'arco (n1,n2)\n";
	g.writeEdge(n1, n2, 10);
	cout << "peso arco (n1,n2): " << g.readEdge(n1, n2) << "\n";
	
	// test funzione removeNode
	g.removeNode(n2);
	cout << "\nFUNZIONE removeNode\nrimosso nodo n2\n";
	
	g.hasNode(n2) ? cout << "n2 appartiene al grafo\n" : 
		cout << "n2 non appartiene al grafo\n";
	l = g.getNodes();
	cout << "nodi nel grafo:\n";
	
	printNodeList(l);
	
	// test funzione removeEdge
	g.removeEdge(n1, n3);
	cout << "\nFUNZIONE removeEdge\nrimosso arco (n1,n3)\n";
	g.hasEdge(n1, n3) ? cout << "l'arco (n1,n3) appartiene al grafo\n" : 
		cout << "l'arco (n1,n3) non appartiene al grafo\n";
	
	cout << "num. nodi: " << g.numNodes() << "\nnum. archi: " << g.numEdges();


	// mantiene aperto il terminale
	int x;
	cout << "\npremere un tasto per continuare...";
	std::cin >> x;
	
	return 0;
}

// funzione che stampa i nodi contenuti in una lista di (puntatori a) nodi
void printNodeList(const LinkedList<Node<int>*> &l)
{
	LinkedList<Node<int>*>::Position pos = l.begin();
	while (!l.finished(pos)) {
		cout << *l.read(pos) << " ";
		pos = l.next(pos);
	}
}