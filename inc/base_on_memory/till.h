#ifndef TILL_H_
#define TILL_H_

#include "vec.h"
#include "order.h"
#include "que.h"
#include "graph.h"

using namespace std;

typedef struct vset_t {
    int id, k;

    vset_t() {}
    vset_t(int vid, int bias) { id = vid, k = bias; }
} vset_t ;

typedef struct span_t {
    int ts, te;

    span_t() {}
    span_t(int t1, int t2) { ts = t1, te = t2; }
    bool dominate(int t1, int t2) {
        return t1 <= ts and te <= t2; }

} span_t ;


typedef struct triplet_t {
    int ui, ts, te, pri;

    triplet_t() {}
    triplet_t(int ui, int ts, int te): ui(ui), ts(ts), te(te) { pri = te - ts; }
} triplet_t ;
bool operator<(const triplet_t & a, const triplet_t &b ) { return a.pri > b.pri; } 

class Till {
public:

    Till(
        int max_size, 
        const char order_path[],
        const char vset_in_path[], 
        const char vset_out_path[], 
        const char span_in_path[], 
        const char span_out_path[]
    ): max_size(max_size)
    {
        cout << "reading till files...";
        assert( o = new Order(order_path, max_size) );
        assert( vset_in = new vec<vec<vset_t>*> ) ;
        assert( vset_out = new vec<vec<vset_t>*> );
        assert( span_in = new vec<vec<span_t>*> ) ;
        assert( span_out = new vec<vec<span_t>*> );
        for (int i = 0; i < max_size; i++) 
            vset_in->push_back(new vec<vset_t>) ,
            vset_out->push_back(new vec<vset_t>),
            span_in->push_back(new vec<span_t>) , 
            span_out->push_back(new vec<span_t>);
        ifstream vset_in_reader(vset_in_path);
        ifstream vset_out_reader(vset_out_path);
        ifstream span_in_reader(span_in_path);
        ifstream span_out_reader(span_out_path);

        int vid, n, id, k, ts, te;
        while (vset_in_reader >> vid >> n) {
            while (n--) {
                vset_in_reader >> id >> k;
                vset_in->at(vid)->push_back(vset_t(id, k));
            }
        }
        while (vset_out_reader >> vid >> n) {
            while (n--) {
                vset_out_reader >> id >> k;
                vset_out->at(vid)->push_back(vset_t(id, k));
            }
        }
        while (span_in_reader >> vid >> n) {
            while (n--) {
                span_in_reader >> ts >> te;
                span_in->at(vid)->push_back(span_t(ts, te));
            }
        }
        while (span_out_reader >> vid >> n) {
            while (n--) {
                span_out_reader >> ts >> te;
                span_out->at(vid)->push_back(span_t(ts, te));
            }
        }
        cout << "[fin]" << endl;
    }

    Till(int max_size, const char order_path[]): max_size(max_size) {
        assert( o = new Order(order_path, max_size) );
        assert( vset_in = new vec<vec<vset_t>*> ) ;
        assert( vset_out = new vec<vec<vset_t>*> );
        assert( span_in = new vec<vec<span_t>*> ) ;
        assert( span_out = new vec<vec<span_t>*> );
        for (int i = 0; i < max_size; i++) 
            vset_in->push_back(new vec<vset_t>) ,
            vset_out->push_back(new vec<vset_t>),
            span_in->push_back(new vec<span_t>) , 
            span_out->push_back(new vec<span_t>);

    }

    ~Till() {
        if (o) delete o;
        vec<vset_t>* p;
        vec<span_t>* q;
        for (int i = 0; i < vset_in->size(); i++) {
            p = vset_in->at(i);
            if (p) delete p;
        }
        for (int i = 0; i < vset_out->size(); i++) {
            p = vset_out->at(i);
            if (p) delete p;
        }
        for (int i = 0; i < span_in->size(); i++) {
            q = span_in->at(i);
            if (q) delete q;
        }
        for (int i = 0; i < span_out->size(); i++) {
            q = span_out->at(i);
            if (q) delete q;
        }
        delete vset_in;
        delete vset_out;
        delete span_in;
        delete span_out;
    }

    bool till_reach(int u, int v, int ts, int te) {
        assert(0 <= u and u < max_size);
        assert(0 <= v and v < max_size);
        int i, j, w, x, loun, linn, wi, wn, xi, xn, ow, ox;
        // code
        i = j = 0;
        vec<vset_t> *vin = vset_in->at(v), *vou = vset_out->at(u);
        vec<span_t> *sin = span_in->at(v), *sou = span_out->at(u);
        while (i < vou->size() or j < vin->size()) {
            if (i < vou->size()) w = vou->at(i).id, get_range_of_spans(vou, sou, i, wi, wn);
            else w = INF, wi = wn = 0;
            if (j < vin->size()) x = vin->at(j).id, get_range_of_spans(vin, sin, j, xi, xn);
            else x = INF, xi = xn = 0;
            if (w == v) 
                for (int m = wn-1; m >= wi; m--)
                    if (sou->at(m).dominate(ts, te)) return true;
            if (x == u)
                for (int n = xn-1; n >= xi; n--)
                    if (sin->at(n).dominate(ts, te)) return true;
            if (i >= vou->size()) { j++; continue; }
            if (j >= vin->size()) { i++; continue; }
            ow = o->ord(w), ox = o->ord(x);
            if (ow < ox) { i++; continue; }
            if (ow > ox) { j++; continue; }
            for (int m = wn-1; m >= wi; m--)
                for (int n = xn-1; n >= xi; n--)
                    if (sou->at(m).dominate(ts, te) and sin->at(n).dominate(ts, te)) return true;
            i++, j++;
        }
        return false;
    }

    void push_Lin(int vid, int ui, int ts, int te) {
        assert(0 <= vid and vid < max_size);
        vec<vset_t> * vs = vset_in->at(vid);
        bool find = false ;
        int k;
        for (int i = 0; i < vs->size(); i++) {
            vset_t * temp = vs->pat(i);
            if (find) temp->k++;  
            else if (temp->id == ui) find = true, k = temp->k;
        }
        if (not find) vs->push_back(vset_t(ui, k = span_in->at(vid)->size()));
        span_in->at(vid)->insert(k, span_t(ts, te));

    }

    void push_Lout(int vid, int ui, int ts, int te) {
        assert(0 <= vid and vid < max_size) ;
        vec<vset_t> * vs = vset_out->at(vid);
        bool find = false;
        int k;
        for (int i = 0; i < vs->size(); i++) {
            vset_t * temp = vs->pat(i);
            if (find) temp->k++;
            else if (temp->id == ui) find = true, k = temp->k;
        }
        if (not find) vs->push_back(vset_t(ui, k = span_out->at(vid)->size()));
        span_out->at(vid)->insert(k, span_t(ts, te));

    }

    void till_construct(const char graph_path[], int theta) {
        Graph * g = new Graph(graph_path); assert(g);
        pque<triplet_t> * q = new pque<triplet_t>(1000000000); assert(q);
        for (int i = 0; i < o->size(); i++) {
            int ui = o->dro(i);
            printf("u[%d]: %d\n", i, ui);
            q->push(triplet_t(ui, +INF, -INF));
            while (not q->empty()) {
                triplet_t tri = q->pop();
                int v = tri.ui, ts = tri.ts, te = tri.te;
                if (ui != v) 
                    if (till_reach(ui, v, ts, te)) continue;
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
                    if (till_reach(v, ui, ts, te)) continue;
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
        delete g, delete q;
    }

    void write_till_file(
        const char vset_in_path[], 
        const char vset_out_path[], 
        const char span_in_path[], 
        const char span_out_path[]
    ) 
    {
        cout << "writing till files...";
        ofstream vset_in_writer(vset_in_path);
        ofstream vset_out_writer(vset_out_path);
        ofstream span_in_writer(span_in_path);
        ofstream span_out_writer(span_out_path);
        for (int i = 0; i < max_size; i++) {
            vset_in_writer << i << ' ' << vset_in->at(i)->size() << ' ';
            vset_out_writer << i << ' ' << vset_out->at(i)->size() << ' ';
            span_in_writer << i << ' ' << span_in->at(i)->size() << ' ';
            span_out_writer << i << ' ' << span_out->at(i)->size() << ' ';
            for (int j = 0; j < vset_in->at(i)->size(); j++) vset_in_writer << vset_in->at(i)->at(j).id << ' ' << vset_in->at(i)->at(j).k << ' ';
            for (int j = 0; j < vset_out->at(i)->size(); j++) vset_out_writer << vset_out->at(i)->at(j).id << ' ' << vset_out->at(i)->at(j).k << ' ';
            for (int j = 0; j < span_in->at(i)->size(); j++) span_in_writer << span_in->at(i)->at(j).ts << ' ' << span_in->at(i)->at(j).te << ' ';
            for (int j = 0; j < span_out->at(i)->size(); j++) span_out_writer << span_out->at(i)->at(j).ts << ' ' << span_out->at(i)->at(j).te << ' ';
        }
        cout << "[fin]" << endl;
    }

private: //functions

    void get_range_of_spans(vec<vset_t> *vs, vec<span_t> *ss, int idx, int &beg, int &end) {
        assert(0 <= idx and idx < vs->size());
        beg = vs->at(idx).k;
        if (idx == vs->size()-1) end = ss->size();
        else end = vs->at(idx+1).k;
    }

private: // variables
    vec<vec<vset_t>*>* vset_in;
    vec<vec<vset_t>*>* vset_out;
    vec<vec<span_t>*>* span_in;
    vec<vec<span_t>*>* span_out;
    int max_size;
    Order * o;

};

#endif