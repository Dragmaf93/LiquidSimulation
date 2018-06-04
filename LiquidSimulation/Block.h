#ifndef BLOCK_H
#define BLOCK_H


class Block{
public:
    enum Positioning{TOP,RIGHT,BOTTOM,LEFT};

    Block(int id, int nBlocks ,int globalRows, int globalCols);

    bool is(Positioning positioning)const;

    int nRows()const;
    int nColumns()const;

    int nRowsWithNeighbours()const;
    int nColumnsWithNeighbours()const;

    int first()const;
    int globalFirst()const;
    int id()const;

    void print()const;
private:
    int id_;
    int nBlocks;

    int cols,rows;
    int colsNeigh,rowsNeigh;
    int globalFirst_,first_;
};

#endif // BLOCK_H
