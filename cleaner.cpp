#include <iostream>

using namespace std;

class graph;
class BFS;

class node {
friend graph;
private:
    int type; // 1 or 0 or R
    int neighbor_row[4]; // neighbor[0~3] up down left right
    int neighbor_col[4]; // same as above
    int travel_yet;
};

class graph{
friend BFS;
private:
    int num_node; // total number of 0 node in the graph
    node** node_array; // the two-dimensional array to represent the graph
    int col, row; // the col, row of the graph
    int R_row, R_col; // the position of the recharge station
    int battery; // the volume of battery
    BFS bfs;
public:
    graph(void); // constructor
    void clean(void); // to print the path of clean
};

class BFS {
friend graph;
private:

}
int main(void)
{
    return 0;
}