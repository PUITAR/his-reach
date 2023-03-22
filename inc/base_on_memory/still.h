#ifndef STILL_H_
#define STILL_H_
#include "vec.h"
#include "bitmap.h"
#include "graph.h"
#include "que.h"
#include "order.h"
#include <fstream>

using namespace std;

typedef struct span_t {
    int ts, te, k;
    span_t() {}
    span_t(int ts, int te, int k): ts(ts), te(te), k(k) {}
    bool less(int t1, int t2) {
        long a = ts + te;
        long b = t1 + t2;
        if (a == b)
            if (ts == t1) return te < t2;
            else return ts < t1;
        return a < b;
    }
    bool equal(int t1, int t2) {
        return ts == t1 and te == t2;
    }
    bool dominate(int t1, int t2) {
        return t1 <= ts and te <= t2 ;
    }
} span_t ;

typedef struct triplet_t {
    int ui, ts, te, pri;
    triplet_t() {}
    triplet_t(int ui, int ts, int te): ui(ui), ts(ts), te(te) {
        pri = te - ts;
    }
    bool operator < (const triplet_t & o) { return pri > o.pri; }
} triplet_t;

class Still {
public:
    Still(
        int max_size, 
        const char vset_in_path[],
        const char vset_out_path[],
        const char span_in_path[],
        const char span_out_path[]
    ): max_size(max_size)
    {
        cout << "reading still files...";
        assert( span_in = new vec<vec<span_t>*> );
        assert( span_out = new vec<vec<span_t>*> );
        assert( vset_in = new vec<vec<int>*> );
        assert( vset_out = new vec<vec<int>*> );
        assert( bitmap = new Bitmap(max_size) );
        for (int i = 0; i < max_size; i++) {
            span_in->push_back(new vec<span_t>);
            span_out->push_back(new vec<span_t>);
            vset_in->push_back(new vec<int>);
            vset_out->push_back(new vec<int>);
        }
        ifstream span_in_reader(span_in_path);
        ifstream span_out_reader(span_out_path);
        ifstream vset_in_reader(vset_in_path);
        ifstream vset_out_reader(vset_out_path);
        int vid, n, ts, te, k, u;
        while (span_in_reader >> vid >> n) {
            while (n--) span_in_reader >> ts >> te >> k,
            span_in->at(vid)->push_back(span_t(ts, te, k));
        }
        while (span_out_reader >> vid >> n) {
            while (n--) span_out_reader >> ts >> te >> k,
            span_out->at(vid)->push_back(span_t(ts, te, k));
        }
        while (vset_in_reader >> vid >> n) {
            while (n--) vset_in_reader >> u,
            vset_in->at(vid)->push_back(u);
        }
        while (vset_out_reader >> vid >> n) {
            while (n--) vset_out_reader >> u,
            vset_out->at(vid)->push_back(u);
        }
        cout << "[fin]" << endl;
    }

    Still(int max_size): max_size(max_size) {
        assert( span_in = new vec<vec<span_t>*> );
        assert( span_out = new vec<vec<span_t>*> );
        assert( vset_in = new vec<vec<int>*> );
        assert( vset_out = new vec<vec<int>*> );
        assert( bitmap = new Bitmap(max_size) );
        for (int i = 0; i < max_size; i++) {
            span_in->push_back(new vec<span_t>);
            span_out->push_back(new vec<span_t>);
            vset_in->push_back(new vec<int>);
            vset_out->push_back(new vec<int>);
        }
    }

    ~Still() {
        vec<span_t> * p;
        vec<int> * q;
        for (int i = 0; i < span_in->size(); i++) {
            p = span_in->at(i);
            if (p) delete p;
            p = span_out->at(i);
            if (p) delete p;
            q = vset_in->at(i);
            if (q) delete q;
            q = vset_out->at(i);
            if (q) delete q;
        }
        delete span_in;
        delete span_out;
        delete vset_in;
        delete vset_out;
        delete bitmap;
    }

    bool still_reach(int u, int v, int ts, int te) {
        assert(0 <= u and u < max_size);
        assert(0 <= v and v < max_size);
        // variables
        int i, j, m, p, n, k;
        span_t t;
        vec<span_t> * svu = span_out->at(u), * svv = span_in->at(v);
        vec<int> * vsu = vset_out->at(u), * vsv = vset_in->at(v);
        bitmap->reset();
        // code
        i = 0, j = svv->size() - 1;
        while (i <= j) {
            m = i + j >> 1;
            t = svv->at(m), p = (long)t.ts + t.te >> 1;
            if (ts <= p and p <= te) { n = m; break; }
            else if (p < ts) i = m + 1;
            else j = m - 1;
        }
        if (i <= j) {
            k = n;
            while (k < svv->size()) {
                t = svv->at(k), p = (long)t.ts + t.te >> 1;
                if (p > te) break;
                if (t.dominate(ts, te)) {
                    m = k==svv->size()-1?vsv->size():svv->at(k+1).k;
                    j = svv->at(k).k;
                    while (j < m) {
                        i = vsv->at(j);
                        if (i == u) return true;
                        else bitmap->set(i);
                        j++;
                    }
                }
                k++;
            }
            k = n-1;
            while (k >= 0) {
                t = svv->at(k), p = (long)t.ts + t.te >> 1;
                if (p < ts) break;
                if (t.dominate(ts, te)) {
                    m = svv->at(k+1).k;
                    j = svv->at(k).k;
                    while (j < m) {
                        i = vsv->at(j);
                        if (i == u) return true;
                        else bitmap->set(i);
                        j++;
                    }
                }
                k--;
            }
        }
        i = 0, j = svu->size() - 1;
        while (i <= j) {
            m = i + j >> 1;
            t = svu->at(m), p = (long)t.ts + t.te >> 1;
            if (ts <= p and p <= te) { n = m; break; }
            else if (p < ts) i = m + 1;
            else j = m - 1;
        }
        if (i <= j) {
            k = n;
            while (k < svu->size()) {
                t = svu->at(k), p = (long)t.ts + t.te >> 1;
                if (p > te) break;
                if (t.dominate(ts, te)) {
                    m = k==svu->size()-1?vsu->size():svu->at(k+1).k;
                    j = svu->at(k).k;
                    while (j < m) {
                        i = vsu->at(j);
                        if (i == v or bitmap->test(i)) return true;
                        j++;
                    }
                }
                k++;
            }
            k = n-1;
            while (k >= 0) {
                t = svu->at(k), p = (long)t.ts + t.te >> 1;
                if (p < ts) break;
                if (t.dominate(ts, te)) {
                    m = svu->at(k+1).k;
                    j = svu->at(k).k;
                    while (j < m) {
                        i = vsu->at(j);
                        if (i == v or bitmap->test(i)) return true;
                        j++;
                    }
                }
                k--;
            }
        }
        return false;
    }

    void still_construct(
            const char order_path[], 
            const char graph_path[], 
            int theta
        ) {
        Graph * g = new Graph(graph_path); assert(g);
        pque<triplet_t> * q = new pque<triplet_t>(1000000000); assert(q);
        Order * o = new Order(order_path, max_size); assert(o);

        for (int i = 0; i < o->size(); i++) {
            int ui = o->dro(i);
            printf("u[%d]: %d\n", i, ui);
            q->push(triplet_t(ui, +INF, -INF));
            while (not q->empty()) {
                triplet_t tri = q->pop();
                int v = tri.ui, ts = tri.ts, te = tri.te;
                if (ui != v) 
                    if (still_reach(ui, v, ts, te)) continue;
                    else push_Lin(v, ui, ts, te);
                edge_set_t * egs = g->get_Nout(v);
                for (int k = 0; k < egs->size(); k++) {
                    edge e = egs->at(k);
                    if (o->ord(e.id) <= o->ord(ui)) continue;
                    int t1 = min(ts, e.timestamp), t2 = max(te, e.timestamp);
                    if (t2-t1+1 > theta) continue;
                    else q->push(triplet_t(e.id, t1, t2));
                }
            }
            q->push(triplet_t(ui, +INF, -INF));
            while (not q->empty()) {
                triplet_t tri = q->pop();
                int v = tri.ui, ts = tri.ts, te = tri.te;
                if (ui != v)
                    if (still_reach(v, ui, ts, te)) continue;
                    else push_Lout(v, ui, ts, te);
                edge_set_t * egs = g->get_Nin(v);
                for (int k = 0; k < egs->size(); k++) {
                    edge e = egs->at(k);
                    if (o->ord(e.id) <= o->ord(ui)) continue;
                    int t1 = min(ts, e.timestamp), t2 = max(te, e.timestamp);
                    if (t2-t1+1 > theta) continue;
                    else q->push(triplet_t(e.id, t1, t2));
                }
            }
        }
        delete g, delete q, delete o;
    }

    void write_still_file(
        const char vset_in_path[], 
        const char vset_out_path[], 
        const char span_in_path[], 
        const char span_out_path[]
    )
    {
        cout << "writing still files...";
        ofstream vset_in_writer(vset_in_path);
        ofstream vset_out_writer(vset_out_path);
        ofstream span_in_writer(span_in_path);
        ofstream span_out_writer(span_out_path);
        for (int i = 0; i < max_size; i++) {
            vset_in_writer << i << ' ' << vset_in->at(i)->size() << ' ';
            vset_out_writer << i << ' ' << vset_out->at(i)->size() << ' ';
            span_in_writer << i << ' ' << span_in->at(i)->size() << ' ';
            span_out_writer << i << ' ' << span_out->at(i)->size() << ' ';
            for (int j = 0; j < vset_in->at(i)->size(); j++) vset_in_writer << vset_in->at(i)->at(j) << ' ';
            for (int j = 0; j < vset_out->at(i)->size(); j++) vset_out_writer << vset_out->at(i)->at(j) << ' ';
            for (int j = 0; j < span_in->at(i)->size(); j++) span_in_writer << span_in->at(i)->at(j).ts << ' ' << span_in->at(i)->at(j).te << ' ' << span_in->at(i)->at(j).k << ' ';
            for (int j = 0; j < span_out->at(i)->size(); j++) span_out_writer << span_out->at(i)->at(j).ts << ' ' << span_out->at(i)->at(j).te << ' ' << span_out->at(i)->at(j).k << ' ';
        }
        cout << "[fin]" << endl;
    }

private: // functions
    void push_Lin(int vid, int ui, int ts, int te) {
        assert(0 <= vid and vid < max_size);
        vec<span_t> * sv = span_in->at(vid);
        bool find = false;
        int k;
        for (int i = 0; i < sv->size(); i++) {
            span_t * temp = sv->pat(i);
            if (temp->less(ts, te)) continue;
            else if (temp->equal(ts, te)) find = true, k = temp->k;
            else if (find) temp->k++;
            else find = true, sv->insert(i, span_t(ts, te, k = temp->k));
        }
        if (not find) sv->push_back(span_t(ts, te, k = vset_in->at(vid)->size()));
        vset_in->at(vid)->insert(k, ui);
    }

    void push_Lout(int vid, int ui, int ts, int te) {
        assert(0 <= vid and vid < max_size);
        vec<span_t> * sv = span_out->at(vid);
        bool find = false;
        int k;
        for (int i = 0; i < sv->size(); i++) {
            span_t * temp = sv->pat(i);
            if (temp->less(ts, te)) continue;
            else if (temp->equal(ts, te)) find = true, k = temp->k;
            else if (find) temp->k++;
            else find = true, sv->insert(i, span_t(ts, te, k = temp->k));
        }
        if (not find) sv->push_back(span_t(ts, te, k = vset_out->at(vid)->size()));
        vset_out->at(vid)->insert(k, ui);
    }


private: // variables
    Bitmap * bitmap;
    vec<vec<span_t>*>* span_in; 
    vec<vec<span_t>*>* span_out; 
    vec<vec<int>*>* vset_in; 
    vec<vec<int>*>* vset_out; 
    int max_size;

} ;

#endif // STILL_H_
