#ifndef STACK_H
#define STACK_H

// Necessary for print()
#include <iostream>
using namespace std;


// This class implements a simple array-based stack.
// All types that support assignment (=) and optionally
// the stream insertion operator (<<) are supported.
template <class T>
class Stack
{
public:
    // Constructors / Destructors
    Stack();
    Stack(const int size);
    Stack(const Stack<T>& orig);
    ~Stack();
    
    // Modification
    bool push(const T& value);
    bool pop(T& value);
    
    // Status
    void print() const;
    bool top(T& value) const;
    bool isFull() const;
    bool isEmpty() const;
    
    int size() const;
    
private:
    static const int DEFAULT_SIZE = 32;
    
    T* mData;      // Array containing the actual data
    int mCapacity; // The size of the array
    int mTop;      // The index of the top of the stack
};

// To initialize all the variables

template <class T>
Stack<T>::Stack()
{
    mData     = new T[DEFAULT_SIZE];
    mCapacity = DEFAULT_SIZE;
    mTop      = -1;
}

template <class T>
Stack<T>::Stack(const int size)
{
    mData     = new T[size];
    mCapacity = size;
    mTop      = -1;
}

template <class T>
Stack<T>::Stack(const Stack<T>& orig)
{
    // Create an array big enough to hold everything in 'orig'.
    // copies are supposed to be different from each other
    
    mData     = new T[orig.mCapacity];
    mCapacity = orig.mCapacity;
    mTop      = orig.mTop;
    
    // Copy the data over
    for (int i = 0; i < mCapacity; ++i)
        mData[i] = orig.mData[i];
}

// clean up the dynamically allocated array

template <class T>
Stack<T>::~Stack()
{
    // Take care of deallocation of the dynamically allocated array
    delete[] mData;
    mData = NULL;
}

template <class T>
bool Stack<T>::push(const T& value)
{    
    // Ensure we're still in the array
    if (mTop < mCapacity - 1)
    {
        // Move top forward, and fill the cell
        mTop++;
        mData[mTop] = value;
        return true;
    }
    
    // The stack must be full. Sorry. :-(
    else return false;
}

template <class T>
bool Stack<T>::pop(T& value)
{
    // Let top() do the hard part. If it returns false,
    // the stack was empty. If it returns true, we have
    // the top value stored already, so we just need to
    // decrement 'top' to shrink the stack.
    
    if (top(value))
    {
        mTop--;
        return true;
    }
    else return false;
}

template <class T>
void Stack<T>::print() const
{
    // Remember, 'mTop' is the index of the top of the stack.
    for (int i = 0; i <= mTop; ++i)
    {
        cout << mData[i] << " ";
    }
    cout << endl;
}

template <class T>
bool Stack<T>::top(T& value) const
{    
    // Make sure the stack has content first.
    if (mTop >= 0)
    {
        value = mData[mTop];
        return true;
    }
    
    // You should have a return false not just a return true.
    // The stack was empty, so there is no top.
    else return false;
}

template <class T>
bool Stack<T>::isFull() const
{
    return (mTop >= mCapacity - 1);
}

template <class T>
bool Stack<T>::isEmpty() const
{
    return (mTop < 0);
}

template <typename T>
int Stack<T>::size() const
{
    return mTop + 1;
}

#endif