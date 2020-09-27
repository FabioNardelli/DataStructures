#include <iostream>
#include "ArrayQueue.h"
using std::cout;

int main()
{
	ArrayQueue<int> queue;
	
	cout << "\n************** TEST CLASSE ArrayQueue **************\n\n";
	
	// stampa una coda vuota
	cout << "Crea e tenta di stampare una coda vuota:\n";
	queue.print();
	
	// inserisce tre elementi nella coda
    queue.enqueue(1);
	queue.enqueue(2);
	queue.enqueue(3);

	// stampa la coda usando l'operatore <<
	cout << "\nFUNZIONE enqueue:\ninserisce un elemento nella coda."
		<< "\nInseriti(nell'ordine) elementi 1, 2 e 3\n" << queue;
	
	// operatore top
	cout << "\nFUNZIONE top";
	cout << "\nL'elemento da piu' tempo in coda e': " << queue.top();
	
	// rimozione elementi
	queue.dequeue();
	cout << "\n\nFUNZIONE dequeue\nRimosso elemento da piu' tempo in coda"
		<< queue;
	queue.dequeue();
	cout << "Rimosso elemento da piu' tempo in coda" << queue;
	queue.dequeue();
	cout << "Rimosso elemento da piu' tempo in coda" << queue;
	
	// operatore ==
	ArrayQueue<int> queue2, queue3;
	queue2.enqueue(45);
	
	cout << "\nOPERATORE ==\nCrea una coda 'queue2'\nqueue2:" << queue2;
	cout << "queue == queue2:\n";
	if (queue == queue2)
		cout << "true\n";
	else
		cout << "false\n";
	
	// operatore =
	queue3 = queue2;
	
	cout << "\nOPERATORE DI ASSEGNAMENTO =\nCrea una coda 'queue3'\n";
	cout << "queue3 = queue2\nqueue3:" << queue3;
	cout << "queue3 == queue2:\n";
	if (queue3 == queue2)
		cout << "true\n";
	else
		cout << "false\n";

	// mantiene aperto il terminale
	int x;
	cout << "\npremere un tasto per continuare...";
	std::cin >> x;

	
	return 0;
}