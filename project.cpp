#include <iostream>
#include <fstream>
#include <time.h>
#include "stack.h"
#include "queue.h"

const char XD[100] = "D:/code/project_2/floor.data";
#define up 0
#define down 1
#define left 2
#define right 3
#define ISclean '8'
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

graph:: graph(void)
{
    fstream fin;
    char **temp, *temp1;
    
    fin.open(XD, ios::in);
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
    heap = NULL;
    capacity = 0;
    heapsize = 0;
    total_step = 0;
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
        else if (array[x].neighbor[i] == R_position)
            return R_position;
    }
    if (furthest == -1)  // the neighbors are either visted or wall
        return predecessor[x];                
    return furthest;
}

void graph:: clean(void)
{
    int j, k;
    Stack s;
    int energy;
    int foo, more;
    array[R_position].Is_clean = 1;
    fout << "                                                           " << '\n';
    fout << R_position / col <<' ' <<R_position % col << '\n';
    
    while (!heap_IsEmpty()) {
        energy = total_energy; // charge
        heap_clean();
        j = heap_top(); // get the index of the furthest node    

        for (int i = 0, k = j; i < distance[j]; i++) {
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
        
        more = foo = 0;
        while (energy > distance[furthest_neighbor(j)] && !heap_IsEmpty()) {
            heap_clean();
            j = furthest_neighbor(j);
            if (j == R_position) {
                fout << R_position / col <<' ' <<R_position % col << '\n';
                ++total_step;
                break;
            } // occasioncally go to home
            fout << j / col <<' '<< j % col <<'\n';
            --energy;
            ++total_step;
            ++more;
            if (array[j].Is_clean == 1) {
                ++foo;
            }
            array[j].Is_clean = 1;

            // avoid to make circle
            if (more > num_node / 16  && (float)foo / more > 0.6)
                break;
        }
        for (int i = 0, k = predecessor[j]; i < distance[j]; i++) {
            array[k].Is_clean = 1;
            fout << k / col<< ' ' << k % col << '\n';
            --energy ;
            k = predecessor[k];
            ++total_step;
        }
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
///////////////////////////////////////////////////////
    int num_step;
    int row, col, total_energy;
    int now_energy;
    int now_position_row, now_position_col;
    int next_position_row, next_position_col;
    int R_col, R_row;
    char *temp1;
    char **temp;

    fstream path, map;
    path.open("final.path", ios::in);
    map.open(XD, ios::in);
    
    map >> row >> col >> total_energy;
    
    temp1 = new char[row * col];
    temp = new char*[row];
    
    for (int i = 0; i < row; i++) {
        temp[i] = temp1 + i * col;
        map >> temp[i];
    }
    map.close();

    path >> num_step;
    path >> now_position_row >> now_position_col;
    if (temp[now_position_row][now_position_col] != 'R') {
        cout << "fail since the R_position is wrong" << endl;
        return -1;
    }
    R_row = now_position_row, R_col = now_position_col; 
    //cout << "R_position " << now_position_row << ' ' << now_position_col << endl;

    for (int i = 0, now_energy = total_energy; i < num_step; i++) {
        if (now_position_row == R_row && now_position_col == R_col)
            now_energy = total_energy;
        path >> next_position_row >> next_position_col;
        if ((next_position_col - now_position_col) * (next_position_col - now_position_col) +
            (next_position_row - now_position_row) * (next_position_row - now_position_row) != 1) {
            cout << "fail since jumping from some step" << endl;
            cout << now_position_row << ' '<<now_position_col <<endl;
            cout << i << endl;
            return -1;
        }
        if (temp[next_position_row][next_position_col] == '1') {
            cout << "fail since walk into wall" << endl;
            return -1;       
        }
        now_energy--;
        if (now_energy < 0) {
            cout << "fail since energy is used out" << endl;
            cout << next_position_row << ' ' << next_position_col << endl;
            cout << i << endl;
            cout << now_energy << endl;
            return -1;
        }
        now_position_row = next_position_row;
        now_position_col = next_position_col;
        temp[now_position_row][now_position_col] = ISclean;
    }
    //path.close();
    if (now_position_row == R_row && now_position_col != R_col) {
        cout << "did'nt go home" << endl;
        cout << now_position_row << ' ' << now_position_col << endl;
    }
    for (int i = 0; i < row * col; i++) {
        if (temp[i / col][i % col] == '0') {
            cout << "not all clean" << endl;
            return -1;
        }
    }
    cout << "success" << endl; 
    //delete[] temp1;
    //delete[] temp;  
    
    return 0;
}