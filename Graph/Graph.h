// By Fabio Nardelli
// Realizzazione di grafo basata su un vettore di liste di adiacenza.
// A ciascun nodo inserito nel grafo viene assegnato un codice numerico(id)
// unico che permette di identificarlo, a prescindere dall'etichetta(piu' nodi
// potrebbero avere la stessa etichetta). Appena creato, un nodo ha id = -1.
// Quando viene aggiunto al grafo, riceve un id univoco(a partire da 0) e
// viene inserito nel vettore nella posizione uguale al suo id(es.: il nodo 
// con id 0 finira' nella locazione 0) in modo da rendere istantaneo l'accesso
// alla relativa locazione. In questo modo, l'accesso alla lista di adiacenza
// di ciascun nodo ha tempo costante, e non dipende dalla posizione del nodo
// nel vettore. Il vettore e' dinamico, viene raddoppiato(in realta' viene
// allocato un nuovo vettore) quando si cerca di inserire un elemento oltre 
// il massimo numero contenibile e dimezzato al raggiungimento del valore 
// soglia di fattore di carico 1/4. Quando viene rimosso un nodo dal grafo, 
// il relativo elemento nel vettore viene solo marcato come eliminato(impo-
// stando a NIL il puntatore al nodo) in modo da evitare una piu' costosa 
// ristrutturazione del vettore. Quando viene allocato un nuovo vettore,
// solo gli elementi non cancellati vi sono copiati, percio' i loro id vengono 
// opportunamente modificati per coincidere con le nuove locazioni.


#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <stdexcept>
#include "../LinkedList/LinkedList.h"

/*=============================  CLASSE NODE  ===============================*/

template <typename T>
class Node;

template <typename T>
std::ostream &operator<<(std::ostream &, const Node<T> &);

// rappresenta un generico nodo del grafo. Ogni nodo e' dotato di un
// codice numerico identificativo unico che lo distingue dagli altri
template <typename T>
class Node {
    friend std::ostream &operator<< <>(std::ostream &, const Node<T> &);
    template <typename L, typename W> friend class Graph;
public:
    // costruttore di default: assegna un id di default al nodo
    Node<T>() : id(-1) { }
    // costruttore con argomento, imposta l'etichetta
    Node<T>(T lab) : id(-1), label(lab) { }

    // operatori == e !=
    bool operator==(const Node<T> &n) const { return id == n.id; }
    bool operator!=(const Node<T> &n) const { return id != n.id; }

    // funzioni per impostare e leggere l'etichetta
    void setLabel(T lab) { label = lab; }
    const T &getLabel() const { return label; }

    // ottiene l'id del nodo
    int getId() const { return id; }
private:  
    int id;  // numero identificativo del nodo
    T label; // etichetta del nodo

    // indica se il nodo e' presente nel grafo
    bool belongs() const { return id > -1; }
};

// overload operatore <<
template <typename T>
std::ostream &operator<<(std::ostream &output, const Node<T> &n)
{
    // stampa l'etichetta del nodo
    output << n.label;
    return output;
}

/*=============================  CLASSE GRAPH  ==============================*/

template <typename L, typename W>
class Graph {
public:
    typedef LinkedList<Node<L>*> NodeList;

    // costruttore di default
    Graph(int c = defaultCapacity) :
        nodeCount(0), edgeCount(0), size(0) { create(c); }
    // costruttore per copia
    Graph(const Graph<L, W> &);
    // overload operatore =
    Graph<L, W> &operator=(const Graph<L, W> &);
    // distruttore
    ~Graph() { delete [] nodes; }

    // operatori

    // restituisce true se il grafo e' vuoto, false altrimenti
    bool isEmpty() const { return size == 0; }
    // aggiunge un nodo al grafo
    void addNode(Node<L> &);
    // crea un arco fra due nodi
    void addEdge(Node<L> &, Node<L> &, W);
    // restituisce true se il nodo indicato e' nel grafo, false altrimenti
    bool hasNode(const Node<L> &n) const { return n.belongs(); }
    // restituisce true se l'arco indicato esiste, false altrimenti
    bool hasEdge(const Node<L> &, const Node<L> &) const;
    // elimina un nodo
    void removeNode(Node<L> &);
    // elimina un arco
    void removeEdge(Node<L> &, Node<L> &);
    // restituisce una lista contenente l'insieme di adiacenza di un nodo,
    // ovvero l'insieme di tutti e soli i nodi adiacenti al nodo indicato
    NodeList neighbors(const Node<L> &) const;
    // restituisce una lista contenente tutti i nodi del grafo
    NodeList getNodes() const;
    // legge l'etichetta del nodo indicato
    L readNode(const Node<L> &) const;
    // scrive nel nodo indicato
    void writeNode(Node<L> &, L);
    // legge il peso dell'arco indicato
    W readEdge(const Node<L> &, const Node<L> &) const;
    // scrive nell'arco indicato
    void writeEdge(Node<L> &, Node<L> &, W);
    // restituisce il numero di nodi presenti
    int numNodes() const { return nodeCount; }
    // restituisce il numero di archi presenti
    int numEdges() const { return edgeCount; }
private:

    // rappresenta un elemento di una lista di adiacenza
    struct Adjacent {
        Node<L> *adjNode; // nodo adiacente
        W weight; // peso dell'arco entrante nel nodo adiacente
    };

    // rappresenta un elemento del vettore
    struct NodeElement {
        Node<L> *node; // puntatore a un nodo del grafo
        LinkedList<Adjacent> adjList; // lista di adiacenza del nodo
    };

    NodeElement *nodes; // puntatore usato per rappresentare un vettore dinamico
    int nodeCount; // num. di nodi (tutti tranne quelli cancellati)
    int edgeCount; // num. di archi

    int size; // numero di nodi presenti nel vettore (anche i cancellati)
    int capacity; // numero massimo di nodi memorizzabili nel vettore
    static const int defaultCapacity = 16;
    static const float minLoadFactor;
    void create(int); // funzione di inizializzazione
    void resize(int); // cambia la dimensione del vettore
};

template <typename L, typename W>
const float Graph<L, W>::minLoadFactor = .25;


/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// crea un vettore vuoto
template <typename L, typename W>
void Graph<L, W>::create(int c)
{
    if (c > 0)
        capacity = c;

    nodes = new NodeElement[capacity];
}

// costruttore per copia
template <typename L, typename W>
Graph<L, W>::Graph(const Graph<L, W> &g)
{
    capacity = g.capacity;
    size = g.size;
    nodeCount = g.nodeCount;
    edgeCount = g.edgeCount;

    nodes = new NodeElement[capacity];

    for (int i = 0; i < capacity; ++i)
        nodes[i] = g.nodes[i];
}

// overload operatore =
template <typename L, typename W>
Graph<L, W> &Graph<L, W>::operator=(const Graph<L, W> &g)
{
    if (&g != this) { // evita l'autoassegnazione
        if (capacity != g.capacity) { // se i due array hanno dimensione
            delete [] nodes;          // diversa, dealloca quello di sinistra
            capacity = g.capacity;    // e alloca un nuovo array di capacita'
                                      // uguale a quello da copiare
            nodes = new NodeElement[capacity];
        }

        // copia gli elementi
        for (int i = 0; i < g.capacity; ++i)
            nodes[i] = g.nodes[i];

        size = g.size;
        nodeCount = g.nodeCount;
        edgeCount = g.edgeCount;
    }

    return *this; // consente a = b = c
}

// utility function per modificare le dimensioni del vettore. Alloca un
// nuovo vettore di dimensioni newCapacity e copia il contenuto del vecchio
// vettore nel nuovo, dopodiche' cancella il vecchio vettore.
template <typename L, typename W>
void Graph<L, W>::resize(int newCapacity)
{
    if (newCapacity <= 0)
        throw std::invalid_argument("newCapacity must be > 0!");

    // alloca un nuovo vettore di dimensione doppia
    NodeElement *temp = new NodeElement[newCapacity];

    // copia nel nuovo vettore gli elementi validi del vettore originale
    int j = 0; // indice per scartare gli elementi cancellati
    for (int i = 0; i < size && j < size; ++i) {

        // bypassa gli elementi cancellati
        while (nodes[j].node == 0 && j < size) { ++j; }

        if (j < size) {
            temp[i] = nodes[j];
            // aggiorna l'id del nodo, in modo che coincida
            // con la nuova posizione nel vettore
            temp[i].node->id = i;
            ++j;
        }
    }

    capacity = newCapacity;
    delete [] nodes;
    nodes = temp;
    size = nodeCount;
}

// aggiunge un nodo al grafo
template <typename L, typename W>
void Graph<L, W>::addNode(Node<L> &n)
{
    // se non c'e' spazio nel vettore, raddoppia le dimensioni
    if (size == capacity)
        resize(capacity * 2);

    // assegna al nodo un id unico, e memorizza il nodo nella
    // posizione del vettore uguale al valore dell'id
    n.id = size;
    nodes[size].node = &n;
    ++size;
    ++nodeCount;
}


// crea un arco fra due nodi, da n1 a n2 (n1,n2)
// I due nodi devono gia' appartenere al grafo
template <typename L, typename W>
void Graph<L, W>::addEdge(Node<L> &n1, Node<L> &n2, W edgeWeight)
{
    // se almeno uno dei due nodi non e' presente, termina
    if (!n1.belongs() || !n2.belongs()) {
        std::cerr << "Cannot insert edge: one or both nodes not found!\n";
        return;
    }

    Adjacent a;
    a.adjNode = &n2;
    a.weight = edgeWeight;
    nodes[n1.id].adjList.pushBack(a);
    ++edgeCount;
}

// indica se esiste un arco tra i due nodi indicati (n1,n2),
// cioe' controlla se nella lista di adiacenza di n1 c'e' n2
template <typename L, typename W>
bool Graph<L, W>::hasEdge(const Node<L> &n1, const Node<L> &n2) const
{
    // se almeno uno dei due nodi non e' presente, ovviamente
    // nemmeno l'arco e' presente
    if (!n1.belongs() || !n2.belongs()) {
        std::cerr << "One or both nodes not found!\n";
        return false;
    }

    // scandisce la lista di adiacenza alla ricerca del nodo n2
    // se lo trova, restituisce true...
    typename LinkedList<Adjacent>::Position pos =
        nodes[n1.id].adjList.begin();

    while (!nodes[n1.id].adjList.finished(pos)) {
        if (nodes[n1.id].adjList.read(pos).adjNode->id == n2.id)
            return true;
        pos = nodes[n1.id].adjList.next(pos);
    }

    return false; // ...altrimenti false
}

// rimuove un nodo dal grafo
template <typename L, typename W>
void Graph<L, W>::removeNode(Node<L> &n)
{
    // se il nodo non e' presente, termina
    if (!n.belongs()) {
        std::cerr << "Cannot delete node: node not found!\n";
        return;
    }

    // elimina eventuali archi uscenti
    while (!nodes[n.id].adjList.isEmpty()) {
        nodes[n.id].adjList.remove(nodes[n.id].adjList.begin());
        --edgeCount;
    }

    // elimina eventuali archi entranti
    for (int i = 0; i < size; ++i) {
        typename LinkedList<Adjacent>::Position pos =
            nodes[i].adjList.begin();
        bool removed = false;

        while (!nodes[i].adjList.finished(pos) && !removed) {
            if (nodes[i].adjList.read(pos).adjNode->id == n.id) {
                nodes[i].adjList.remove(pos);
                --edgeCount;
                removed = true;
            }
            pos = nodes[i].adjList.next(pos);
        }
    }

    // rimozione nodo
    // imposta a NIL il puntatore al nodo da cancellare
    // del relativo NodeElement
    nodes[n.id].node = 0;
    n.id = -1; // n e' fuori dal grafo
    --nodeCount;

    // se il fattore di carico raggiunge la soglia minLoadFactor,
    // dimezza le dimensioni del vettore
    if (static_cast<float>(nodeCount) / capacity <= minLoadFactor)
        resize(capacity / 2);
}

// rimuove un arco (n1,n2) dal grafo. 
template <typename L, typename W>
void Graph<L, W>::removeEdge(Node<L> &n1, Node<L> &n2)
{
    // se almeno uno dei due nodi non e' presente, termina
    if (!n1.belongs() || !n2.belongs()) {
        std::cerr << "One or both nodes not found!\n";
        return;
    }

    typename LinkedList<Adjacent>::Position pos =
        nodes[n1.id].adjList.begin();

    while (!nodes[n1.id].adjList.finished(pos)) {
        if (nodes[n1.id].adjList.read(pos).adjNode->id == n2.id) {
            nodes[n1.id].adjList.remove(pos);
            --edgeCount;
            return;
        }
        pos = nodes[n1.id].adjList.next(pos);
    }

    // se l'esecuzione della funzione arriva qui, significa
    // che l'arco non e' presente.
    std::cerr << "Edge not found!\n";
}

// restituisce la lista di adiacenza di un nodo
template <typename L, typename W>
typename Graph<L, W>::NodeList Graph<L, W>::neighbors(const Node<L> &n) const
{ 
    NodeList list;

    // se il nodo non e' presente, restituisce una lista vuota
    if (!n.belongs())
        return list;

    // altrimenti, restituisce list dopo avervi inserito
    // i nodi presenti nella lista di adiacenza di n
    typename LinkedList<Adjacent>::Position pos =
        nodes[n.id].adjList.begin();

    while (!nodes[n.id].adjList.finished(pos)) {
        list.pushBack(nodes[n.id].adjList.read(pos).adjNode);
        pos = nodes[n.id].adjList.next(pos);
    }

    return list;
}

// restituisce una lista contenente tutti i nodi del grafo
template <typename L, typename W>
typename Graph<L, W>::NodeList Graph<L, W>::getNodes() const
{
    NodeList list;

    // se il grafo e' vuoto, restituisce una lista vuota
    if (isEmpty())
        return list;

    // altrimenti, restituisce la lista dei nodi
    for (int i = 0; i < size; ++i) {
        if (nodes[i].node != 0)
            list.pushBack(nodes[i].node);
    }

    return list;
}

// legge l'etichetta del nodo indicato
template <typename L, typename W>
L Graph<L, W>::readNode(const Node<L> &n) const
{
    // se il nodo non e' presente
    if (!n.belongs())
        throw std::domain_error("Node not found!");

    return n.label;
}

// scrive nel nodo indicato
template <typename L, typename W>
void Graph<L, W>::writeNode(Node<L> &n, L lab)
{
    // se il nodo non e' presente, termina
    if (!n.belongs()) {
        std::cerr << "Node not found!\n";
        return;
    }

    n.label = lab;
}

// legge il peso dell'arco indicato
template <typename L, typename W>
W Graph<L, W>::readEdge(const Node<L> &n1, const Node<L> &n2) const
{
    // se almeno uno dei due nodi non e' presente, restituisce NIL
    if (!n1.belongs() || !n2.belongs())
        throw std::domain_error("One or both nodes not found!\n");

    // cerca nella lista di adiacenza di n1 il nodo n2
    typename LinkedList<Adjacent>::Position pos =
        nodes[n1.id].adjList.begin();

    while (!nodes[n1.id].adjList.finished(pos)) {

        // se lo trova, restituisce il peso
        if (nodes[n1.id].adjList.read(pos).adjNode->id == n2.id)
            return nodes[n1.id].adjList.read(pos).weight;

        pos = nodes[n1.id].adjList.next(pos);
    }

    // se l'arco non esiste
    throw std::runtime_error("Edge not found!");
}

// scrive nell'arco indicato
template <typename L, typename W>
void Graph<L, W>::writeEdge(Node<L> &n1, Node<L> &n2, W w) 
{
    // se almeno uno dei due nodi non e' presente, termina
    if (!n1.belongs() || !n2.belongs()) {
        std::cerr << "One or both nodes not found!\n";
        return;
    }

    // cerca nella lista di adiacenza di n1 il nodo n2
    typename LinkedList<Adjacent>::Position pos =
        nodes[n1.id].adjList.begin();

    while (!nodes[n1.id].adjList.finished(pos)) {

        // se lo trova, scrive un peso nell'arco e termina
        if (nodes[n1.id].adjList.read(pos).adjNode->id == n2.id) {
            Adjacent t;
            t.adjNode = nodes[n1.id].adjList.read(pos).adjNode;
            t.weight = w;
            nodes[n1.id].adjList.write(pos, t);
            return;
        }

        pos = nodes[n1.id].adjList.next(pos);
    }
}

#endif