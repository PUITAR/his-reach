/*
 * @author: yangming
 * @Description: DEBUG
 * @Date: 2023-01-16 23:02:34
 */
#ifndef DEBUG_H_
#define DEBUG_H_
#include "till.h"
#include "../graph.h"
#include <iostream>
using namespace std;

void printL(TILL *till, int vid, char io) {
    L_t *lp = till->L(vid, io);
    int vnum = lp->vnum();
    int v, n;
    interval_t *itv;
    if (io == IN) printf("Lin(%d): ", vid);
    else printf("Lout(%d): ", vid);
    for (int i = 1; i <= vnum; i++) {
        v = lp->vid(i);
        itv = lp->intervals(i, n);
        for (int j = 0; j < n; j++)
            printf("<%d %d %d> ", v, TS(itv[j]), TE(itv[j]));
    }
    printf("\n");
    FreeL(lp);
}


void printN(Graph *g, int vid, char io) {
    N_t *np = g->N(vid, io);
    tuple_t *t;
    if (io == IN) printf("Nin(%d): ", vid);
    else printf("Nout(%d): ", vid);
    while ( (t = np->next()) != nullptr ) {
        printf("<%d %d> ", vidof(t), timeof(t));
    }
    printf("\n");
    freeN(np);
}

#endif // DEBUG_H_