#include <iostream>

using namespace std;

class graph;
class BFS;

class node {
friend graph;
private:
    int type; // 1 or 0 or R
    int position;
    bool neighbor[4];
    bool Is_clean; // 1 is clean, 0 is dirty
};

class graph{
friend BFS;
private:
    int num_node; // total number of 0 node in the graph
    int col, row; // the col, row of the graph
    int R_position; // the position of the recharge station
    int battery; // the volume of battery
    node* node_array; // the two-dimensional array to represent the graph
public:
    graph(void); // constructor
    void clean(void); // to print the path of clean
};

class BFS {
friend graph;
private:
    int *predecessor;
    int *distance;
public:
    BST(int);
}
int main(void)
{
    return 0;
}