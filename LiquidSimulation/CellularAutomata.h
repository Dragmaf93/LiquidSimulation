#ifndef CELLULAR_AUTOMATA_H
#define CELLULAR_AUTOMATA_H

#include<cassert>
#include<iostream>


template <class T>
class CellularAutomata
{
public:
    enum Neighbour{UP=0,LEFT,DOWN,RIGHT};

    struct Cell{
        Cell(unsigned long i, unsigned long j){
            this->i=i;
            this->j=j;
        }
        unsigned long i;
        unsigned long j;
    };

    CellularAutomata(T* states,unsigned long size);
    CellularAutomata(T* states,unsigned long dim1,unsigned long dim2);


    CellularAutomata(unsigned long size);

    CellularAutomata(long unsigned dim1, long unsigned dim2);
    ~CellularAutomata();

    T& state(Cell &cell);
    T& state(Cell &cell)const;
    T& state(long unsigned i, long unsigned j)const;
    T& state(long unsigned i, long unsigned j);
    T& state(long unsigned i);
    T& state(long unsigned i)const;

    T *automataStates();

    Cell **neighbour(Cell &cell)const;
    Cell **neighbour(long unsigned i, long unsigned j)const;

    long unsigned getDim1()const;
    long unsigned getDim2()const;
    long unsigned getSize()const;

    void print()const;
    long unsigned convert(long unsigned i, long unsigned j)const;
    Cell convert(long unsigned i)const;
protected:
    long unsigned dim1;
    long unsigned dim2;

    long unsigned size;
    T * states;
    
};

#include"CellularAutomata.cpp"

#endif
