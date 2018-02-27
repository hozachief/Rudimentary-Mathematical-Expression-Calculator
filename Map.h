#ifndef MAP_H
#define MAP_H

// Necessary for print()
#include <iostream>
using namespace std;

// Represents a single Node in the BST used to implement the map.
template <class Key, class Value>
struct Node
{
    Key key;
    Value value;
    Node<Key, Value>* left;
    Node<Key, Value>* right;
};

// This is an implementation of a binary tree-based map. A map
// stores (key, value) pairs of arbitrary types, and the nodes
// are stored in sorted order based on the key.
// This implementation does NOT guarantee O(lg N) behavior.
template <class Key, class Value>
class Map
{
public:
    // Constructors / Destructors
    Map();
    Map(const Map<Key, Value>& orig);
    ~Map();
    
    // Tree modification
    void insert(const Key& key, const Value& value);
    bool remove(const Key& key, Value& value);
    
    // Tree statistics
    bool search(const Key& key, Value& value) const;
    void print() const;
    int size() const;
    
private:
    Node<Key, Value>* mRoot;
    
    // Private helper functions. These do all the real work
    // for the recursive algorithms, but we expose a simpler
    // API to the user so they don't have to worry about the
    // implementation details.
    void copyHelper(Node<Key, Value>* src, Node<Key, Value>*& dest);
    void destroyHelper(Node<Key, Value>* root);
    
    void insertHelper(const Key& key, const Value& value, Node<Key, Value>*& root);
    bool removeHelper(const Key& key, Value& value, Node<Key, Value>*& root);
    void deleteNode(Node<Key, Value>*& root);
    
    bool searchHelper(const Key& key, Value& value, const Node<Key, Value>* root) const;
    void printHelper(const Node<Key, Value>* root) const;
    int sizeHelper(const Node<Key, Value>* root)  const;
};

// ----------------------------------------------------//

template <class Key, class Value>
Map<Key, Value>::Map()
{
    mRoot = NULL;
}

// ----------------------------------------------------//

template <class Key, class Value>
Map<Key, Value>::Map(const Map<Key, Value>& orig)
{
    mRoot = NULL;
    copyHelper(orig.mRoot, mRoot);
}

template <class Key, class Value>
void Map<Key, Value>::copyHelper(Node<Key, Value>* src, Node<Key, Value>*& dest)
{
    // 'src' was empty - nothing to clone
    if (src == NULL) 
        dest = NULL;
    
    else
    {
        // Turn 'dest' into a clone of 'src'
        dest        = new Node<Key, Value>();
        dest->key   = src->key;
        dest->value = src->value;
        dest->left  = NULL;
        dest->right = NULL;
        
        // Recursively clone the left and right children
        copyHelper(src->left,  dest->left);
        copyHelper(src->right, dest->right);
    }
}

// ----------------------------------------------------//

template <class Key, class Value>
Map<Key, Value>::~Map()
{
    destroyHelper(mRoot);
    mRoot = NULL;
}

template <class Key, class Value>
void Map<Key, Value>::destroyHelper(Node<Key, Value>* root)
{
    // Use a postorder traversal for deletion. Delete both
    // the left and right children before we delete ourselves.
    if (root != NULL)
    {
        destroyHelper(root->left);
        destroyHelper(root->right);
        delete root;
    }
}

// ----------------------------------------------------//

// IMPLEMENT INSERT(), REMOVE(), and SEARCH() HERE
template <typename Key, typename Value>
void Map<Key, Value>::insert(const Key& key, const Value& value)
{
    // insert() should NOT allow duplicate keys to be inserted. If a duplicate node is 
    // found, the node's value should be updated instead.
    
    insertHelper(key, value, mRoot);
}

template <typename Key, typename Value>
void Map<Key, Value>::insertHelper(const Key& key, const Value& value, Node<Key, Value>*& root)
{
    // When 'root' is NULL, we've found where to insert the value
    if (root == NULL)
    {
        root         = new Node<Key, Value>();
        root->key    = key;
        root->value  = value;
        root->left   = NULL;
        root->right  = NULL;
    }
    
    // duplicate keys
    else if (root->key == key)
    {
        root->value = value;
    }    
    // Recurse either to the left or to the right to determine
    // where 'value' should be placed.
    else if (root->key > key)
    {
        insertHelper(key, value, root->left);
    }
    else
    {
        insertHelper(key, value, root->right); 
    }
}

template <typename Key, typename Value>
bool Map<Key, Value>::remove(const Key& key, Value& value)
{
    // remove() should return the value of the node being deleted along with the 
    // flag indicating whether or not the operation was successful.
    
    return removeHelper(key, value, mRoot);
}

template <typename Key, typename Value>
bool Map<Key, Value>::removeHelper(const Key& key, Value& value, Node<Key, Value>*& root)
{
    // If the root is empty, 'value' wasn't in the tree.
    if (root == NULL)
        return false;
    
    // We found 'value'! Eliminate it and return true.
    else if (root->key == key)
    {
        value = root->value;
        // Remove root
        deleteNode(root);
        return true;
    }
    // basically recurse until you find the root i think
    // 'value' must either be on the left branch or the
    // right branch. Use binary search to find it.
    else if (root->key > key)
    {
        return removeHelper(key, value, root->left);
    }
    else
    {
        return removeHelper(key, value, root->right);
    }    
}

template <typename Key, typename Value>
void Map<Key, Value>::deleteNode(Node<Key, Value>*& root)
{
    // if its a leaf node
    // Zero children - We delete ourselves.
    if (root->left == NULL && root->right == NULL)
    {
        delete root;
        root = NULL;
    }
    
    // One child (left) - Replace ourselves with our child
    else if (root->left != NULL && root->right == NULL)
    {
        // set pointer to NULL when pointer exist beyond the scope of function
        // temp pointer is to allow us to delete the node i think
        Node<Key, Value>* tmp = root;
        root = root->left;
        delete tmp;
    }
    
    // One child (right) - Replace ourselves with our child
    else if (root->left == NULL && root->right != NULL)
    {
        Node<Key, Value>* tmp = root;
        root = root->right;
        delete tmp;
    }
    
    // Two children - Find our successor, and replace ourselves
    // with them. Then delete the successor.
    else
    {
        // Find the successor (leftmost child of right subtree)
        Node<Key, Value>* parent = root;
        Node<Key, Value>* succ   = parent->right;
        while (succ->left != NULL)
        {
            parent = succ;
            succ   = succ->left;
        }
        
        // The successor's parent will adopt the successor's children
        if (parent != root)
            parent->left = succ->right;
        
        // The successor is our child. We must adopt their children.
        else root->right = succ->right;
        
        // Swap our data with the successor's
        root->value = succ->value;
        root->key = succ->key;
        
        // It's safe to delete the node now.
        delete succ;       
    }
}

template <typename Key, typename Value>
bool Map<Key, Value>::search(const Key& key, Value& value) const
{
    // search() should save the corresponding value once the node with the given key 
    // is found. 
    
    return searchHelper(key, value, mRoot);
}

template <typename Key, typename Value>
bool Map<Key, Value>::searchHelper(const Key& key, Value& value, const Node<Key, 
        Value>* root) const
{
    // If the subtree is empty, we didn't find 'value'
    if (root == NULL) return false;
    
    // We found 'value'!
    else if (root->key == key)
    {
        value = root->value;
        return true;
    }
    // We have to search either the left or the right
    // subtree using binary search.
    else if (root->key > key)
    {
        return searchHelper(key, value, root->left);
    }
    else
    {
        return searchHelper(key, value, root->right);  
    }
}

// ----------------------------------------------------//

template <class Key, class Value>
void Map<Key, Value>::print() const
{
    printHelper(mRoot);
    cout << endl;
}

template <class Key, class Value>
void Map<Key, Value>::printHelper(const Node<Key, Value>* root) const
{
    // If the tree is empty, there is nothing to print
    if (root == NULL)
        return;
    
    // Use an in-order traversal to print the tree's contents.
    // An in-order traversal will print the data in sorted order.
    printHelper(root->left);
    cout << "(" << root->key << ", " << root->value << ") ";
    printHelper(root->right);
}

// ----------------------------------------------------//

template <class Key, class Value>
int Map<Key, Value>::size() const
{
    return sizeHelper(mRoot);
}

template <class Key, class Value>
int Map<Key, Value>::sizeHelper(const Node<Key, Value>* root) const
{
    if (root == NULL) return 0;
    else              return sizeHelper(root->left) + sizeHelper(root->right) + 1;
}
  
#endif
