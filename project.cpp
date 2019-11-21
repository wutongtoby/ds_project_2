#include <iostream>
#include <fstream>
#include <time.h>
#include "stack.h"
#include "queue.h"

const char XD[100] = "floor.data";
#define up 0
#define down 1
#define left 2
#define right 3
#define white 0
#define grey 1
#define black 2

using namespace std;

fstream fout;

class graph;

class Pair {
    public:
        int x; // index
        int y; // distance
        Pair(int a = 0, int b = 0): x(a), y(b) {}
        bool operator>(const Pair &c) {
        return (y > c.y); 
        }
        bool operator<(const Pair &c) {
            return (y < c.y);
        }
        bool operator>=(const Pair &c) {
            return (y >= c.y);
        }
        bool operator<=(const Pair &c) {
            return (y <= c.y);
        }
    };


class node {
friend graph;
private:
    char type; // 1 or 0 or R
    int position; // the position of node
    int neighbor[4]; // from 0 to 3 are up down left right
    bool Is_clean; // 1 is clean, 0 is dirty
};

class graph {
private:
    int num_node; // total number of 0 node in the graph
    int col, row; // the col, row of the graph
    int R_position; // the position of the recharge station
    int total_energy; // the volume of battery
    node* array; // the two-dimensional array to represent the graph
    int *predecessor; // the predecessor array as a result of BFS
    int *distance; // the closest distance from any node to the R
    Pair* heap;
    int heapsize;
    int capacity;
    int total_step;
public:
    graph(void); // constructor
    ~graph(void);
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void heap_clean(void);
    bool heap_IsEmpty(void) {
        return (heapsize == 0);
    }
    int heap_top(void) {
        return heap[1].x;
    }
    void heap_push(const Pair&);
    void heap_pop(void);
    void heap_set(void);
    int furthest_neighbor(const int&);
};

graph:: ~graph(void)
{
    delete[] array;
    delete[] predecessor;
    delete[] distance;
    delete[] heap;
}

graph:: graph(void): heap(NULL), capacity(0), heapsize(0), total_step(0)
{
    fstream fin;
    char *temp_map;
    
    fin.open(XD, ios::in);
    fin >> row >> col >> total_energy;
    num_node = col * row;
    
    temp_map = new char[num_node + 1];
    
    array = new node[num_node];
    predecessor = new int[num_node];
    distance = new int[num_node];

    for (int i = 0; i < row; i++) {
        fin >> (temp_map + i * col);
    }
    
    for (int i = 0; i < num_node; i++) {
        array[i].type = temp_map[i];
        if (array[i].type == 'R') {
            R_position = i;
        }
        array[i].position = i;
        array[i].Is_clean = 0;
        array[i].neighbor[up] = -1;
        array[i].neighbor[down] = -1;
        array[i].neighbor[left] = -1;
        array[i].neighbor[right] = -1;
    }
    fin.close();

    delete [] temp_map;
    // set the neighbor data
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1') {
            if (i / col == 0) // R is at the top of the graph
                array[i].neighbor[down] = i + col;
            else if (i / col == row - 1) // R is at the bottom of the graph
                array[i].neighbor[up] = i - col;
            else if (i % col == 0) // R is at the left wall
                array[i].neighbor[right] = i + 1;
            else if (i % col == col - 1) // R is at the right wall
                array[i].neighbor[left] = i - 1;
            else {
                if (array[i - col].type != '1') 
                    array[i].neighbor[up] = i - col;
                if (array[i + col].type != '1')
                    array[i].neighbor[down] = i + col;
                if (array[i - 1].type != '1')
                    array[i].neighbor[left] = i - 1;
                if (array[i + 1].type != '1')
                    array[i].neighbor[right] = i + 1;
            }
        }
    }
   
    set_BFS();
    heap_set();
}

void graph::set_BFS(void)
{
    int *colour = new int[num_node];
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1')
            colour[i] = white;
        else
            colour[i] = black;
        predecessor[i] = -1;
        distance[i] = col + row - 2;
    }
     
    queue q;
    int i = R_position;
 
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
    delete [] colour;
}

void graph::heap_set(void) {
    heap = new Pair[num_node + 1];

    for (int i = 0; i < num_node; i++) 
        if (array[i].type != '1')
		    heap_push(Pair(i, distance[i])); // push the distance of i and i   
}

void graph::heap_push(const Pair& e)
{
    int currentNode = ++heapsize;
    while (currentNode != 1 && heap[currentNode / 2] < e) {
        heap[currentNode] = heap[currentNode / 2];
        currentNode /= 2;
    }
    heap[currentNode] = e;
}

void graph:: heap_pop(void)
{
    if (heap_IsEmpty()) return;
    heap[1].y = -1;
    // remove the last element from heap
    Pair lastE = heap[heapsize--];

    // tickle down
    int currentNode = 1; // root
    int child = 2; // a child of currentNode
    while (child <= heapsize) {
        // set child to smaller child of currentNode
        if (child < heapsize && heap[child] < heap[child + 1]) child++;
        // can we put last in currentNode?
        if (lastE >= heap[child]) break; // Yes!!

        // No!
        heap[currentNode] = heap[child]; // move child up
        currentNode = child; child *= 2; // move down a level
    }
    heap[currentNode] = lastE;
}

void graph::heap_clean(void)
{
    while (array[heap_top()].Is_clean && !heap_IsEmpty())
        heap_pop();
}

int graph::furthest_neighbor(const int &x) 
{
    int furthest = -1;
    int furthest_distance = 0;

    // find the most dirty and furthest first, if can't find any, then go to the furthest
    for (int i = 0; i < 4; i++) {
        if (array[x].neighbor[i] == R_position)
            return R_position;
        if (array[x].neighbor[i] != -1 &&
            array[array[x].neighbor[i]].Is_clean == 0 && 
            furthest_distance < distance[array[x].neighbor[i]]) {
            furthest = array[x].neighbor[i];
            furthest_distance = distance[furthest];
        } 
    }
    if (furthest == -1)  // the neighbors are either visted or wall
        return predecessor[x];                
    return furthest;
}

void graph:: clean(void)
{
    int i, j, k;
    Stack s;
    int energy;

    array[R_position].Is_clean = 1;
    fout << "                                                           " << '\n';
    fout << R_position / col << ' ' << R_position % col << '\n';
    
    while (!heap_IsEmpty()) {
        energy = total_energy; // charge
        j = heap_top(); // get the index of the furthest node    

        for (i = 0, k = j; i < distance[j]; i++) {
            s.push(k);
            array[k].Is_clean = 1;
            k = predecessor[k];
        }
        total_step += distance[j];
        energy -= distance[j];

        while (!s.IsEmpty()) {
            fout << s.front() / col<<' ' <<s.front() % col <<'\n';
            s.pop();
        }
        
        while (energy > distance[furthest_neighbor(j)] && !heap_IsEmpty()) {
            heap_clean();
            j = furthest_neighbor(j);
            if (j == R_position) {
                fout << R_position / col <<' ' <<R_position % col << '\n';
                ++total_step;
                break;
            } // occasioncally go to home
            fout << j / col <<' '<< j % col << '\n';
            --energy;
            ++total_step;
            array[j].Is_clean = 1;
        }
        for (int i = 0, k = predecessor[j]; i < distance[j]; i++) {
            array[k].Is_clean = 1;
            fout << k / col<< ' ' << k % col << '\n';
            k = predecessor[k];
        }
        energy -= distance[j];
        total_step += distance[j];
        heap_clean();
    }
    fout.seekp(0L, ios::beg);
    fout << total_step;
}

int main(void)
{
    graph mygraph;
    fout.open("final.path", ios::out);
    
    mygraph.clean();
    fout.close();

    return 0;
}
