
class graph;

enum {white, grey, black};

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
    int num_dirty_node; // number of node that have't be cleaned
    min_heap graph_heap;
public:
    graph(void); // constructor
    ~graph(void);
    void clean(void); // to print the path of clean
    void set_BFS(void);
    void print_BFS(void);
    void print_neighbor(void);
    int getFurthest(void);
};

graph:: graph(void)
{
    fstream fin;
    char **temp, *temp1;
    
    fin.open("floor.data", ios::in);
    fin >> row >> col >> battery;
    
    num_dirty_node = num_node = col * row;
    
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
}