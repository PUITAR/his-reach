/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-20 12:43:12
 */
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]) {
    ofstream fout("testcase.txt");
    int minid = atoi(argv[1]);
    int maxid = atoi(argv[2]);
    int mintime = atoi(argv[3]);
    int maxtime = atoi(argv[4]);
    int n = atoi(argv[5]);
    int theta = atoi(argv[6]);

    for (int u = minid; u <= maxid and n; u++) {
        for (int v = u+1; v <= maxid and n; v++) {
            fout << u << ' ' << v << ' ' << mintime << ' ' << maxtime << endl;
            fout << v << ' ' << u << ' ' << mintime << ' ' << maxtime << endl;
            n-=2;
        }
    }

    return 0;
}