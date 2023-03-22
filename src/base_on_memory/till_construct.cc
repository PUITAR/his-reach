/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-13 23:07:49
 */
#include "till.h"
#include "timer.h"

#include <stdlib.h>
using namespace std;

int main(int argc, char * argv[]) {
    int maxid = atoi(argv[2]);
    int theta = atoi(argv[7]);
    Till * till = new Till(maxid+5, argv[5]);
    Timer timer;
    timer.StartTimer();
    till->till_construct(argv[6], theta);
    timer.StopTimer();
    cout << timer.GetTimerCount() << " ns" << endl;
    till->write_till_file(argv[8], argv[9], argv[10], argv[11]);

    // int maxid = atoi("1899");
    // int theta = atoi("2147483000");
    // Till * till = new Till(maxid+5, "ord_files/collegemsg.txt");

    // till->till_construct("data/collegemsg.txt", theta);
    // timer.StopTimer();
    // cout << timer.GetTimerCount() << " ns" << endl;
    // till->write_till_file("till/vset_in/collegemsg.txt", "till/vset_out/collegemsg.txt", "till/span_in/collegemsg.txt", "till/span_out/collegemsg.txt");

    return 0;
}