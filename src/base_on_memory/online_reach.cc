/*
 * @author: yangming
 * @Description: online-reach
 * @Date: 2023-03-12 19:30:16
 */
#include "graph.h"
#include "bitmap.h"
#include "timer.h"
#include "que.h"

using namespace std;

#define LUT_FILTER 0

#if (LUT_FILTER)
#define MININ  0
#define MAXIN  1
#define MINOUT 2
#define MAXOUT 3
#endif  // LUT


/************************* Global variables *************************/
Graph *g;
Bitmap *Ru, *Rv;
Que<int> *Qu, *Qv;
edge eg;
int toggle, w, ti, s, l;
edge_set_t *edges;
int theta;

#if (LUT_FILTER)
int (*lut)[4];
#endif

/************************* Span Reachiability *************************/
bool online_reach(int u, int v, int ts, int te) {
    if (te - ts + 1 > theta) return false;
    // reset
    Ru->reset(), Rv->reset(), Qu->reset(), Qv->reset();
    // code
    Ru->set(u), Rv->set(v), Qu->push(u), Qv->push(v);
    toggle = v;
    while ( Qu->size() + Qv->size() > 0 ) {
        if ( toggle == v and not Qu->empty() ) {
            toggle = u;
            l = Qu->size(); 
            while ( l-- ) {
                w = Qu->pop();
                #if (LUT_FILTER)
                if (lut[w][MAXOUT] < ts or lut[w][MINOUT] > te) continue;
                #endif
                edges = g->get_Nout(w);
                for (int i = 0; i < edges->size(); i++) {
                    eg = edges->at(i);
                    ti = eg.timestamp;
                    if (ti < ts or te < ti) continue ;
                    s = eg.id;
                    if (Rv->test(s)) return true;
                    if (not Ru->test(s)) Qu->push(s), Ru->set(s);
                }
            }
        }
        else {
            toggle = v;
            l = Qv->size();
            while ( l-- ) {
                w = Qv->pop();
                #if (LUT_FILTER)
                if (lut[w][MAXIN] < ts or lut[w][MININ] > te) continue;
                #endif
                edges = g->get_Nin(w);
                for (int i = 0; i < edges->size(); i++) {
                    eg = edges->at(i);
                    ti = eg.timestamp;
                    if (ti < ts or te < ti) continue;
                    s = eg.id;
                    if (Ru->test(s)) return true;
                    if (not Rv->test(s)) Qv->push(s), Rv->set(s);
                }
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    // initialize
    assert( g = new Graph(argv[1]) );
    printf("cond = %d %d %d %d\n", g->minid, g->maxid, g->mintime, g->maxtime);
    assert( Ru = new Bitmap(g->maxid+5) );
    assert( Rv = new Bitmap(g->maxid+5) );
    assert( Qu = new Que<int>(g->maxid+5) );
    assert( Qv = new Que<int>(g->maxid+5) );
    theta = atoi(argv[2]);
    Timer timer;
    int u, v, ts, te;
    ifstream fin("testcase.txt");
    ofstream fout("online_reach_res.txt");

    #if (LUT_FILTER)
    assert( lut = new int[g->maxid+5][4] );
    for (int i = g->minid; i <= g->maxid; i++) {
        lut[i][MININ] = lut[i][MINOUT] = +INF;
        lut[i][MAXIN] = lut[i][MAXOUT] = -INF;
        edge_set_t * Nin = g->get_Nin(i), *Nout = g->get_Nout(i);
        for (int j = 0; j < Nin->size(); j++) {
            lut[i][MININ] = min(lut[i][MININ], Nin->at(j).timestamp);
            lut[i][MAXIN] = max(lut[i][MAXIN], Nin->at(j).timestamp);
        }
        for (int j = 0; j < Nout->size(); j++) {
            lut[i][MINOUT] = min(lut[i][MINOUT], Nout->at(j).timestamp);
            lut[i][MAXOUT] = max(lut[i][MAXOUT], Nout->at(j).timestamp);
        }
    }
    #endif

    // code
    cout << "querying...";
    timer.StartTimer();
    while (fin >> u >> v >> ts >> te) 
        fout << online_reach(u, v, ts, te) << endl;
    timer.StopTimer();
    fout << timer.GetTimerCount() << " ns" << endl;
    cout << "[fin]" << endl;

    return 0;
}