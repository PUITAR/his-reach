/*
 * @author: yangming
 * @Description: till-reach
 * @Date: 2023-01-15 19:38:30
 */
#include "till.h"
#include "../timer.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {

    // initialize
    int minid   = atoi(argv[1]);
    int maxid   = atoi(argv[2]);
    int mintime = atoi(argv[3]);
    int maxtime = atoi(argv[4]);
    TILL * till = new TILL("index", new Order("order.txt", maxid+1), 
                           minid, maxid, mintime, maxtime);
    Timer timer;
    int u, v, t1, t2;

    ifstream cin(argv[5]);
    ofstream cout(argv[6]);

    timer.StartTimer();

    // test
    while ( cin >> u >> v >> t1 >> t2 ) {
        cout << "Result: " 
        << ( till->SpanReach(u, v, t1, t2) == true ? "Ture" : "False" ) << endl;
    }
    timer.StopTimer();
    cout << "Cost: "
        << timer.GetTimerCount() << " ns" << endl;
    return 0;
}