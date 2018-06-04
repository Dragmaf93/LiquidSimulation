#include "CellularAutomata.h"
template <class T>
CellularAutomata<T>::CellularAutomata(T *states, unsigned long size)
{
    this->states = states;
    this->size = size;
    this->dim1 = size/2;
    this->dim2 = size/2;
}
template<class T>
CellularAutomata<T>::CellularAutomata(T *states, unsigned long dim1, unsigned long dim2)
{
    this->states = states;
    this->dim1 = dim1;
    this->dim2 = dim2;
    this->size = dim1*dim2;
}

template<class T>
CellularAutomata<T>::CellularAutomata(long unsigned size)
{
    this->dim1 = size/2;
    this->dim2 = size/2;
    this->size = size;

    this->states = new T[size];
}

template<class T>
CellularAutomata<T>::CellularAutomata(long unsigned dim1, long unsigned dim2){
    this->dim1 = dim1;
    this->dim2 = dim2;
    this->size = dim1*dim2;

    this->states = new T[size];
}

template <class T>
CellularAutomata<T>::~CellularAutomata(){
    delete this->states;
}

template<class T>
T &CellularAutomata<T>::state(CellularAutomata::Cell &cell)
{
    return state(cell.i,cell.j);
}

template<class T>
T &CellularAutomata<T>::state(CellularAutomata::Cell &cell) const
{
    return state(cell.i,cell.j);
}

template <class T>
T& CellularAutomata<T>::state(long unsigned i, long unsigned j)const{
    long unsigned tmp = convert(i,j);
    return this->states[tmp];
}

template <class T>
T& CellularAutomata<T>::state(long unsigned i, long unsigned j){
    long unsigned tmp = convert(i,j);
    return this->states[tmp];
}

template <class T>
T& CellularAutomata<T>::state(long unsigned i)const{
    assert(i<this->size);
    return this->states[i];
}

template<class T>
T* CellularAutomata<T>::automataStates()
{
    return states;
}

template<class T>
typename CellularAutomata<T>::Cell** CellularAutomata<T>::neighbour(CellularAutomata<T>::Cell &cell) const
{
    return neighbour(cell.i,cell.j);
}

template<class T>
typename CellularAutomata<T>::Cell** CellularAutomata<T>::neighbour(unsigned long i, unsigned long j) const
{
   Cell** neighbours = new Cell*[4];
   if(i>0 && i<dim1 && j<dim2 && j>=0)
    neighbours[UP] = new Cell(i-1,j);

   if(i<dim1-1 && i>=0 && j<dim2 && j>=0)
       neighbours[DOWN] = new Cell(i+1,j);

   if(j>0 && j<dim2 && i<dim1 && i>=0)
       neighbours[LEFT] = new Cell(i,j-1);

   if(j<dim2-1 && j>=0 && i<dim1 && i>=0)
       neighbours[RIGHT] = new Cell(i,j+1);

   return neighbours;
}

template <class T>
T& CellularAutomata<T>::state(long unsigned i){
    assert(i<this->size);
    return this->states[i];
}

template <class T>
long unsigned CellularAutomata<T>::getDim1()const{
    return this->dim1;
}
template <class T>
long unsigned CellularAutomata<T>::getDim2()const{
    return this->dim2;
}

template<class T>
unsigned long CellularAutomata<T>::getSize() const
{
    return size;
}
template <class T>
void CellularAutomata<T>::print()const{

    for(long unsigned i=0; i < dim1; i++)
    {
        for(long unsigned  j=0; j< dim2;j++)
            std::cout<<this->states[convert(i,j)]<<" ";
        
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

template <class T>
long unsigned CellularAutomata<T>::convert(long unsigned i, long unsigned j)const{
    assert(i<dim1);
    assert(j<dim2);
    return i*dim2+j;
}

template<class T>
typename CellularAutomata<T>::Cell CellularAutomata<T>::convert(unsigned long i) const
{
    assert(i<size);

    return Cell(i/dim2,i%dim2);
}


