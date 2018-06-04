#include "Block.h"

#include<iostream>

Block::Block(int id, int nBlocks, int globalRows, int globalCols)
{
    this->id_ = id;
    this->nBlocks=nBlocks;

    //Compute the number of columns
    if(is(Positioning::LEFT))
        this->cols = globalCols/2;
    else if(is(Positioning::RIGHT))
        this->cols = globalCols - globalCols/2;

    //Compute the number of rows
    if(!is(Positioning::BOTTOM))
        this->rows = globalRows / (nBlocks/2);
    else
        this->rows = globalRows - ((globalRows / (nBlocks/2)) * (nBlocks/2 - 1));

    //Compute the number of rows and columns considering neighbours
    this->colsNeigh = this->cols;
    this->rowsNeigh = this->rows;

    if(!is(Positioning::TOP))
        this->rowsNeigh++;
    if(!is(Positioning::BOTTOM))
        this->rowsNeigh++;

    if(!is(Positioning::LEFT))
        this->colsNeigh++;
    if(!is(Positioning::RIGHT))
        this->colsNeigh++;


    //Compute the first cell in the Cellular Automata
    if(is(Positioning::LEFT))
        this->globalFirst_ = (this->id_/2) * (rows)*globalCols - globalCols*(id/2?1:0);
    if(is(Positioning::RIGHT))
        this->globalFirst_ = ((this->id_/2) * (rows)*globalCols) + (globalCols/2 - 1)   - globalCols*(id/2?1:0);


    //Compute the first cell in the Block
    this->first_ = 0;
    if(!is(Positioning::TOP))
        this->first_ = this->first_ + this->colsNeigh;
    if(!is(Positioning::LEFT))
        this->first_++;

}

bool Block::is(Block::Positioning positioning) const
{
    if(positioning == Positioning::TOP)
        return 0<=id_ && id_<2;

    if(positioning == Positioning::RIGHT)
        return id_ % 2 != 0;

    if(positioning == Positioning::BOTTOM)
        return (nBlocks-2)<=id_ && id_<nBlocks;

    if(positioning == Positioning::LEFT)
        return id_ % 2 == 0;
}

int Block::nRows() const
{
    return this->rows;
}

int Block::nColumns() const
{
    return this->cols;
}

int Block::nRowsWithNeighbours() const
{
    return this->rowsNeigh;
}

int Block::nColumnsWithNeighbours() const
{
    return this->colsNeigh;
}

int Block::first() const
{
    return this->first_;
}

int Block::globalFirst() const
{
    return this->globalFirst_;
}

int Block::borderTop() const
{
    return 0;
}

int Block::borderRight() const
{
    return colsNeigh-2;
}

int Block::borderLeft() const
{
    return 0;
}

int Block::borderBottom() const
{
    return colsNeigh*(rowsNeigh-1) - colsNeigh;
}

void Block::print() const
{
    std::cout<<"Id " << id_ << " rows " << rows << " cols " <<cols
            <<" rowsNeigh " << rowsNeigh << " colsNeigh " << colsNeigh
           << " first " << first_ <<" globalFirst "<<globalFirst_<<std::endl;

}
