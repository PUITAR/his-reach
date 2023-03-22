/*
 * @author: 
 * @Description: graph class
 * @Date: 2023-03-12 19:42:30
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include "vec.h"

#include <fstream>
#include <iostream>
using namespace std;

const int INF = 2147483000;

typedef struct edge {
    int id, timestamp;
    
    edge() {}
    edge(int vid, int t) {
        id = vid, timestamp = t; }

} edge ;

typedef vec<edge> edge_set_t;

class Graph {
public: // variables
    int minid, maxid, mintime, maxtime;

public: // functions
    Graph(const char dataPth[]) {
        cout << "graph loading...";
        assert( Nin = new vec<edge_set_t*> );
        assert( Nout = new vec<edge_set_t*> );

        minid = mintime = +INF;
        maxid = maxtime = -INF;
        ifstream fin(dataPth);
        int u, v, t;
        
        while (fin >> u >> v >> t) {
            // cout << u << ' ' << v << ' ' << t << endl;
            minid = u < minid ? u : minid;
            minid = v < minid ? v : minid;
            maxid = u > maxid ? u : maxid;
            maxid = v > maxid ? v : maxid;
            mintime = t < mintime ? t : mintime;
            maxtime = t > maxtime ? t : maxtime;

            push_Nout(u, v, t), push_Nin(v, u, t);
        }
        for (int i = Nin->size(); i < Nout->size(); i++) Nin->push_back(new edge_set_t) ;
        for (int i = Nout->size(); i < Nin->size(); i++) Nout->push_back(new edge_set_t) ;
        cout << "[fin]" << endl;

    }

    ~Graph() {
        edge_set_t * p;
        for (int i = 0; i < Nin->size(); i++) {
            p = Nin->at(i);
            if (p) delete p;
        }
        for (int i = 0; i < Nout->size(); i++) {
            p = Nout->at(i);
            if (p) delete p;
        }
        delete Nin;
        delete Nout;
    }

    edge_set_t * get_Nin(int vid) {
        assert(0 <= vid and vid < Nin->size());
        return Nin->at(vid);
    }

    edge_set_t * get_Nout(int vid) {
        assert(0 <= vid and vid < Nout->size());
        return Nout->at(vid);
    }

    int degree_in(int vid) {
        if(0 <= vid and vid < Nin->size() and Nin->at(vid))
            return Nin->at(vid)->size();
        return 0;
    }

    int degree_out(int vid) {
        if (0 <= vid and vid < Nout->size() and Nout->at(vid))
            return Nout->at(vid)->size();
        return 0;
    }

private: // functions
    void push_Nin(int vid, int id, int t) {
        // idx from 0 to sz-1
        assert( vid >= 0 );
        for (int i = Nin->size(); i <= vid; i++) 
            Nin->push_back( new edge_set_t );
        Nin->at(vid)->push_back( edge(id, t) );
    }

    void push_Nout(int vid, int id, int t) {
        assert( vid >= 0 );
        for (int i = Nout->size(); i <= vid; i++) 
            Nout->push_back( new edge_set_t );
        Nout->at(vid)->push_back( edge(id, t) );
    }

private: // variables
    vec<edge_set_t*>* Nin;
    vec<edge_set_t*>* Nout;

};

#endif