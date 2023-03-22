/*
 * @author: 
 * @Description: 
 * @Date: 2023-03-13 20:34:54
 */
#include "graph.h"
#include "order.h"
using namespace std;

int main(int argc, char * argv[]) {
    Graph * g = new Graph(argv[1]);
    Order * o = new Order(argv[2]);
    o->write_order_file(g);
    return 0;
}