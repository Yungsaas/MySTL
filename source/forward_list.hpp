#pragma once

namespace mystl
{

template<typename T>
class forward_list
{
    private:

    //node struct to hold data
    struct Node
    {
        T data; //actual data stored in this node
        Node* next; //pointer to the next node in the list

        //constructor
        Node(const T& val, Node* nxt = nullptr): data(val), next(nxt){}

        //move constructor
        Node(T&& val, Node* nxt = nullptr): data(val), next(nxt){}
    };
    //number of elements in the list
    size_t _size;
};

}