#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>
#include <stack>
#include <time.h>
#include <algorithm>

#define up 0
#define down 1
#define left 2
#define right 3
using namespace std;

fstream fout;

class list;
class graph;
enum {white, grey, black};

struct Pair {
        int x; // index
        int y; // distance
        Pair(int a = 0, int b = 0): x(a), y(b) {}
};
bool compare(const Pair& a,const Pair& b)  {return (a.y > b.y);}

class node {
friend graph;
friend list;
private:
    char type; // 1 or 0 or R
    int position; // the position of node
    int neighbor[4]; // from 0 to 3 are up down left right
    bool Is_clean; // 1 is clean, 0 is dirty
};

class list {
private:
    Pair* clean_list;
    int length;
    int current;
public:
    ~list(void);
    void set(int, int*);
    bool Isempty(void);
    int top(void);
    void update(node*);
    void print(void) {
        for (int i = 0; i < length; i++)
            cout << "index "<<clean_list[i].x << " distance" << clean_list[i].y << endl;
    }
};

class graph {
friend list;
private:
    int num_node; // total number of 0 node in the graph
    int col, row; // the col, row of the graph
    int R_position; // the position of the recharge station
    int total_energy; // the volume of battery
    node* array; // the two-dimensional array to represent the graph
    int *predecessor; // the predecessor array as a result of BFS
    int *distance; // the closest distance from any node to the R
    list mylist;
public:
    graph(void); // constructor
    ~graph(void);
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void print_BFS(void);
    void print_neighbor(void);
    int furthest_neighbor(const int&);
    void check_clean(void);
    void set_list(void);
};

void graph:: clean(void)
{
    int target;
    int parent;
    int step = 0, repeat = 0;
    stack<int> s;
    int energy;

    array[R_position].Is_clean = 1;
    mylist.print();
    
    while (!mylist.Isempty()) {
        energy = total_energy; // charge
        mylist.update(array);
        target = mylist.top(); // get the index of the furthest node
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

        while (!s.empty()) {
            fout << s.top() / col<<' ' <<s.top() % col <<'\n';
            s.pop();
        }
        
        mylist.update(array);
        while (energy > distance[target] && !mylist.Isempty()) {
            target = furthest_neighbor(target);
            if (target == R_position) break; // occasioncally go to home
            fout << target / col <<' '<< target % col <<'\n';
            --energy;
            ++step;
            if (array[target].Is_clean == 1)
                ++repeat;
            array[target].Is_clean = 1;
            mylist.update(array);
        }

        for (int i = 0, parent = predecessor[target]; i < distance[target]; i++) {
            if (array[parent].Is_clean == 1)
                ++repeat;
            array[parent].Is_clean = 1;
            fout << parent / col<< ' '<<parent % col << '\n';
            parent = predecessor[parent];
            --energy;
            ++step;
        }
        mylist.update(array);
    }
    fout << "number of step is " << step << '\n';
    fout << "number of repeated step is " << repeat << '\n';
    fout << "And the percentage is " << (float) repeat / step << '\n';
}
list:: ~list(void)
{
    delete [] clean_list;
}
void list:: set(int num_node, int *distance)
{
    clean_list = new Pair[num_node];
    length = num_node;
    current = 0;

    //for (int i = 0; i < length; i++)
      //   cout << i << "QWQ"<< distance[i] << endl;

    for (int i = 0; i < length; i++) {
        if (distance[i] == -1) {
             cout << distance[i] <<"DDDD" <<endl;
            --length;
            --i;
        }
        else {
            clean_list[i] = Pair(i, distance[i]);
            cout << distance[i] << ' '<< i << endl;
        }
    }

    std::sort(clean_list, clean_list + num_node, compare);
}


bool list::Isempty(void)
{
    return (current == length);
}

int list::top(void)
{
    return clean_list[current].x;
}

void list::update(node* array)
{
    while(array[clean_list[current].x].Is_clean == 1 && current < length)
        ++current;
}


graph:: ~graph(void)
{
    delete[] array;
    delete[] predecessor;
    delete[] distance;
}

graph:: graph(void)
{
    fstream fin;
    char **temp, *temp1;
    
    fin.open("floor.data", ios::in);
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
    set_list();
}
void graph::set_list(void)
{
    mylist.set(num_node, distance);
}
void graph::set_BFS(void)
{
    int *colour = new int[num_node];
    
    for (int i = 0; i < num_node; i++) {
        if (array[i].type != '1') {
            colour[i] = white;
            distance[i] = col + row - 2;
        }
        else {
            colour[i] = black;
            distance[i] = -1;
        }
        predecessor[i] = -1;
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

int graph::furthest_neighbor(const int &x) 
{
    int furthest = 0;
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
    if (furthest == 0) {
        for (int i = 0; i < 4; i++) {
            if (array[x].neighbor[i] != -1 &&
                furthest_distance < distance[array[x].neighbor[i]]) {
                furthest = array[x].neighbor[i];
                furthest_distance = distance[furthest];
            }
        }
        return furthest;                
    }
    return furthest;
}



void graph::check_clean(void)
{
    for (int i = 0 ; i < num_node; i++)
        if (array[i].Is_clean == 0 && array[i].type != '1') {
            fout << i / col << ' ' << i % col << '\n';
            return;
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

    //mygraph.check_clean();
    fout.close();
    cout << (long double)(clock() - begin) / CLOCKS_PER_SEC << endl;

    return 0;
}