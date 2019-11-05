#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>

#define up 0
#define down 1
#define left 2
#define right 3
using namespace std;

class graph;

enum {white, grey, black};

class node {
friend graph;
private:
    int type; // 1 or 0 or R
    int position; // the position of node
    int neighbor[4]; // from 0 to 3 are up down left right
    bool Is_clean; // 1 is clean, 0 is dirty
};

class graph{
private:
    int num_node; // total number of 0 node in the graph
    int col, row; // the col, row of the graph
    int R_position; // the position of the recharge station
    int battery; // the volume of battery
    node* array; // the two-dimensional array to represent the graph
    int *predecessor;
    int *distance;
public:
    graph(void); // constructor
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void print_BFS(void);
    void print_neighbor(void);
};


graph:: graph(void) 
{
    //fstream fin;

    //fin.open("floor.data", ios::in);
    cin >> row >> col >> battery;

    num_node = col * row;
    array = new node[num_node];
    predecessor = new int[num_node];
    distance = new int[num_node];


    for (int i = 0; i < num_node; i++) {
        cin >> array[i].type;
        if (array[i].type == 3) {
            R_position = i;
        }
        array[i].position = i;
        array[i].Is_clean = 0;
        array[i].neighbor[up] = -1;
        array[i].neighbor[down] = -1;
        array[i].neighbor[left] = -1;
        array[i].neighbor[right] = -1;
    }
    //fin.close();
    // set the neighbor data
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != 1) {
            if (i / col == 0) // R is at the top of the graph
                array[i].neighbor[down] = i + col;
            else if (i / col == row - 1) // R is at the bottom of the graph
                array[i].neighbor[up] = i - col;
            else if (i % col == 0) // R is at the left wall
                array[i].neighbor[right] = i - 1;
            else if (i % col == col - 1) // R is at the right wall
                array[i].neighbor[left] = i + 1;
            else {
                if (array[i - col].type != 1) 
                    array[i].neighbor[up] = i - col;
                if (array[i + col].type != 1)
                    array[i].neighbor[down] = i + col;
                if (array[i - 1].type != 1)
                    array[i].neighbor[left] = i - 1;
                if (array[i + 1].type != 1)
                    array[i].neighbor[right] = i + 1;
            }
        }
    }
  
    set_BFS(); // run the BSF on the graph
}

void graph::set_BFS(void)
{
    int *colour = new int[num_node];
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != 1)
            colour[i] = white;
        else
            colour[i] = black;
        predecessor[i] = -1;
        distance[i] = col + row - 2;
    }
    
    queue<int> q;
    int i = R_position;
    for (int j = 0; j < num_node; j++) {
        if (colour[i] == white) {
            colour[i] = grey;
            distance[i] = 0;
            predecessor[i] = -1;
            q.push(i);
            while (!q.empty()) {
                int u = q.front();
                for (int k = 0; k < 4; k++) {
                    int current_pos = array[u].neighbor[k];
                    if (colour[current_pos] == white) {
                        colour[current_pos] = grey;
                        distance[current_pos] = distance[u] + 1;
                        predecessor[current_pos] = u;
                        q.push(current_pos);
                    }
                }
                q.pop();
                colour[u] = black;
            }
        }
        i = j;
    }
    delete [] colour;
}

void graph:: print_BFS(void) 
{
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != 1) {
            cout << i << "th vertex has distance "<<distance[i];
            cout << ", and it's predecessor is " << predecessor[i] << endl;
        }
    }
}

void graph:: print_neighbor(void)
{
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != 1)
            cout << i << endl;
        for (int j = 0; j < 4; j++)
            if (array[i].type != 1) {
             cout << array[i].neighbor[j] << ' ';
            }
        if (array[i].type != 1)  cout << endl;
    }
}
int main(void)
{
    graph mygraph;
    mygraph.print_BFS();
    mygraph.print_neighbor();
    return 0;
}