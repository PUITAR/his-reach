/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-13 22:23:19
 */
#ifndef QUE_H_
#define QUE_H_

#include <assert.h>

template < typename Item >
class Que {
public:
    explicit Que(int buf_size): max_size(buf_size) {
        buf = new Item[max_size];
        front = 0, rear = -1;
    }

    ~Que() { if (buf) delete[] buf; }

    void push(const Item &e) {
        assert(rear+1<max_size);
        buf[++rear] = e;
    }

    Item pop() {
        assert(front <= rear);
        return buf[front++];
    }

    bool empty() { return rear < front; }

    void reset() { front = 0, rear = -1; }

    int size() { return rear - front + 1; }

private:
    Item * buf;
    int max_size;
    int front, rear;

};


template< typename Item > 
class pque {

public: // functions
    pque(int cap): sz(0), capacity(cap) {
        assert(cap > 0);
        buf = new Item[capacity];
    }

    bool empty() { return sz == 0; }

    void clear() { sz = 0; }

    void push(const Item & e) { buf[++sz] = e, swim(sz); }

    Item pop() {
        Item m = buf[1];
        swap(buf[1], buf[sz--]);
        sink(1);
        return m;
    }

    int size() { return sz; }

    ~pque() { delete[] buf; }

private: // functions
    void swim(int k) {
        while (k > 1 and buf[k/2] < buf[k]) swap(buf[k/2], buf[k]), k/=2; }
    
    void sink(int k) {
        while (2*k <= sz) {
            int j = 2 * k;
            if (j < sz and buf[j] < buf[j+1]) j ++ ;
            if ( not (buf[k] < buf[j]) ) break;
            swap(buf[k], buf[j]);
            k = j;
        }
    }

private: // variables
    int capacity, sz;
    Item * buf;

};

#endif