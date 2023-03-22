/*
 * @author: yangming
 * @Description: making testcase
 * @Date: 2023-01-15 19:54:48
 */
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {

    int minid   = atoi(argv[1]);
    int maxid   = atoi(argv[2]);
    int mintime = atoi(argv[3]);
    int maxtime = atoi(argv[4]);
    int n       = atoi(argv[5]);
    int u, v ;
    ofstream fout("testcase.txt");

    // make n testcases
    // randomValue = (rand() % (max - min + 1)) + min; // range [min,max]
    for (int i = 0; i < n; i++) {
        u = ( rand() % (maxid-minid+1) ) + minid ;
        v = ( rand() % (maxid-minid+1) ) + minid ;
        fout << u << ' ' << v << ' ' << mintime << ' ' << maxtime << endl;
    }

}