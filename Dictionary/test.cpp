#include <iostream>
#include <string>
#include "Dictionary.h"
using std::cout;
using std::string;

void checkKey(const string &key, const Dictionary<string, int> &dic);
void printData(const string &key, const Dictionary<string, int> &dic);

int main()
{
    Dictionary<string, int> myDictionary;

    cout << "\n************** TEST CLASSE Dictionary **************\n\n";

    // crea e stampa la dimensione del dizionario (vuoto)
    cout << "Dizionario appena creato.\nDim. dizionario: "
         << myDictionary.getSize();

    // inserisce alcuni elementi nel dizionario
    cout << "\n\nINSERIMENTO ELEMENTI\n";
    myDictionary.insert("Pippo", 45);
    cout << "Elemento inserito: " << "Pippo" << ", "
         << 45 << "\n";

    myDictionary.insert("Pluto", 38);
    cout << "Elemento inserito: " << "Pluto" << ", "
         << 38 << "\n";

    myDictionary.insert("Anacleto", 92);
    cout << "Elemento inserito: " << "Anacleto" << ", "
         << 92 << "\n" << "Dim. dizionario: "
         << myDictionary.getSize();

    // controlla l'appartenenza delle chiavi
    cout << "\n\nVERIFICA APPARTENENZA";
    checkKey("Pippo", myDictionary);
    checkKey("Pluto", myDictionary);
    checkKey("Anacleto", myDictionary);
    checkKey("Ascilto", myDictionary);
    checkKey("Eumolpo", myDictionary);

    // recuoera i dati associati alle chiavi
    cout << "\n\nCERCA ELEMENTI\n";
    cout << "chiave \"Pippo\": ";
    printData("Pippo", myDictionary);
    cout << "chiave \"Pluto\": ";
    printData("Pluto", myDictionary);
    cout << "chiave \"Anacleto\": ";
    printData("Anacleto", myDictionary);
    cout << "chiave \"Ascilto\": ";
    printData("Ascilto", myDictionary);
    cout << "chiave \"Encolpio\": ";
    printData("Encolpio", myDictionary);

    // modifica il valore associato a una chiave
    cout << "\n\nMODIFICA ELEMENTI\n";
    myDictionary.insert("Pippo", 71);
    cout << "Elemento con chiave \"Pippo\" modificato\n";
    cout << "chiave \"Pippo\": ";
    printData("Pippo", myDictionary);

    // rimozione elementi
    cout << "\nRIMOZIONE ELEMENTI\n";
    myDictionary.remove("Anacleto");
    cout << "Rimosso elemento con chiave \"Anacleto\":";
    checkKey("Anacleto", myDictionary);
    cout << "\nTentativo di rimuovere chiave \"Paperino\"(non presente):\n";
    myDictionary.remove("Paperino");

    // stampa dizionario
    cout << "\nSTAMPA DIZIONARIO\n";
    myDictionary.print();

    // stampa i valori del dizionario
    cout << "\nSTAMPA VALORI\n";
    myDictionary.values();

    // stampa le chiavi del dizionario
    cout << "\nSTAMPA CHIAVI\n";
    myDictionary.keys();

    // test funzione containsValue()
    cout << "\nVERIFICA APPARTENENZA VALORE\n";
    myDictionary.containsValue(38) ? cout << "Valore 38 presente\n" :
        cout << "Valore 38 NON presente!\n";
    myDictionary.containsValue(5) ? cout << "Valore 5 presente\n" :
        cout << "Valore 5 NON presente!\n";

    // inserisce elemento e stampa usando l'operatore <<
    myDictionary.insert("Nonna Papera", 23);
    cout << "\nOPERATORE <<\n" << "Elemento inserito: " << "Nonna Papera" << ", "
         << 23 << "\nDizionario:\n\n" << myDictionary;

    // operatore =
    Dictionary<string, int> dictionary2;
    cout << "\nOPERATORE =\nCrea un dizionario vuoto \"dictionary2\""
         << "\ndictionary2:\n\n" << dictionary2;

    dictionary2 = myDictionary;

    cout << "\ndopo l'assegnazione dictionary2 = myDictionary:\n\n"
         << dictionary2;

    // mantiene aperto il terminale
    int x;
    cout << "\npremere un tasto per continuare...";
    std::cin >> x;

    return 0;
}

// funzione di servizio che stampa un messaggio indicante se una chiave e'
// presente nel dizionario
void checkKey(const string &key, const Dictionary<string, int> &dic)
{
    dic.belongs(key) ? cout << "\nChiave " << "\"" << key << "\"" << " presente"
        : cout << "\nChiave " << "\"" << key << "\"" << " non presente!";
}

// stampa la coppia chiave-valore corrispondente alla chiave indicata
void printData(const string &key, const Dictionary<string, int> &dic)
{
    dic.find(key) != 0 ? cout << key << ", " << *dic.find(key) << "\n"
        : cout << "Key not found!\n";
}