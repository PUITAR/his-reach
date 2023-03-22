/*
 * @author: yangming
 * @Description: TILL-Construct
 * @Date: 2023-01-14 15:08:09
 */
#include "../graph.h"
#include "till.h"
#include "debug.h"
#include "../timer.h"
#include <stdlib.h>
#include <queue>
using namespace std;

#define MIN(a, b) ( a < b ? a : b )
#define MAX(a, b) ( a > b ? a : b )
typedef priority_queue<triplet_t> pq_t;

/********************** GLOBAL VARs ***********************/
int minid, maxid, mintime, maxtime;
int ui, v, ts, te, w, tsw, tew, tw, theta;
pq_t      *Q;
Order     *O;
Graph     *g;
TILL      *till;
N_t       *n;
tuple_t   *t;
Timer     *timer;

/********************** GLOBAL VARs ***********************/


/** +------------------------------+
 *  | @description: TILL-Construct |
 *  +------------------------------+
 */
int main(int argc, char *argv[]) { 
    system("rm -rf index/*");

PREPARE:
    minid   = atoi(argv[1]);
    maxid   = atoi(argv[2]);
    mintime = atoi(argv[3]);
    maxtime = atoi(argv[4]);
    assert( Q = new pq_t() );
    assert( timer = new Timer() );

INPUT: 
    // a temporal graph
    assert ( g = new Graph("../db") );
    // a vertex order O
    assert ( O = new Order("order.txt", maxid+1) );
    // a parameter theta (the default value of theta is +INF )
    theta = +INF ;

OUTPUT: 
    assert( till = new TILL("./index", O, minid, maxid, mintime, maxtime) );

CODE:
    timer->StartTimer();

    for (int i = 0; i < maxid; i++) {
        ui = O->dro( i ); // the i-th vertex in O.
        printf("u%d: %d\n", i, ui);
        // printf("[Lin]\n");
        assert( Q->empty() ) ;
        Q->push( triplet_t(ui, +INF, -INF) );
        while ( not Q->empty() ) {
            triplet_t tri = Q->top(); Q->pop();
            v = tri.ui; ts = tri.ts; te = tri.te;
            // printf("pop <%d %d %d>\n", v, ts, te);
            if ( ui != v ) {
                // printL(till, ui, OUT);
                // printL(till, v, IN);
                if ( till->SpanReach(ui, v, ts, te) ) { 
                    // printf("%d->%d[%d,%d] continue\n", ui, v, ts, te); 
                    continue; 
                }
                else {
                    // printf("Lin(%d) <= <%d %d %d>\n", v, ui, ts, te);
                    till->push(v, IN, ui, ts, te); 
                }
            }
            // printf("Nout(%d):\n", v);
            n = g->N(v, OUT);
            while ( (t = n->next()) != nullptr ) {
                w = vidof(t); tw = timeof(t); 
                // printf("<%d %d> ", w, tw);
                if ( O->ord(w) <= O->ord(ui) ) { 
                    // printf("O(%d)<=O(%d): continue\n", w, ui);
                    continue ; 
                }
                tsw = MIN(ts, tw); tew = MAX(te, tw);
                if ( tew-tsw+1 > theta ) continue ;
                else {
                    // printf("Q <= <%d %d %d>\n", w, tsw, tew);
                    Q->push( triplet_t(w, tsw, tew) );
                }
            }
            freeN(n);
        }
        // Lout: repeat line 49-66 to construct.
        // printf("[Lout]\n");
        assert( Q->empty() );
        Q->push( triplet_t(ui, +INF, -INF) );
        while ( not Q->empty() ) {
            triplet_t tri = Q->top(); Q->pop();
            v = tri.ui; ts = tri.ts; te = tri.te;
            // printf("pop <%d %d %d>\n", v, ts, te);
            if ( ui != v ) {
                // printL(till, v, OUT);
                // printL(till, ui, IN);
                if ( till->SpanReach(v, ui, ts, te) ) {
                    // printf("%d->%d[%d,%d] continue\n", v, ui, ts, te); 
                    continue ;
                }
                else {
                    // printf("Lout(%d) <= <%d %d %d>\n", v, ui, ts, te);
                    till->push(v, OUT, ui, ts, te);
                }
            }
            // printf("Nin(%d):\n", v);
            n = g->N(v, IN);
            while ( (t = n->next()) != nullptr ) {
                w = vidof(t); tw = timeof(t);
                // printf("<%d %d> ", w, tw);
                if ( O->ord(w) <= O->ord(ui) ) {
                    // printf("O(%d)<=O(%d): continue\n", w, ui);
                    continue ;
                }
                tsw = MIN(ts, tw); tew = MAX(te, tw);
                if ( tew-tsw+1 > theta ) continue ;
                else {
                    // printf("Q <= <%d %d %d>\n", w, tsw, tew);
                    Q->push( triplet_t(w, tsw, tew) );
                }
            }
            freeN(n);
        }
    }
    timer->StopTimer();

    printf("timer: %.0f ns\n", timer->GetTimerCount());

END:

    return 0;

}

