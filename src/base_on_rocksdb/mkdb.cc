/*
 * @author: yangming
 * @Description: make db
 * @Date: 2023-01-12 21:08:50
 */
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "rocksdb/c.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>
using namespace std;

#define NIN_VID(vid)  ( vid & 0x7fffffff )
#define NOUT_VID(vid) ( vid | 0x80000000 )

// const char DBPath[] = "./db/";
// const char dataPath[] = "../CollegeMsg.txt"; 

#define MAX_NBR     4096
#define MAX_INT     2147483647
#define NULL_VID    0

/**
 * @description: main
 * @param <int> argc
 * @param <char*> argv [ dataPath ]
 * @return <*>
 */
int main(int argc, char* argv[]) {
    system("rm -rf ../db/*");

    rocksdb_t *db;
    rocksdb_options_t *options = rocksdb_options_create();
    rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
    rocksdb_readoptions_t *readoptions = rocksdb_readoptions_create();
    long cpus = sysconf(_SC_NPROCESSORS_ONLN);
    rocksdb_options_increase_parallelism(options, (int)(cpus));
    rocksdb_options_optimize_level_style_compaction(options, 0);
    rocksdb_options_set_create_if_missing(options, 1);

    // open DB
    char *err = NULL;
    db = rocksdb_open(options, "../db", &err);
    assert(!err);

    // make graph
    ifstream fin(argv[1]);
    map<int, vector<long> >* g = 
        new map<int, vector<long> >;
    int u, v, t;
    long val;
    size_t len;
    char key[sizeof(int)];
    char* value = new char[MAX_NBR * sizeof(long)];
    int minid = MAX_INT;
    int maxid = -MAX_INT;
    int mintime = MAX_INT;
    int maxtime = -MAX_INT;
    bool end = false;

    while ( true ) {
        while (g->size() < 2048 && fin >> u >> v >> t) {
            if (u==0 and v==0 and t==0) { end=true; break; }
            minid = minid < u ? minid : u;
            minid = minid < v ? minid : v;
            maxid = maxid > u ? maxid : u;
            maxid = maxid > v ? maxid : v;
            mintime = mintime < t ? mintime : t;
            maxtime = maxtime > t ? maxtime : t;
            val = v; val <<= 32; val |= t;
            (*g)[NOUT_VID(u)].push_back(val);
            val = u; val <<= 32; val |= t;
            (*g)[NIN_VID(v)].push_back(val);
        }
        for (auto it: *g) {
            *(int*)key = it.first;
            err = NULL;
            char * buff = rocksdb_get(db, readoptions, key, sizeof(int), &len, &err);
            assert( !err );
            memcpy(value, buff, len);
            long *des = (long*)(value+len);
            for (int i = 0; i < it.second.size(); i++) {
                des[i] = it.second[i];
            }
            rocksdb_put(db, writeoptions, key, sizeof(int), value, len+it.second.size()*sizeof(long), &err);
            // pay attention to the type of your machine (LE, HE)
        }
        g->clear();
        if (end) break;
    }

    cout << "minid: " << minid << endl;
    cout << "maxid: " << maxid << endl;
    cout << "mintime: " << mintime << endl;
    cout << "maxtime: " << maxtime << endl;
    printf("cond = %d %d %d %d\n", minid, maxid, mintime, maxtime);

    // cleanup
    rocksdb_writeoptions_destroy(writeoptions);
    rocksdb_readoptions_destroy(readoptions);
    rocksdb_options_destroy(options);
    rocksdb_close(db);
    return 0;
}
