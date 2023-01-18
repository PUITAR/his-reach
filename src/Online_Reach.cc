/*
 * @author: yangming
 * @Description: Online-Reach
 * @Date: 2023-01-12 22:27:47
 */
#include "../timer.h"
#include "../graph.h"
#include <bitset>
#include <queue>
#include <stdlib.h>
#include <fstream>
using namespace std;

/******************** Preparation ********************/
// MICRO DEFINITION and LIMIT VARs: different graph differnt micro
#define MAX_BITSET_SIZE 2048
#define INF             20000000

// TYPE DEFINITION
typedef bitset<MAX_BITSET_SIZE> set_t;
typedef queue<int> queue_t;

/*****************************************************/


/********************** global variables **********************/
set_t    *Ru, *Rv; 
queue_t  *Qu, *Qv;
int      toggle, l, w; 
N_t      *n;
tuple_t  *t;
int      minid;
int      maxid;
int      mintime;
int      maxtime;

bool ISIV(int u, int v, int ts, int te) {
    return ( u < minid || u > maxid || v < minid || v > maxid || ts > te || ts < mintime || te > maxtime || u == v ) ;
    // Input Is InValid
}

/**************************************************************/
bool OnlineReach(Graph * g, int u, int v, int ts, int te) {
    if ( ISIV(u, v, ts, te) ) return false; 
    // ISV (Input Is Valid)
    Ru->set(u); Rv->set(v);
    Qu->push(u); Qv->push(v);
    toggle = v;
    while ( Qu->size() + Qv->size() > 0 ) { // Qu U Qv != empty
        if ( toggle == v and not Qu->empty() ) {
            toggle = u;
            l = Qu->size();
            for (int i = 1; i <= l; i++) {
                w = Qu->front(); Qu->pop();
                n = g->N(w, OUT);
                while ( (t = n->next())!=nullptr ) {
                    int ti = timeof(t);
                    if ( ti < ts or ti > te ) continue ;
                    int s = vidof(t);
                    if ( Rv->test(s) ) return true;
                    if ( not Ru->test(s) ) {
                        Qu->push(s); 
                        Ru->set(s);
                    }
                }
                freeN(n);
            }
        }
        else { // repeat if codeblock in line 41
            toggle = v;
            l = Qv->size();
            for (int i = 1; i <= l; i++) {
                w = Qv->front(); Qv->pop();
                n = g->N(w, IN);
                while ( (t = n->next())!=nullptr ) {
                    int ti = timeof(t);
                    if ( ti < ts or ti > te ) continue ;
                    int s = vidof(t);
                    if ( Ru->test(s) ) return true;
                    if ( not Rv->test(s) ) {
                        Qv->push(s);
                        Rv->set(s);
                    }
                }
                freeN(n);
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {

    // initialize
    minid   = atoi(argv[1]);
    maxid   = atoi(argv[2]);
    mintime = atoi(argv[3]);
    maxtime = atoi(argv[4]);
    Graph *g = new Graph("../db");
    Ru = new set_t;
    Rv = new set_t;
    Qu = new queue_t;
    Qv = new queue_t;
    Timer timer;
    int u, v, t1, t2;
    ifstream cin(argv[5]);
    ofstream cout(argv[6]);

    // test
    timer.StartTimer();

    while ( cin >> u >> v >> t1 >> t2 ) {
        // query preparation
        Ru->reset(); Rv->reset();
        while (not Qu->empty()) Qu->pop();
        while (not Qv->empty()) Qv->pop();
        // query processing
        cout << "Result: " 
        << ( OnlineReach(g, u, v, t1, t2) == true ? "Ture" : "False" ) << endl;
    }
    timer.StopTimer();
    cout << "Cost: "
        << timer.GetTimerCount() << " ns" << endl;
    return 0;
}