#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>
#include <stack>

#define up 0
#define down 1
#define left 2
#define right 3
using namespace std;

class graph;
enum {white, grey, black};
namespace mynamespace {
class pair {
    public:
        int x; // index
        int y; // distance
        pair(int a = 0, int b = 0): x(a), y(b) {}
        bool operator>(const pair &c) {
        return (y > c.y); 
        }
        bool operator<(const pair &c) {
            return (y < c.y);
        }
        bool operator>=(const pair &c) {
            return (y >= c.y);
        }
        bool operator<=(const pair &c) {
            return (y <= c.y);
        }
    };
}
class max_heap {
public:
    max_heap(void) {
        heap = NULL;
        heapsize = 0;
        capacity = 0;
    }
    bool IsEmpty(void) {
        return (heapsize == 0);
    }
    int top(void) {
        return heap[1].x;
    }
    void push(const mynamespace::pair&);
    void pop(void);
    void set_heap(int distance[], int);
private:
    mynamespace::pair* heap;
    int heapsize;
    int capacity;
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
    int battery; // the volume of battery
    node* array; // the two-dimensional array to represent the graph
    int *predecessor; // the predecessor array as a result of BFS
    int *distance; // the closest distance from any node to the R
    max_heap graph_heap;
public:
    graph(void); // constructor
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void print_BFS(void);
    void print_neighbor(void);
    int getFurthest(void);
    void clean_graph_heap(void);
};

graph:: graph(void)
{
    fstream fin;
    char **temp, *temp1;
    
    fin.open("floor.data", ios::in);
    fin >> row >> col >> battery;
    
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
    graph_heap.set_heap(distance, num_node);
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
     
    queue<int> q;
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
            cout << i << "th vertex has distance "<< distance[i];
            cout << ", and it's predecessor is " << predecessor[i] << endl;
        }
    }
}

void graph:: print_neighbor(void)
{
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1')
            cout << i << endl;
        for (int j = 0; j < 4; j++)
            if (array[i].type != '1') {
             cout << array[i].neighbor[j] << ' ';
            }
        if (array[i].type != '1')  cout << endl;
    }
}

int graph::getFurthest(void)
{
    return graph_heap.top();
}

void max_heap:: set_heap(int distance[], int num_node) {
    heap = new mynamespace::pair[num_node + 1];
    for (int i = 0; i < num_node; i++) {
    	mynamespace::pair temp = mynamespace::pair(i, distance[i]);
		push(temp); // push the distance of i and i
	}
        
}

void max_heap::push(const mynamespace::pair& e)
{
    int currentNode = ++heapsize;
    while (currentNode != 1 && heap[currentNode / 2] < e) {
        heap[currentNode] = heap[currentNode / 2];
        currentNode /= 2;
    }
    heap[currentNode] = e;
}

void max_heap:: pop(void)
{
    if (IsEmpty()) return;
    // rempde the last element form heap
    mynamespace::pair lastE = heap[heapsize--];

    // tickle down
    int currentNode = 1; // root
    int child = 2; // a chhild of currentNode
    while (child < heapsize) {
        // set child to larger child of currentNode
        if (child < heapsize && heap[child] < heap[child + 1]) child++;
        // can we put last in currentNode?
        if (lastE >= heap[child]) break; // Yes!!

        // No!
        heap[currentNode] = heap[child]; // move child up
        currentNode = child; child *= 2; // move down a level
    }
    heap[currentNode] = lastE;
}

void graph:: clean_graph_heap(void)
{
    while (array[graph_heap.top()].Is_clean == 0)
        graph_heap.pop();
}
void graph:: clean(void)
{
    int target;
    int parent;
    stack<int> s;

    array[R_position].Is_clean = 1;

    while (!graph_heap.IsEmpty()) {
        target = getFurthest(); // get the index of the furthest node
        for (int i = 0, parent = target; i < distance[target]; i++) {
            s.push(parent);
            array[parent].Is_clean = 1;
            parent = predecessor[parent];
        }
        cout << R_position << endl;
        while (!s.empty()) {
            cout << s.top();
            s.pop();
        }
        clean_graph_heap();   
    }
}
int main(void)
{
    graph mygraph;
    //mygraph.print_BFS();
    //mygraph.print_neighbor();
    mygraph.clean();
    return 0;
}