struct list_node{
    int data;
    list_node *next;
    list_node(void):data(0),next(0){};
    list_node(int x):data(x),next(0){};
};

class queue{
private:
    list_node *first;
    list_node *last;
    int size;
public:
    queue(void):first(0),last(0),size(0){};
    void push(int x);
    void pop(void);
    bool empty(void);
    int front(void);
};

void queue::push(int x){
    if (empty()) {
        first = new list_node(x);
        last = first;
        size++;
        return;
    }
    list_node *newNode = new list_node(x);
    last->next = newNode;
    last = newNode;         // update last pointer
    size++;
}

void queue::pop(){
    if (empty()) {
        return;
    }
    list_node *deletenode = first;
    first = first->next;    // update first pointer
    delete deletenode;
    deletenode = 0;
    size--;
}

int queue::front(){
    if (empty()) {
        return -1;
    }
    return first->data;
}

bool queue::empty(){
    return ((first && last) == 0);
}