#include <iostream>
#include <fstream>

using namespace std;

class graph;
class BFS;

enum {up = 0, down = 1, left = 2, right = 3};

class node {
friend graph;
private:
    int type; // 1 or 0 or R
    int position; // the position of node
    bool neighbor[4]; // from 0 to 3 are up down left right
    bool Is_clean; // 1 is clean, 0 is dirty
};

class graph{
friend BFS;
private:
    int num_node; // total number of 0 node in the graph
    int col, row; // the col, row of the graph
    int R_position; // the position of the recharge station
    int battery; // the volume of battery
    node* array; // the two-dimensional array to represent the graph
    BFS bsf; // the bsf tree of the graph
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
    void set_BSF(int);
};

graph:: graph(void) 
{
    fstream fin;

    fin.open("floor.data", ios::in);
    fin >> col >> row >> battery;
    num_node = col * row;
    array = new node[num_node];

    for (int i = 0; i < num_node; i++) {
        fin >> array[i].type;
        if (array[i].type == 'R')
            R_position = i;
        array[i].position = i;
        array[i].Is_clean = 0;
        array[i].neighbor[up] = 0;
        array[i].neighbor[down] = 0;
        array[i].neighbor[left] = 0;
        array[i].neighbor[right] = 0;
    }
    fin.close();

    // set the neighbor data
    for (int i = 0; i < num_node; i++) {
        if (array[i].typea != 1) {
            if (i / col == 0) // R is at the top of the graph
                array[i].neighbor[down] = 1;
            else if (i / col == row - 1) // R is at the bottom of the graph
                array[i].neighbor[up] = 1;
            else if (i % col == 0) // R is at the left wall
                array[i].neighbor[right] = 1;
            else if (i % col == col - 1) // R is at the right wall
                array[i].neighbor[left] = 1;
            else {
                if (array[i - col].type != 1) 
                    array[i].neighbor[up] = 1;
                if (node_aray[i + col].type != 1)
                    array[i].neighbor[down] = 1;
                if (array[i - 1].type != 1)
                    array[i].neighbor[left] = 1;
                if (array[i + 1].type != 1)
                    array[i].neighbor[right] = 1;
            }
        }
    }
    bsf.set_BSF(); // run the BSF on the graph
}
int main(void)
{
    graph mygraph;
    return 0;
}