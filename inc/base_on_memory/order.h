/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-13 19:42:53
 */
#ifndef ORDER_H_
#define ORDER_H_

#include "graph.h"

#include <fstream>
using namespace std;

typedef struct deg_t {
    int id, val;

    deg_t() {}
    void set(int vid, int value) { id = vid, val = value; }
    bool operator<(const deg_t & o) const {
        return val > o.val;
    }

} deg_t; 

class Order {

public: // functions
    Order(const char order_path[]): order_pth(order_path) {}

    Order(const char order_path[], int buf_size): capacity(buf_size), order_pth(order_path) {
        p = new int[capacity];
        q = new int[capacity];
        ifstream fin(order_path);
        sz = 0; while (fin >> p[sz]) sz++;
        for (int i = 0; i < sz; i++) q[p[i]] = i;

    }

    ~Order() { delete[] p; delete[] q; }

    int dro(int idx) {
        assert(0 <= idx and idx < sz);
        return p[idx];
    }

    int ord(int vid) {
        if (vid < 0 or vid >= capacity) return INF;
        return q[vid];
    }

    void write_order_file(Graph *g) {
        cout << "writing order file...";
        ofstream fout(order_pth);
        vec<deg_t> temp(g->maxid+5);
        for (int i = g->minid; i <= g->maxid; i++) 
            temp.at(i).set(i, (g->degree_in(i)+1)*(g->degree_out(i)+1));
        temp.sort();
        for (int v = g->minid; v <= g->maxid; v++) 
            fout << temp.at(v).id << ' ';
        cout << "[fin]" << endl;
    }

    int size() { return sz; }

private:
    int *p, *q;
    int capacity;
    const char* order_pth;
    int sz;
};

#endif