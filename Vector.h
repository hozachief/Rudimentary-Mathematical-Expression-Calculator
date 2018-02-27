#ifndef VECTOR_H
#define VECTOR_H

// Necessary for print()
#include <iostream>
using namespace std;

template <class T>
class Vector
{
public:
    // Constructors / Destructors
    Vector();
    Vector(const int size);
    Vector(const Vector<T>& orig);
    // Prevents memory leak
    ~Vector();
    
    // Adjust size/capacity
    void resize(const int size);
    // my array is at least, larger than fine, but at minimum this size
    void reserve(const int capacity);
    T& operator[](const int index);
    const T& operator[] (const int index) const;
    // makes a copy
    T get(const int index) const;
    void set(const int index, const T& value);
    void pushBack(const T& value);
    // decrement size by one
    void popBack();
    
    // Getters
    int getCapacity() const;
    int getSize() const;
    void print() const;
    
private:
    T* mData;
    // Size of the array
    int mCapacity;
    // Number of elements in the vector
    int mSize;
};

// Implementations

template <class T>
Vector<T>::Vector()
{
    // Start out in a simple default state
    mData     = NULL;
    mCapacity = 0;
    mSize     = 0;
    
    // Use reserve() to do the hard work of actually creating
    // the underlying array.
    reserve(16);
}

template <class T>
Vector<T>::Vector(const int size)
{
    // Start out in a simple default state
    mData     = NULL;
    mCapacity = 0;
    mSize     = 0;
    
    // Let resize() handle the memory management. It will
    // fill all the cells with 0's.
    resize(size);
}

template <class T>
Vector<T>::Vector(const Vector<T>& orig)
{
    // Start out in a simple default state.
    mData     = NULL;
    mCapacity = 0;
    mSize     = 0;
    
    // Let resize() handle the memory management. The array
    // will be initialized with 0's.
    resize(orig.mSize);
    
    // Copy the data from 'orig' into our own array
    for (int i = 0; i < mSize; ++i)
        mData[i] = orig.mData[i];
}

template <class T>
Vector<T>::~Vector()
{
    // We clean up our mess by deallocating the array we're
    // using for storage. It's also a good practice to change
    // the pointer to NULL so there's no chance of trying to
    // access this memory again later.
    delete[] mData;
    mData = NULL;
}

template <class T>
void Vector<T>::resize(const int size)
{
    // NOP
    // Ensure there's enough space for 'size' elements
    reserve(size);
    
    // If we're getting bigger, fill the remaining cells
    // with 0's.
    for (int i = mSize; i < size; ++i)
        mData[i] = T();
    
    // Update mSize to the new value
    mSize = size;
}

template <class T>
void Vector<T>::reserve(const int capacity)
{
    if (capacity >= mCapacity)
    {
        // Allocate a new array. The () syntax will cause
        // 'new' to initialize the memory to 0's initially.
        mCapacity = capacity;
        // guaranteeing that all "0" i think
        T* data   = new T[capacity]();
        
        // Copy any existing data into the new array
        // copy as long as there is actually data to copy
        if (mData != NULL)
        {
            for (int i = 0; i < mSize; ++i)
                data[i] = mData[i];
        }
        
        // Swap the two arrays
        // only deleting original array
        delete[] mData;
        mData = data;

        
    }
}
   
template <class T>
T& Vector<T>::operator[](const int index)
{
    return mData[index];
}

template <class T>
const T& Vector<T>::operator[](const int index) const
{
    return mData[index];
}
    
template <class T>
T Vector<T>::get(const int index) const
{
    // If 'index' is within bounds, just return that cell.
    if (index >= 0 && index < mSize)
        return mData[index];
    
    // Otherwise, we'll return 0. Every code path has to
    // return something.
    // T() if T is an object calls default constructor
    // its the equivalent of zero but for that specific type
    // if had 0 would not work for strings, only works for integral types
    else return T();
}

template <class T>
void Vector<T>::set(const int index, const T& value)
{
    // If 'index' is within bounds, perform the update.
    if (index >= 0 && index < mSize)
        mData[index] = value;
}

template <class T>
void Vector<T>::pushBack(const T& value)
{
    // When we've run out of room, double the space
    if (mSize >= mCapacity)
        // does reallocation does resizing
        // this doubles the size of the "array"
        // order of  N operation
        reserve(mCapacity * 2);
    
    // Insert into the last cell of the array.
    mData[mSize] = value;
    mSize++;
}

template <class T>
void Vector<T>::popBack()
{
    // We don't need to bother clearing the last cell. We just
    // have to agree that it's not part of the vector any more.
    if (mSize > 0)
        mSize--;
}

template <class T>
int Vector<T>::getCapacity() const
{
    return mCapacity;
}

template <class T>
int Vector<T>::getSize() const
{
    return mSize;
}

template <class T>
void Vector<T>::print() const
{
    for (int i = 0; i < mSize; ++i)
        cout << mData[i] << " ";
    cout << endl;
}

#endif