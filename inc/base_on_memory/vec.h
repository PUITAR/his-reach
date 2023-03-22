/*
 * @author: yangming
 * @Description: vector class
 * @Date: 2023-03-12 20:30:19
 */
#ifndef VEC_H_
#define VEC_H_

#include <assert.h>
#include <iostream>
#include <algorithm>
using namespace std;


template < typename Item > 
class vec {

public:
    vec() { sz = capacity = 0; buf = nullptr; }

    vec(int size) {
        assert( size > 0 ); 
        assert( buf = new Item[sz = capacity = size] );
    }

    ~vec() { if (buf) delete [] buf; }

    void push_back(const Item & e) {
        if (sz >= capacity) {
            if (capacity) capacity <<= 1;
            else capacity = 1;
            Item * temp = new Item[capacity];
            for (int i = 0; i < sz; i++) temp[i] = buf[i];
            if (buf) delete[] buf;
            buf = temp;
        }
        buf[sz++] = e;
    }

    void insert(int k, const Item &e) {
        assert(0 <= k and k <= sz);
        if (sz >= capacity) {
            if (capacity) capacity <<= 1;
            else capacity = 1;
            Item * temp = new Item[capacity];
            for (int i = 0; i < sz; i++) temp[i] = buf[i];
            if (buf) delete[] buf;
            buf = temp;
        }
        for (int i = sz++; i > k; i--) buf[i] = buf[i-1];
        buf[k] = e;

    }

    Item at(int idx) {
        assert(0 <= idx and idx < sz);
        return buf[idx];
    }

    Item * pat(int idx) {
        assert(0 <= idx and idx < sz);
        return &buf[idx];
    }

    void sort() { std::sort(buf, buf+sz); }

    int size() { return sz; }

private:
    Item * buf;
    int capacity;
    int sz;

};


#endif