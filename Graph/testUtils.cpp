#include <iostream>
#include "Graph.h"
#include "GraphUtils.h"
using std::cout;

int main()
{
	Graph<int, int> g; 
	
	Node<int> n1(1), n2(2), n3(3), n4(4), n5(5), n6(6);
	
	// popola il grafo
	g.addNode(n1);
	g.addNode(n2);
	g.addNode(n3);
	g.addNode(n4);
	g.addNode(n5);
	g.addNode(n6);
	
	g.addEdge(n1, n2, 1);
	g.addEdge(n1, n3, 2);
	g.addEdge(n2, n5, 1);
	g.addEdge(n3, n2, 3);
	g.addEdge(n3, n4, 3);
	g.addEdge(n5, n4, 2);
	g.addEdge(n5, n6, 5);
	g.addEdge(n4, n6, 2);
	
	cout << "Grafo creato:\n(arco, peso)\n";
	cout << " 1->2, 1\n 1->3, 2\n 3->2, 3\n 2->5, 1\n 3->4, 3\n 5->4, 2\n"
		<< " 5->6, 5\n 4->6, 2\n\n";
	
	
	// visita dfs
	cout << "DFS (sorgente = 1):\n";
	dfs(g, n1);
	
	// visita bfs
	cout << "\n\nBFS (sorgente = 1):\n";
	bfs(g, n1);

	// mostra i cammini minimi ottenuti con l'algoritmo di Johnson
	
	// albero cammini minimi implementato come vettore dei padri
	Node<int> *T[6];
	
	johnson(g, n1, T);
	cout << "\n\nCAMMINI MINIMI\nJOHNSON\ncammino minimo da 1 a 6:\n";
	printPath(n1, n6, T);
	cout << "\ncammino minimo da 5 a 6:\n";
	printPath(n5, n6, T);
	
	bellman(g, n1, T);
	cout << "\nBELLMAN-FORD-MOORE\ncammino minimo da 1 a 6:\n";
	printPath(n1, n6, T);
	cout << "\ncammino minimo da 5 a 6:\n";
	printPath(n5, n6, T);
	
	// mantiene aperto il terminale
	int x;
	cout << "\n\npremere un tasto per continuare...";
	std::cin >> x;
	
	return 0;
}