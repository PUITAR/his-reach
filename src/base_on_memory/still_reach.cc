/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-14 20:40:31
 */
#include "still.h"
#include "timer.h"
using namespace std;

int main(int argc, char * argv[]) {
    int maxid = atoi(argv[2]);
    Still * still = new Still(maxid+5, argv[5], argv[6], argv[7], argv[8]);
    ifstream fin("testcase.txt");
    ofstream fout("still_reach_res.txt");
    Timer timer;
    int u, v, ts, te;
    cout << "querying...";
    timer.StartTimer();
    while (fin >> u >> v >> ts >> te) 
        fout << still->still_reach(u, v, ts, te) << endl;
    timer.StopTimer();
    fout << timer.GetTimerCount() << " ns" << endl;
    cout << "[fin]" << endl;

    // Still * still = new Still(17, "still/vset_in/tiny.txt", "still/vset_out/tiny.txt", "still/span_in/tiny.txt", "still/span_out/tiny.txt");
    // ifstream fin("testcase.txt");
    // ofstream fout("still_reach_res.txt");
    // Timer timer;
    // int u, v, ts, te;
    // cout << "querying...";
    // timer.StartTimer();
    // while (fin >> u >> v >> ts >> te) 
    //     fout << still->still_reach(u, v, ts, te) << endl;
    // timer.StopTimer();
    // fout << timer.GetTimerCount() << " ns" << endl;
    // cout << "[fin]" << endl;
    return 0;
}