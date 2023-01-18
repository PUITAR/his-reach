/*
 * @author: yangming
 * @Description: vertex order
 * @Date: 2023-01-14 10:36:40
 */
#include "../graph.h"
#include <stdlib.h>
#include <algorithm>
#include <fstream>
using namespace std;

Graph *g = nullptr;

int degr(int u, char io) {
    N_t *n = g->N(u, io);
    int d = n->len/sizeof(tuple_t);
    freeN(n);
    return d;
}

typedef struct Node {
    int vid, value;
    // We say the rank of a vertex u is higher than that of a vertex v if O(u) < O(v)
    bool operator<(const Node &o) const {
        if (value == o.value) return vid < o.vid;
        return value > o.value; 
    }

} Node ;

int main(int argc, char *argv[]) {
    
    int minid = atoi(argv[1]);
    int maxid = atoi(argv[2]);
    g = new Graph("../db");
    
    Node *buff = new Node[maxid+1];
    for (int v = minid; v <= maxid; v++) {
        buff[v].vid = v;
        buff[v].value = (degr(v, IN)+1)*(degr(v, OUT)+1) ; // PRIORITY
    }
    sort( buff+minid, buff+maxid+1 );
    ofstream fout("order.txt");

    for (int v = minid; v <= maxid; v++) {
        fout << buff[v].vid << endl;
    }

    return 0;
}
