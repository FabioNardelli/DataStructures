#include <iostream>
#include "Tree.h"
using std::cout;

int main()
{
    Tree<int> tree;

    cout << "\n************** TEST CLASSE Tree **************\n\n";

    // stampa un albero vuoto
    cout << "Crea un albero 'tree'\ntree:";
    tree.print();

    // crea un nodo e lo inserisce nell'albero come radice
    Node<int> *root = new Node<int>;
    root->setValue(1);
    tree.insertRoot(root);

    // stampa l'albero
    cout << "\nInserita radice con etichetta 1: ";
    tree.print();
    cout << "\n";

    // aggiunge due nodi all'albero                       //       1       //
    Node<int> *n2 = new Node<int>(2),                     //     /   \     //
            *n3 = new Node<int>(4);                           //   2       4   //
    tree.insertChild(tree.root(), n2);                    //               //
    tree.insertSibling(tree.firstChild(tree.root()), n3);

    // stampa l'albero usando l'operatore << in overload
    cout << "Aggiunti tre nodi come figli"
         << " \ndella radice, con etichette"
         << " 2 e 4:" << tree;

    // aggiunge un sottoalbero di tre nodi
    // (si fa uso dell'inizializizazione tramite costruttore)
    Node<int> *root1 = new Node<int>(3), *n = new Node<int>(5),
            *n1 = new Node<int>(6);
    Tree<int> t1;
    t1.insertRoot(root1);                                 //       1       //
    t1.insertChild(t1.root(), n);                         //     / | \     //
    t1.insertSibling(t1.firstChild(t1.root()), n1);       //   2   3   4   //
    tree.insertSibling(tree.firstChild                    //     /   \     //
        (tree.root()), t1.root());                        //    5     6    //

    Node<int> *current = tree.nextSibling
            (tree.firstChild(tree.root()));

    // mostra l'altezza dei nodi con etichette
    // 1, 3 e 5
    cout << "\nPROPRIETA' DELL'ALBERO\n"
         << "Altezza nodo con etichetta 1: "
         << tree.height(tree.root())
         << "\nAltezza nodo con etichetta 3: "
         << tree.height(current)
         << "\nAltezza nodo con etichetta 5: "
         << tree.height(tree.firstChild(current)) << "\n";

    // stampa il numero di nodi dei sottoalberi aventi radice
    // nei nodi con etichetta 2, 3 e 1(intero albero)	
    cout << "\nN. nodi nel sottoalbero con etichetta 2: "
         << tree.count(tree.firstChild(tree.root()))
         << "\nN. nodi nel sottoalbero con etichetta 3: "
         << tree.count(current)
         << "\nN. nodi nell'intero albero "
         << "\n(sottoalbero con etichetta 1):           "
         << tree.count(tree.root()) << "\n";

    // visite in profondita' (Depth First Search)
    cout << "\nVISITE IN PROFONDITA' (DFS)\n"
         << "\nVisita in pre-ordine:\n";
    tree.preOrder(tree.root());
    cout << "\nVisita in ordine(i = 1):\n";
    tree.inOrder(tree.root(), 1);
    cout << "\nVisita in post-ordine:\n";
    tree.postOrder(tree.root());
    cout << "\n";

    // visita in ampiezza (BFS)
    cout << "\nVISITA IN AMPIEZZA (BFS)\n";
    tree.bfs(tree.root());

    // cancella il sottoalbero avente radice              //      1        //
    // nel nodo con etichettta 3	                      //    /   \      //
    tree.deleteSubTree(current);                          //   2     4     //

    // stampa l'albero
    cout << "\nELIMINAZIONE NODI\nEliminazione sottoalbero"
         << " con etichetta 3:" << tree;

    // testa l'operatore =
    Tree<int> tree1;
    tree1 = tree;
    cout << "\nOPERATORE DI ASSEGNAZIONE\n"
         << "crea un albero 'tree1'\ntree1 = tree\ntree1:" << tree1;

    // cancella l'albero tree1 (copia di tree), dimostrando
    // il funzionamento dell'operatore =
    tree1.deleteSubTree(tree1.root());
    cout << "\nELIMINAZIONE ALBERO\nEliminazione tree1\ntree1: "
         << tree1 << "\ntree:" << tree;

    // testa le funzioni addAsChild e addAsSibling        //        1      //
    tree.addAsChild(tree.firstChild(tree.root()), 7);     //      /   \    //
    tree.addAsSibling(tree.firstChild                     //     2     4   //
            (tree.firstChild(tree.root())), 8);				  //   /   \       //
    cout << "\nAggiunti due nodi con etichette"           //  7     8      //
         << " 7 e 8:" << tree;

    // mantiene aperto il terminale
    int x;
    cout << "\npremere un tasto per continuare...";
    std::cin >> x;

    return 0;
}
