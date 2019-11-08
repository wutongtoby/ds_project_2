class Stack{
private:
    int top;                
    int capacity;           
    int *stack; 
public:             
    Stack(void):top(-1),capacity(1){   
        stack = new int[capacity];       
    }
    void push(int x);
    void pop(void);
    bool IsEmpty(void);
    int front(void);
    int getSize(void);
};

void Stack::push(int x){
    if (top == capacity - 1) {  
        capacity *= 2;                           
        int *newStack = new int[capacity];       

        for (int i = 0 ; i < capacity/2; i++)
            newStack[i] = stack[i];
        delete [] stack;             
        stack = newStack;
    }
    stack[++top] = x;             
}

void Stack::pop(){
    if (IsEmpty())      
        return;
    top--;
}

bool Stack::IsEmpty(void) {return (top == -1);}

int Stack::front(){
    if (IsEmpty())   
        return -1;
    return stack[top];   // return the top element
}