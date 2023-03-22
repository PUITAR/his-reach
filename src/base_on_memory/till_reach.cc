/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-14 20:40:31
 */
#include "till.h"
#include "timer.h"
using namespace std;

int main(int argc, char * argv[]) {
    int maxid = atoi(argv[2]);
    Till * till = new Till(maxid+5, argv[5], argv[6], argv[7], argv[8], argv[9]);
    ifstream fin("testcase.txt");
    ofstream fout("till_reach_res.txt");
    Timer timer;
    int u, v, ts, te;
    cout << "querying...";
    timer.StartTimer();
    while (fin >> u >> v >> ts >> te) 
        fout << till->till_reach(u, v, ts, te) << endl;
    timer.StopTimer();
    fout << timer.GetTimerCount() << " ns" << endl;
    cout << "[fin]" << endl;
    return 0;
}