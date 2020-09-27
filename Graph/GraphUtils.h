// By Fabio Nardelli
// funzioni di utilita' sui grafi: visite, cammini minimi

#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <limits>
#include "Graph.h"
#include "../LinkedQueue/LinkedQueue.h"
#include "../PriorityQueue/PriorityQueue.h"

/*================================  VISITE  =================================*/

template <typename L, typename W>
void dfs(const Graph<L, W> &, const Node<L> &);

template <typename L, typename W>
void bfs(const Graph<L, W> &, const Node<L> &);

/*============================  CAMMINI MINIMI  =============================*/

template <typename L>
void printPath(const Node<L> &, const Node<L> &, Node<L> *[]);

template <typename L, typename W>
void johnson(const Graph<L, W> &, Node<L> &, Node<L> *[]);

template <typename L, typename W>
void bellman(const Graph<L, W> &, Node<L> &, Node<L> *[]);


/*===========================================================================*/
/*                           DEFINIZIONI FUNZIONI                            */
/*===========================================================================*/

// utility function che implementa un algoritmo di visita dfs ricorsivo
template <typename L, typename W>
void dfsHelper(const Graph<L, W> &g, const Node<L> &u, bool visited[])
{
    visited[u.getId()] = true;
    std::cout << u << ", "; // esamina il nodo in previsita

    LinkedList<Node<L>*> adj = g.neighbors(u);
    typename LinkedList<Node<L>*>::Position pos = adj.begin();
    while (!adj.finished(pos)) {
        // visita l'arco (u,v) per un nodo v adiacente a u
        std::cout << "(" << u << "," << *adj.read(pos) << "), ";
        if (!visited[adj.read(pos)->getId()])
            dfsHelper(g, *adj.read(pos), visited);
        pos = adj.next(pos);
    }
}

// visita in profondita'(dfs)
template <typename L, typename W>
void dfs(const Graph<L, W> &g, const Node<L> &n)
{
    bool *v = new bool[g.numNodes()];
    for (int i = 0; i < g.numNodes(); ++i)
        v[i] = false;

    dfsHelper(g, n, v);
    delete [] v;
}

// visita in ampiezza(bfs)
template <typename L, typename W>
void bfs(const Graph<L, W> &g, const Node<L> &n)
{
    bool *visited = new bool[g.numNodes()];
    for (int i = 0; i < g.numNodes(); ++i)
        visited[i] = false;

    LinkedQueue<const Node<L>*> q;
    q.enqueue(&n);
    visited[n.getId()] = true;
    while (!q.isEmpty()) {
        Node<L> u = *q.top();
        q.dequeue();
        std::cout << u << ", ";
        LinkedList<Node<L>*> adj = g.neighbors(u);
        typename LinkedList<Node<L>*>::Position pos = adj.begin();
        while (!adj.finished(pos)) {
            // visita l'arco (u,v) per un nodo v adiacente a u
            std::cout << "(" << u << "," << *adj.read(pos) << "), ";
            if (!visited[adj.read(pos)->getId()]) {
                visited[adj.read(pos)->getId()] = true;
                q.enqueue(adj.read(pos));
            }
            pos = adj.next(pos);
        }
    }

    delete [] visited;
}

// funzione che stampa il cammino minimo tra due nodi del grafo, utilizzando
// l'albero dei cammini minimi generato da una delle funzioni qui sotto
template <typename L>
void printPath(const Node<L> &s, const Node<L> &d, Node<L> *t[])
{
    if (s == d) {
        std::cout << s << " ";
    } else if (t[d.getId()] == 0) {
        std::cout << "Error ";
    } else {
        printPath(s, *t[d.getId()], t);
        std::cout << d << " ";
    }
}

// algoritmo di Johnson per i cammini minimi a sorgente singola,
// ideale per grafi sparsi con archi con pesi non negativi
// [complessita': O(m*log n)] 
template <typename L, typename W>
void johnson(const Graph<L, W> &g, Node<L> &r, Node<L> *t[])
{
    // vettore delle distanze: d[i] rappresenta il costo del cammino
    // da r al nodo i
    int *d = new int[g.numNodes()];
    // vettore booleano: b[u.getId()] == true se il nodo u e' in s
    bool *b = new bool[g.numNodes()];

    // inizializzazione vettori
    for (int i = 0; i < g.numNodes(); ++i) {
        t[i] = 0;
        d[i] = std::numeric_limits<int>::max();
        b[i] = false;
    }

    t[r.getId()] = 0;
    d[r.getId()] = 0;
    b[r.getId()] = true;
    PriorityQueue<Node<L>*> s(g.numNodes());
    s.insert(&r, 0);
    while (!s.isEmpty()) {
        Node<L> &u = *s.min();
        s.deleteMin();
        b[u.getId()] = false;
        LinkedList<Node<L>*> adj = g.neighbors(u);
        typename LinkedList<Node<L>*>::Position pos = adj.begin();
        while (!adj.finished(pos)) {
            // verifica condizioni di Bellman
            if (d[u.getId()] + g.readEdge(u, *adj.read(pos))
                < d[adj.read(pos)->getId()]) {
                if (!b[adj.read(pos)->getId()]) {
                    s.insert(adj.read(pos), d[u.getId()]
                        + g.readEdge(u, *adj.read(pos)));
                    b[adj.read(pos)->getId()] = true;
                } else {
                    s.decrease(adj.read(pos), d[u.getId()] +
                        g.readEdge(u, *adj.read(pos)));
                }
                t[adj.read(pos)->getId()] = &u;
                d[adj.read(pos)->getId()] = d[u.getId()] +
                    g.readEdge(u, *adj.read(pos));
            }
            pos = adj.next(pos);
        }
    }

    delete [] d;
    delete [] b;
}

// algoritmo di Bellman-Ford-Moore per i cammini minimi a sorgente singola,
// ideale per grafi con archi con pesi negativi
// [complessita': O(m*n)] 
template <typename L, typename W>
void bellman(const Graph<L, W> &g, Node<L> &r, Node<L> *t[])
{
    // vettore delle distanze: d[i] rappresenta il costo del cammino
    // da r al nodo i
    int *d = new int[g.numNodes()];
    // vettore booleano: b[u.getId()] == true se il nodo u e' in s
    bool *b = new bool[g.numNodes()];

    // inizializzazione vettori
    for (int i = 0; i < g.numNodes(); ++i) {
        t[i] = 0;
        d[i] = std::numeric_limits<int>::max();
        b[i] = false;
    }

    t[r.getId()] = 0;
    d[r.getId()] = 0;
    b[r.getId()] = true;
    LinkedQueue< Node<L>* > s;//(g.numNodes());
    s.enqueue(&r);
    while (!s.isEmpty()) {
        Node<L> &u = *s.top();
        s.dequeue();
        b[u.getId()] = false;
        LinkedList<Node<L>*> adj = g.neighbors(u);
        typename LinkedList<Node<L>*>::Position pos = adj.begin();
        while (!adj.finished(pos)) {
            // verifica condizioni di Bellman
            if (d[u.getId()] + g.readEdge(u, *adj.read(pos))
                < d[adj.read(pos)->getId()]) {
                if (!b[adj.read(pos)->getId()]) {
                    s.enqueue(adj.read(pos));
                    b[adj.read(pos)->getId()] = true;
                }
                t[adj.read(pos)->getId()] = &u;
                d[adj.read(pos)->getId()] = d[u.getId()] +
                    g.readEdge(u, *adj.read(pos));
            }
            pos = adj.next(pos);
        }
    }

    delete [] d;
    delete [] b;
}

/* versione che produce un vettore di id dei nodi
// algoritmo di Johnson per i cammini minimi a sorgente singola,
// ideale per grafi sparsi con archi con pesi non negativi
template <typename L, typename W>
void johnson(const Graph<L, W> &g, const Node<L> &r, int t[])
{
	// vettore delle distanze: d[i] rappresenta il costo del cammino
	// da r al nodo i
	int *d = new int[g.numNodes()];
	// vettore booleano: b[u.getId()] == true se il nodo u e' in s
	bool *b = new bool[g.numNodes()];

	// inizializzazione vettori
	for (int i = 0; i < g.numNodes(); ++i) {
		t[i] = 0;
		d[i] = std::numeric_limits<int>::max();
		b[i] = false;
	}

	t[r.getId()] = 0;
	d[r.getId()] = 0;
	b[r.getId()] = true;
	PriorityQueue< Node<L> > s(g.numNodes()); 
	s.insert(r, 0);
	while (!s.isEmpty()) {
		Node<L> u = s.min(); 
		s.deleteMin();        
		b[u.getId()] = false;
		LinkedList<Node<L>*> adj = g.neighbors(u);
		typename LinkedList<Node<L>*>::Position pos = adj.begin();
		while (!adj.finished(pos)) {
			// verifica condizioni di Bellman
			if (d[u.getId()] + g.readEdge(u, *adj.read(pos))
				< d[adj.read(pos)->getId()]) {
				if (!b[adj.read(pos)->getId()]) {
					s.insert(*adj.read(pos), d[u.getId()]
						+ g.readEdge(u, *adj.read(pos)));
					b[adj.read(pos)->getId()] = true;
				} else {
					s.decrease(*adj.read(pos), d[u.getId()] +
						g.readEdge(u, *adj.read(pos)));
				} 
				t[adj.read(pos)->getId()] = u.getId();
				d[adj.read(pos)->getId()] = d[u.getId()] + 
					g.readEdge(u, *adj.read(pos));
			}
			pos = adj.next(pos);
		}
	}

	delete [] d;
	delete [] b;
}*/

#endif
