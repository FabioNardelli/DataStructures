#include <iostream>
#include "BinTree.h"
using std::cout;

int main()
{
	BinTree<int> tree;
	
	cout << "\n************** TEST CLASSE BinTree **************\n\n";
	// costruzione dall'alto verso il basso
	// stampa un albero vuoto
	cout << "COSTRUZIONE DALL'ALTO VERSO IL BASSO\n"
		<< "Crea un albero tree\ntree:";
	tree.print();
	
	// crea un nodo e lo inserisce nell'albero come radice
	Node<int> *root = new Node<int>;
	root->setValue(1);
	tree.insertRoot(root);
	
	// stampa l'albero
	cout << "\nInserita radice con etichetta 1: ";
	tree.print();
	cout << "\n";
	
	// crea due nodi figlio sinistro e destro      //      1       //
	tree.insertLeft(tree.root());                  //    /   \     //
 	tree.write(2, tree.leftChild(tree.root()));	   //   2     3    // 
	tree.insertRight(tree.root());                 //              // 
	tree.write(3, tree.rightChild(tree.root()));   //              //

	// stampa l'albero usando l'operatore << in overload
	cout << "Aggiunti due nodi come figli sinistro" 
		<< " e destro \ndella radice, con etichette"
		<< " 2 e 3:" << tree;
	
	// aggiunge altri due nodi                     //      1        //
	Node<int> *current =                           //    /   \      //
		tree.rightChild(tree.root());              //   2     3     //
	tree.insertLeft(current);                      //       /   \   //
	tree.write(5, tree.leftChild(current));        //      5     6  //
 	tree.insertRight(current);                     //               //
	tree.write(6, tree.rightChild(current));       //               //
	
	// stampa l'albero
	cout << "\nAggiunti altri due nodi con etichette"
		<< " 5 e 6" << tree << "\n";
	
	// mostra l'altezza dei nodi con etichette 
	// 1, 3 e 5
	cout << "PROPRIETA' DELL'ALBERO\n"
		<< "Altezza nodo con etichetta 1: "
		<< tree.height(tree.root())
		<< "\nAltezza nodo con etichetta 3: "
		<< tree.height(current)
		<< "\nAltezza nodo con etichetta 5: "
		<< tree.height(tree.leftChild(current)) << "\n";
		
	// stampa il numero di nodi dei sottoalberi aventi radice 
    // nei nodi con etichetta 2 e 3	
	cout << "\nN. nodi nel sottoalbero con etichetta 2: "
		<< tree.count(tree.leftChild(tree.root()))
		<< "\nN. nodi nel sottoalbero con etichetta 3: "
		<< tree.count(current) << "\n";
	
	// visite in profondita' (Depth First Search)
	cout << "\nVISITE IN PROFONDITA' (DFS)\n" 
		<< "\nVisita in pre-ordine:\n";
	tree.preOrder(tree.root());
	cout << "\nVisita in ordine:\n";
	tree.inOrder(tree.root());
	cout << "\nVisita in post-ordine:\n";
	tree.postOrder(tree.root());
	
	// visita in ampiezza (BFS)
	cout << "\nVISITA IN AMPIEZZA (BFS)\n";
	tree.bfs(tree.root());
	
	// cancella il sottoalbero avente radice       //      1        //
    // nel nodo con etichettta 3	               //    /          //
 	tree.deleteSubTree(current);                   //   2           //
	
	// stampa l'albero
	cout << "\nELIMINAZIONE NODI\nEliminazione sottoalbero"
		<< " con etichetta 3:" << tree;
	
	// testa l'operatore =
	BinTree<int> tree1;
	
	tree1 = tree;
	
	cout << "\nOPERATORE DI ASSEGNAZIONE\n"
		<< "crea un albero tree1\ntree1 = tree\ntree1:" << tree1;

	tree1.deleteSubTree(tree1.root());
	
	cout << "\nELIMINAZIONE ALBERO\nEliminazione tree1\ntree1: " << tree1;
	
	// costruzione dal basso verso l'alto
	BinTree<int> tree2, t1, t2;
	
	cout << "\nCOSTRUZIONE DAL BASSO VERSO L'ALTO\n";
	Node<int> r1, r2;
	t1.insertRoot(&r1); // crea due sottoalberi...
	t2.insertRoot(&r2); 
	t1.write(23, t1.root());
	t2.write(2, t2.root());
	
	tree2.buildTree(t1, t2); // .. e li usa per costruire un unico albero
	cout << "sott.albero 1: " << t1 
		<< "\nsott.albero 2: " << t2 
		<< "\nalbero ottenuto:" << tree2 
		<< " (Nota: la radice contiene un valore grezzo, perche' gli\n"
		<< " oggetti di tipo int non sono inizializzati "
		<< " automaticamente)" << "\n";

	BinTree<int> tree3;
	
	for (int i = 1; i < 7; ++i)
		tree3.insert(i);
	
	cout << "\nINSERIMENTO PER LIVELLI\nInseriti in sequenza: "
		<< "1, 2, 3, 4, 5, 6\n"; tree3.bfs(tree3.root());
	
	// mantiene aperto il terminale
	int x;
	cout << "\npremere un tasto per continuare...";
	std::cin >> x;
	
	return 0;
}