#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace mystl
{

template<typename T>

class my_vector
{
    public:
    //default constructor
    my_vector(): _data(nullptr), _size(0), _cap(0){}
    
    //construct with n elements
    explicit my_vector(size_t n): _data(nullptr), _size(0), _cap(0) 
    { 
        reserve(n); 
        for(size_t i = 0; i<n;i++)
        {
            push_back(T());
        }
    }

    //deconstructor
    ~my_vector() 
    { 
        clear(); 
        //raw memory gets freed
        operator delete[](static_cast<void*>(_data)); 
    }

    //copy constructor
    my_vector(const my_vector& other): _data(nullptr), _size(0), _cap(0)
    {
        //allocate enough memory for all elements in the other vector to fit
        reserve(other._size);
        //copy construct each element
        for(size_t i = 0; i<other._size;i++)
        {
            push_back(other._data[i]);
        }
    }

    //copy assignment
    my_vector& operator = (const my_vector& other)
    {
        //check for self assignment
        if(this==&other) return *this;
        clear();
        reserve(other._size);
        for(size_t i = 0; i<other._size; i++)
        {
            push_back(other._data[i]);
        }
        return *this;
    }

    //move constructor
    my_vector(my_vector&& other) noexcept: _data(other._data), _size(other._size), _cap(other._cap)
    {
        //make other vector empty
        other._data = nullptr;
        other._size = 0;
        other._cap = 0;
    }

    //move assignment
    my_vector& operator = (my_vector&& other)
    {
        if(this != &other)
        {
            //clear current elements
            clear();
            //free current memory
            operator delete[](static_cast<void*>(_data));

            //take ownership of other vectors buffers
            _data = other._data;
            _size = other._size;
            _cap = other._cap;

            //empty other vector
            other._data = nullptr;
            other._size = 0;
            other._cap = 0;
        }
        return *this;
    }

    //size query
    size_t size() const noexcept{return _size;}

    //capacity query
    size_t capacity() const noexcept{return _cap;}

    //empty query
    bool empty() const noexcept{return _size==0;}

    //reserve specific capacity
    void reserve(size_t new_cap)
    {
        //capacity shouldnt be lowered -> early out
        if(new_cap<=_cap) return;
        //allocate raw memory
        T* new_data = static_cast<T*>(operator new[](new_cap * sizeof(T)));
        //move construct existing data into new storage
        for(size_t i = 0; i<_size;i++)
        {
            new(new_data + i) T(std::move(_data[i]));
        }
        //destroy old elements
        for(size_t i = 0; i<_size;i++)
        {
            _data[i].~T();
        }
        //free old memory
        operator delete[](static_cast<void*>(_data));
        //update pointer to new storage
        _data = new_data;
        //update capacity
        _cap = new_cap;
    }

    //element access operator
    T& operator[](size_t id)
    {
        if(id>=_size)
        {
            throw std::out_of_range("tried to access out of bounds id");
        } 

        return _data[id];
    }
    const T& operator[](size_t id) const
    {
        if(id>=_size)
        { 
            throw std::out_of_range("tried to access out of bounds id");
        }
        return _data[id];
    }

    //push_back copy
    void push_back(const T& val)
    {
        if(_size==_cap)
        {   
            //doubling _cap trades unused memory for way fewer reallocations
            reserve((_cap==0)?1:_cap*2);
            /*
            * old:
            * reserve(_cap++);
            */
        }
        //copy construct val
        new(_data + _size) T(val); 
        _size++;
    }

    //push_back move
    void push_back(T&& val)
    {
        if(_size==_cap)
        {
            reserve((_cap==0)?1:_cap*2);
        }
        //move construct val
        new(_data + _size) T(std::move(val));   
        _size++;
    }

    //pop_back (remove last element)
    void pop_back()
    {
        if(_size==0)
        {
            throw std::out_of_range("tried to use pop_back on empty vector");
        }
        //reduce logical size first
        --_size;
        //destroy element at former end
        _data[_size].~T();
    }

    //clear function, destroys all elements but keeps capacity
    void clear()
    {
        for(size_t i = 0; i<_size; i++)
        {
            _data[i].~T();
        }
        _size = 0;
    }

    //equal operator
    bool operator == (const my_vector& other)
    {
        //check size diff
        if(_size!=other._size) return false;
        //check data diff
        for(size_t i = 0; i<_size; i++)
        {
            if(_data[i]!=other._data[i]) return false;
        }
        return true;
    }

    //inequal operator
    bool operator != (const my_vector& other)
    {
        return !(*this == other);
    }

    private:
    //pointer to storage for elements
    T* _data;
    //number of valid elements in storage
    size_t _size;
    //element capacity of the container
    size_t _cap;
};

}