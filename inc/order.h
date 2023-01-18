/*
 * @author: yangming
 * @Description: order class
 * @Date: 2023-01-14 16:42:56
 */

#ifndef ORDER_H_
#define ORDER_H_
#include <fstream>

class Order {
public:
    Order(const char OrderPath[], int buffsize) {
        p = new int[buffsize] ;
        q = new int[buffsize] ;
        std::ifstream fin(OrderPath) ;
        sz = 0; while (fin >> p[sz++]) ; // buffer has enough space
        for (int i = 0; i < sz; i++) q[p[i]] = i;
    }

    ~Order() { delete[] p; delete[] q; }

    // the vid of the idx-th vertex.
    int dro(int idx) { return p[idx]; }

    // the order of vid
    int ord(int vid) { return q[vid]; }

private:
    int *p, *q;
    int sz;

};

#endif // ORDER_H_