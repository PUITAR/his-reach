/*
 * @author: yangming
 * @Description: graph
 * @Date: 2023-01-12 12:08:24
 */
#ifndef GRAPH_H_
#define GRAPH_H_
/*********************** rocksdb *************************/
#include "rocksdb/c.h"
#include <unistd.h>
#include <assert.h>

/*********************************************************/
#ifndef IOCONFIG
#define IOCONFIG
#define NIN_VID(vid)  ( vid & 0x7fffffff )
#define NOUT_VID(vid) ( vid | 0x80000000 )
#define IN  0
#define OUT 1
#endif

// namespace OnlineReach {  }; 


/**************************** Nin/Nout Type *****************************/
typedef long tuple_t;
/** 
 * 8 bits : xxxx xxxx
 *          vid  timestamp  
 */

typedef struct N_t {
    // variables
    char * set = nullptr;
    size_t len = 0;
    tuple_t * cur = nullptr;
    // functions
    N_t() = default ;
    // return now element, and cur++
    tuple_t * next() { 
        tuple_t * r = cur++;
        return r >= (long*)(set+len) ? nullptr : r ;
    }

} N_t;

/************************************************************************/


class Graph {
public:
    Graph(const char DBPath[]) {
        // setup db options
        options = rocksdb_options_create();
        rocksdb_options_increase_parallelism(options, (int)(sysconf(_SC_NPROCESSORS_ONLN)));
        rocksdb_options_optimize_level_style_compaction(options, 0);
        rocksdb_options_set_create_if_missing(options, 1);
        
        // setup db readoptions
        readoptions = rocksdb_readoptions_create();
        
        // open db
        err = NULL;
        db = rocksdb_open(options, DBPath, &err);
        assert( !err );

    }

    ~Graph() {
        // cleanup
        rocksdb_readoptions_destroy(readoptions);
        rocksdb_options_destroy(options);
        rocksdb_close(db);
    }

    N_t * N(int vid, char io) {
        N_t * n = new N_t();
        assert( n );
        char key[sizeof(int)];
        *(int*)key = ( io == IN ? NIN_VID(vid) : NOUT_VID(vid) );
        err = NULL;
        n->set = rocksdb_get(db, readoptions, key, sizeof(key), &n->len, &err);
        n->cur = (tuple_t *)n->set;
        assert( !err );
        return n;

    }

private:
    rocksdb_t * db;
    rocksdb_options_t * options;
    rocksdb_readoptions_t * readoptions;
    char * err;
};

/**************************** Public Functions *****************************/

int timeof(tuple_t * tp) { return *(int*)tp; }

int vidof(tuple_t * tp) { return *((int*)tp+1); }

void freeN(N_t * np) {
    if (np == nullptr) return ;
    if (np->set != nullptr) delete[] np->set;
    delete np; 
}

/***************************************************************************/

#endif // Graph_h_