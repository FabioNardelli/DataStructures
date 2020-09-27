#include <iostream>
#include "LinkedStack.h"
using std::cout;

int main()
{
    LinkedStack<int> stack;

    cout << "\n************** TEST CLASSE LinkedStack **************\n\n";

    // stampa una pila vuota
    cout << "Crea e tenta di stampare una pila vuota:\n";
    stack.print();

    // inserisce tre elementi nella pila
    stack.push(1);
    stack.push(2);
    stack.push(3);

    // stampa la pila usando l'operatore <<
    cout << "\nFUNZIONE push:\ninserisce un elemento nella pila."
         << "\nInseriti(nell'ordine) elementi 1, 2 e 3\n" << stack;

    // operatore top
    cout << "\nFUNZIONE top";
    cout << "\nL'elemento in cima alla pila e': " << stack.top();

    // rimozione elementi
    stack.pop();
    cout << "\n\nFUNZIONE pop\nRimosso elemento in cima"
         << stack;
    cout << "L'elemento in cima alla pila e': " << stack.top();
    stack.pop();
    cout << "\n\nRimosso elemento in cima" << stack;
    cout << "L'elemento in cima alla pila e': " << stack.top();
    stack.pop();
    cout << "\n\nRimosso elemento in cima" << stack;

    // operatore ==
    LinkedStack<int> stack2, stack3;
    stack2.push(45);

    cout << "\nOPERATORE ==\nCrea una pila 'stack2'\nstack2:" << stack2;
    cout << "stack == stack2:\n";
    if (stack == stack2)
        cout << "true\n";
    else
        cout << "false\n";

    // operatore =
    stack3 = stack2;

    cout << "\nOPERATORE DI ASSEGNAMENTO =\nCrea una pila 'stack3'\n";
    cout << "stack3 = stack2\nstack3:" << stack3;
    cout << "stack3 == stack2:\n";
    if (stack3 == stack2)
        cout << "true\n";
    else
        cout << "false\n";

    // mantiene aperto il terminale
    int x;
    cout << "\npremere un tasto per continuare...";
    std::cin >> x;

    return 0;
}