#include <iostream>
#include <fstream>
#include <time.h>
#include "stack.h"
#include "queue.h"


#define up 0
#define down 1
#define left 2
#define right 3
using namespace std;

fstream fout;

class graph;
enum {white, grey, black};


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
    int repeat;
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
    int step;
public:
    graph(void); // constructor
    ~graph(void);
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void print_BFS(void);
    void print_neighbor(void);
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
    void heap_print(void) {
        for (int i = 1; i <= heapsize; i++) {
            fout <<"index is  " << heap[i].x << " distance is " << heap[i].y << endl;
        }
    }
    int furthest_neighbor(const int&);
    void check_clean(void);
};

graph:: ~graph(void)
{
    delete[] array;
    delete[] predecessor;
    delete[] distance;
    delete[] heap;
}

graph:: graph(void)
{
    fstream fin;
    char **temp, *temp1;
    
    fin.open("thousand.data", ios::in);
    fin >> row >> col >> total_energy;
    
    num_node = col * row;
    
    temp1 = new char[num_node + 1];
    temp = new char*[row];
    for (int i = 0; i < row; i++) {
        temp[i] = temp1 + i * col;
        fin >> temp[i];
    }
    
    array = new node[num_node];
    predecessor = new int[num_node];
    distance = new int[num_node];

    for (int i = 0; i < num_node; i++) {
        array[i].type = temp[i / col][i % col];
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
    delete [] temp1;
    delete [] temp;
    
    // set the neighbor data
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1') {
            if (i / col == 0) // R is at the top of the graph
                array[i].neighbor[down] = i + col;
            else if (i / col == row - 1) // R is at the bottom of the graph
                array[i].neighbor[up] = i - col;
            else if (i % col == 0) // R is at the left wall
                array[i].neighbor[right] = i - 1;
            else if (i % col == col - 1) // R is at the right wall
                array[i].neighbor[left] = i + 1;
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

    heap = NULL;
    capacity = 0;
    heapsize = 0;
    step = 0;
    repeat = 0;
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

void graph:: print_BFS(void) 
{
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1') {
            fout << i << "th vertex has distance "<< distance[i];
            fout << ", and it's predecessor is " << predecessor[i] << endl;
        }
    }
}

void graph:: print_neighbor(void)
{
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1')
            fout << i << endl;
        for (int j = 0; j < 4; j++)
            if (array[i].type != '1') {
             fout << array[i].neighbor[j] << ' ';
            }
        if (array[i].type != '1')  fout << endl;
    }
}


void graph::heap_set(void) {
    heap = new Pair[num_node + 1];
    Pair temp;

    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1') {
    	    temp = Pair(i, distance[i]);
		    heap_push(temp); // push the distance of i and i
        }
    }   
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
    heap[1].y = 0;
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
    while (array[heap_top()].Is_clean && !heap_IsEmpty()) {
        heap_pop();
    }
}

int graph::furthest_neighbor(const int &x) 
{
    int furthest = -1;
    int furthest_distance = 0;

    // find the most dirty and furthest first, if can't find any, then go to the furthest
    for (int i = 0; i < 4; i++) {
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
    int target;
    int parent;
    Stack s;
    int energy;
    int foo, more;
    array[R_position].Is_clean = 1;
    
    while (!heap_IsEmpty()) {
        energy = total_energy; // charge
        heap_clean();
        target = heap_top(); // get the index of the furthest node
        fout << R_position / col <<' ' <<R_position % col << '\n';
        for (int i = 0, parent = target; i < distance[target]; i++) {
            s.push(parent);
            if (array[parent].Is_clean == 1)
                ++repeat;
            array[parent].Is_clean = 1;
            parent = predecessor[parent];
        }
        step += distance[target];
        energy -= distance[target];

        while (!s.IsEmpty()) {
            fout << s.front() / col<<' ' <<s.front() % col <<'\n';
            s.pop();
        }
        
        more = foo = 0;
        while (energy > distance[target] && !heap_IsEmpty()) {
            heap_clean();
            target = furthest_neighbor(target);
        
            if (target == R_position) break; // occasioncally go to home
            fout << target / col <<' '<< target % col <<'\n';
            --energy;
            ++step;
            ++more;
            if (array[target].Is_clean == 1) {
                ++repeat;
                ++foo;
            }
            array[target].Is_clean = 1;

            // avoid to make circle
            if (more > num_node / 16  && (float)foo / more > 0.6)
                break;
        }

        for (int i = 0, parent = predecessor[target]; i < distance[target]; i++) {
            if (array[parent].Is_clean == 1)
                ++repeat;
            array[parent].Is_clean = 1;
            fout << parent / col<< ' ' << parent % col << '\n';
            parent = predecessor[parent];
            --energy;
            ++step;
        }
    }
    fout << "number of step is " << step << '\n';
    fout << "number of repeated step is " << repeat << '\n';
    fout << "And the percentage is " << (float) repeat / step << '\n';
}

void graph::check_clean(void)
{
    for (int i = 0 ; i < num_node; i++)
        if (array[i].Is_clean == 0 && array[i].type != '1') {
            fout << i / col << ' ' << i % col << '\n';
        }
    fout << "Success" << endl;
}

int main(void)
{
    clock_t begin = clock();
    graph mygraph;

    fout.open("output.path", ios::out);
    //mygraph.heap_print();
    //fout << endl;
    //mygraph.print_BFS();
 
    //mygraph.print_neighbor();
    
    mygraph.clean();
    mygraph.check_clean();
    fout.close();
    cout << (long double)(clock() - begin) / CLOCKS_PER_SEC << endl;

    return 0;
}