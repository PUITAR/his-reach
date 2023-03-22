/*
 * @author: yangming
 * @Description: TILL-INDEX DEFINITION
 * @Date: 2023-01-14 11:36:26
 */
#ifndef TILL_H_
#define TILL_H_
#include "rocksdb/c.h"
#include "order.h"
#include <assert.h>
#include <unistd.h>

#ifndef IOCONFIG
#define IOCONFIG
#define NIN_VID(vid)  ( vid & 0x7fffffff )
#define NOUT_VID(vid) ( vid | 0x80000000 )
#define IN  0
#define OUT 1
#endif  // IO CONFIG

#define MAX_BUFF_LEN 4096
#define NULL_VID     -1
#define INF          20000000


/**
 * set                 tp                        end
 * +---+---------------+-------------------------+
 * | t | (vid, pos)    | [ts, te]                |
 * +---+---------------+-------------------------+
 */
typedef long interval_t ; // [ts, te]

/*************************************** Lin/Lout TYPE ****************************************/
typedef struct L_t {
    // VARs
    int        *set = nullptr; 
    interval_t *tp  = nullptr;
    interval_t *end = nullptr;
    
    // FUNs

    // the num of the set of the vertexes in L.
    int vnum() const { return set==nullptr?0:(set[0]-1)>>1; }

    /** 
     *  +------------------------------------+
     *  | FUNCTIONS: `vidof` and `intervals` |
     *  +------------------------------------+
     */    

    // the i-th vertex in V(L(v, io))
    int vid(int idx) const { 
        return (set==nullptr or idx<1 or idx>vnum())? NULL_VID : set[(idx<<1)-1]; 
    }

    // intervals of idx-th vertex (&n contains the numbers of the intervals)
    interval_t * intervals(int idx , int &n) const {
        if (set == nullptr or idx<1 or idx>vnum()) {
            n = 0;
            return nullptr;
        }
        int p = set[idx<<1];
        int q = idx==vnum() ? end-tp : set[(idx+1)<<1];
        n = q-p; 
        return tp + p;
    }

} L_t ;

/*************************************** Lin/Lout TYPE ****************************************/

/*************************************** triplet type ****************************************/
typedef struct triplet_t {
    int ui, ts, te;
    int pri;
    triplet_t(int ui, int ts, int te):
        ui(ui), ts(ts), te(te) { pri = te-ts; } 

} triplet_t ;

/*************************************** triplet type ****************************************/

/**************************************** PUBLIC FUNs ****************************************/
// we always pop a triplet with the smallest value of `te-ts`
bool operator<(const triplet_t & a, const triplet_t &b ) { return a.pri > b.pri; } 

void FreeL(L_t *lp) {
    if (lp == nullptr) return ;
    if ( lp->set ) delete[] lp->set ;
    delete lp;
}

int TE(interval_t i) { return *(int*)&i; }
int TS(interval_t i) { return *((int*)&i+1); }

// a[ats, ate] dominate b[bts, bte]
bool dominate(interval_t a, interval_t b) {
    return TS(b)<=TS(a) and TE(a)<=TE(b); 
}

interval_t makeIntv(int ts, int te) {
    interval_t r = ts; r <<= 32; r += te;
    return r;
}

/*********************************************************************************************/


/************************************* TILL DEFINITION **************************************/

class TILL {
public:
    TILL(const char DBPath[], Order *o, int minid, int maxid, int mintime, int maxtime): 
        MINID(minid), MAXID(maxid), MINTIME(mintime), MAXTIME(maxtime), O(o)
    {
        // setup db options
        options = rocksdb_options_create();
        rocksdb_options_increase_parallelism(options, (int)(sysconf(_SC_NPROCESSORS_ONLN)));
        rocksdb_options_optimize_level_style_compaction(options, 0);
        rocksdb_options_set_create_if_missing(options, 1);
        readoptions = rocksdb_readoptions_create();
        writeoptions = rocksdb_writeoptions_create();
        // open db
        err = NULL;
        db = rocksdb_open(options, DBPath, &err);
        assert( !err );
        // order
        assert( O ) ;
        // buffer
        assert( buffer = new int[MAX_BUFF_LEN] );

    }

    ~TILL() {
        // cleanup
        rocksdb_writeoptions_destroy(writeoptions);
        rocksdb_readoptions_destroy(readoptions);
        rocksdb_options_destroy(options);
        rocksdb_close(db);
        delete[] buffer;

    }

    L_t * L(int vid, char io) {
        L_t *lp;
        size_t bytes;
        assert( lp = new L_t() );
        *(int*)key = io==IN?NIN_VID(vid):NOUT_VID(vid);
        err = NULL;
        lp->set = (int*)rocksdb_get(db, readoptions, key, sizeof(key), &bytes, &err);
        assert( not err );
        if ( lp->set ) {
            lp->tp  = (interval_t*)(lp->set+lp->set[0]);
            lp->end = (interval_t*)(lp->set+bytes/sizeof(int));
        }
        else lp->tp = lp->end = nullptr ;
        return lp;
    }

    bool SpanReach(int u, int v, int t1, int t2) {
        if ( ISIV(u, v, t1, t2) ) return false ;
        // variables: 
        interval_t itv; // itv = [t1, t2]
        int        i, j, w, x, Loun, Linn, wn, xn, Ow, Ox;
        L_t        *Lou, *Lin; // Lout(u) and Lin(v)
        interval_t *witvs, *xitvs; 
        // code:
        itv = makeIntv(t1, t2);
        i = 1, j = 1;
        Lou  = L(u, OUT);   Lin  = L(v, IN); 
        Loun = Lou->vnum(); Linn = Lin->vnum();
        while ( i <= Loun or j <= Linn ) {
            w = Lou->vid(i); 
            x = Lin->vid(j);
            witvs = Lou->intervals(i, wn);
            xitvs = Lin->intervals(j, xn);
            if ( w == v ) { 
                for (int wi = 0; wi < wn; wi++) {
                    if ( dominate(witvs[wi], itv) ) goto TRUE ;
                }
            }
            if ( x == u ) { 
                for (int xi = 0; xi < xn; xi++) {
                    if ( dominate(xitvs[xi], itv) ) goto TRUE ;
                }
            }
            if ( i > Loun) { j++; continue; }
            if ( j > Linn) { i++; continue; }
            Ow = O->ord(w);  Ox = O->ord(x);
            if ( Ow < Ox ) { i++; continue; }
            if ( Ow > Ox ) { j++; continue; }
            for (int wi = 0; wi < wn; wi++) {
                for (int xi = 0; xi < xn; xi++) {
                    if ( dominate(witvs[wi], itv) and dominate(xitvs[xi], itv) ) goto TRUE ;
                }
            }
            i++; j++;
        }

        FALSE: FreeL(Lou); FreeL(Lin); return false;
        TRUE:  FreeL(Lou); FreeL(Lin); return true;

    }

    void push(int vid, char io, int ui, int ts, int te) {
        // variables
        L_t        *lp  = L(vid, io);
        int        *set = lp->set;
        interval_t *tp  = lp->tp;
        interval_t *end = lp->end;
        interval_t *btp, *des, *src;
        int        bytes = 0, axis = 0, vnum = 0, cn = 0;
        
        // processing
        if ( set == nullptr ) {
            bytes = 5 * sizeof(int);
            buffer[0] = 3;
            buffer[1] = ui;
            buffer[2] = 0;
            *(interval_t*)(buffer+3) = makeIntv(ts, te);
        }
        else {
            // copy for vector 1
            vnum = lp->vnum();
            for (int i = 1; i <= vnum; i++) {
                buffer[(i<<1)-1] = set[(i<<1)-1];
                if ( axis == 0 ) buffer[i<<1] = set[i<<1];
                else             buffer[i<<1] = set[i<<1]+1;
                if ( set[(i<<1)-1]==ui ) axis = i;
            }
            if ( axis==0 ) {
                buffer[0] = set[0] + 2;
                bytes = (char*)end - (char*)set + (sizeof(int)<<2);
                axis = ++vnum;
                buffer[(axis<<1)-1] = ui;
                buffer[axis<<1] = end-tp;
            }
            else {
                buffer[0] = set[0];
                bytes = (char*)end - (char*)set + (sizeof(int)<<1);
            }
            // copy for vector 2
            btp = (interval_t*)(buffer+buffer[0]);
            for (int i = 1; i <= vnum; i++) {
                src = lp->intervals(i, cn);
                des = btp+buffer[i<<1];
                for (int j = 0; j < cn; j++) des[j] = src[j];
                if (i == axis) des[cn] = makeIntv(ts, te);
            }
        }
        err = NULL;
        *(int*)key = io==IN ? NIN_VID(vid) : NOUT_VID(vid);
        rocksdb_put(db, writeoptions, key, sizeof(key), (const char*)buffer, bytes, &err);
        // some datas will make mistakes here. (sad
        assert( not err );
        FreeL( lp );

    }

private:
    // functions
    bool ISIV(int u, int v, int t1, int t2) {
        return u < MINID || u > MAXID || v < MINID || v > MAXID || u == v || 
        t1 > t2 || t1 < MINTIME || t2 > MAXTIME ;
    }

    // variables
    rocksdb_t               *db;
    rocksdb_options_t       *options;
    rocksdb_readoptions_t   *readoptions;
    rocksdb_writeoptions_t  *writeoptions;
    char                    * err;
    int                     MINID, MAXID, MINTIME, MAXTIME ;
    Order                   *O;
    int                     *buffer;
    char                    key[sizeof(int)];

};

/************************************* TILL DEFINITION **************************************/

#endif // TILL_H_
