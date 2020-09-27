#include <iostream>
#include <string>
#include "PriorityQueue.h"
using std::cout;
using std::string;

int main()
{
    PriorityQueue<int> q;

    cout << "\n************** TEST CLASSE PriorityQueue **************\n\n";

    // crea e tenta di stampare una coda vuota
    cout << "Creata una coda vuota di interi.\nCiascun elemento coincide con il"
         << " valore di priorita'\n";
    q.print();

    // inserisce alcuni elementi nella coda, indicando le rispettive priorita',
    // e stampa la coda usando l'operatore <<
    q.insert(5, 5);
    q.insert(10, 10);
    q.insert(8, 8);
    q.insert(11, 11);
    q.insert(13, 13);
    q.insert(12, 12);
    q.insert(9, 9);
    q.insert(18, 18);
    q.insert(3, 3);
    q.insert(6, 6);
    cout << "\nINSERIMENTO ELEMENTI";
    cout << "\nInseriti elementi:\n" << q;

    // restituisce il minimo
    cout << "\nL'elemento con la priorita' piu' alta(valore minimo) e':\n"
         << q.min();

    // cancella il minimo (l'elemento con la priorita' piu' alta)
    q.deleteMin();
    cout << "\n\nELIMINAZIONE ELEMENTI";
    cout << "\nCancellato l'elemento minimo\n" << q;	

    // test operatore =
    cout << "\nOPERATORE =";
    PriorityQueue<int> copy;
    cout << "\nCrea una coda vuota 'copy':\n" << copy;
    cout << "\nDopo l'assegnazione:\ncopy:\n";
    copy = q;
    cout << copy;

    // diminuisce la priorita' di un elemento
    PriorityQueue<string> q2;

    q2.insert("Pippo", 2);
    q2.insert("Pluto", 3);
    q2.insert("Paperino", 4);

    cout << "\nMODIFICA PRIORITA'";
    cout << "\nCreata una coda di stringhe con i seguenti elementi"
         << "\n(priorita' rispettive 2, 3 e 4):\n" << q2;
    q2.decrease("Paperino", 1);
    cout << "\nPriorita' elemento \"Paperino\" diminuita(=1):\n" << q2;	

    // mantiene aperto il terminale
    int x;
    cout << "\npremere un tasto per continuare...";
    std::cin >> x;

    return 0;
}
