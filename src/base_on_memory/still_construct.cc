/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-17 15:46:59
 */
#include "still.h"
#include "timer.h"

#include <stdlib.h>
using namespace std;

int main(int argc, char * argv[]) {
    // int maxid = atoi(argv[2]);
    // int theta = atoi(argv[7]);
    // Still * still = new Still(maxid + 5);
    // Timer timer;
    // timer.StartTimer();
    // still->still_construct(argv[5], argv[6], theta);
    // timer.StopTimer();
    // cout << timer.GetTimerCount() << " ns" << endl;
    // still->write_still_file(argv[8], argv[9], argv[10], argv[11]);
    
    int maxid = atoi("1899");
    int theta = atoi("10000");
    Still * still = new Still(maxid + 5);
    Timer timer;
    timer.StartTimer();
    still->still_construct("ord_files/collegemsg.txt", "data/collegemsg.txt", theta);
    timer.StopTimer();
    cout << timer.GetTimerCount() << " ns" << endl;
    still->write_still_file("still/vset_in/collegemsg.txt", "still/vset_out/collegemsg.txt", "still/span_in/collegemsg.txt", "still/span_out/collegemsg.txt");
    return 0;
}