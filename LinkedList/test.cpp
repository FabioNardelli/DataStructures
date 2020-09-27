#include <iostream>
#include "LinkedList.h"
using std::cout;

int main()
{
    LinkedList<int> list;

    cout << "\n************** TEST CLASSE LinkedList **************\n\n";

    // stampa una lista vuota
    cout << "Crea e tenta di stampare una lista vuota:\n";
    list.print();

    // inserisce tre elementi nella lista usando la funzione insert
    // e le funzioni begin e end per indicare la posizione
    list.insert(list.begin(), 1);
    list.insert(list.end(), 2);

    // stampa la lista usando l'operatore <<
    cout << "\nFUNZIONE insert:\ninserisce un elemento"
         << " nella posizione precedente a quella indicata."
         << "\nSe la lista e' vuota inserisce in prima posizione."
         << "\nSe la lista contiene un solo elemento la prima posizione"
         << " coincide con l'ultima."
         << "\nInseriti elementi 1(in lista vuota, prima del primo"
         << " elemento) e 2(prima dell'ultimo elemento)\n" << list;

    // aggiunge altri elementi con le funzioni pushFront e pushBack
    list.pushFront(3);
    list.pushBack(4);
    cout << "\nFUNZIONI pushFront E pushBack:\naggiungono un "
         << "elemento rispettivamente in testa e in coda."
         << "\nAggiunti elementi 3(in testa) e 4(in coda)\n" << list;

    // operatore ==
    LinkedList<int> list2, list3;
    list2.pushBack(45);

    cout << "\nOPERATORE ==\nCrea una lista 'list2'\nlist2:" << list2;
    cout << "list == list2:\n";
    if (list == list2)
        cout << "true\n";
    else
        cout << "false\n";
    // operatore =
    list3 = list2;

    cout << "\nOPERATORE DI ASSEGNAMENTO =\nCrea una lista 'list3'\n";
    cout << "list3 = list2\nlist3:\n" << list3;
    cout << "list3 == list2:\n";
    if (list3 == list2)
        cout << "true\n";
    else
        cout << "false\n";

    // rimozione elementi
    list.remove(list.end());
    cout << "\nFUNZIONE remove\nRimosso ultimo elemento da 'list'\n"
            << list;
    list.popBack();
    list.popFront();
    cout << "\nFUNZIONI popFront E popBack\nRimossi primo e ultimo"
         << " elemento\n" << list;

    // mantiene aperto il terminale
    int x;
    cout << "\npremere un tasto per continuare...";
    std::cin >> x;


    return 0;
}
