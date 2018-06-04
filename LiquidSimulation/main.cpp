#include <QCoreApplication>

#include <mpi.h>
#include <iostream>
#include <string>

#include "LiquidSimulation.h"
#include "Block.h"

#define MASTER 0
#define TAG_INIT_BLOCKS 1
#define TAG_SEND_VERTICAL_NEIGHBOURS 2
#define TAG_SEND_HORIZONTAL_NEIGHBOURS 3

#define DIM1 8
#define DIM2 8

std::string arrayToString(float* array, int size);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int id,numProces;

    int rows=0,columns=0;

    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &numProces);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    CellularAutomata<float> *automata;
    CellularAutomata<float> *automataBlock;

    Block** allBlocks = NULL;

    rows= DIM1;
    columns = DIM2;

    Block block(id,numProces,rows,columns);


    if(id == MASTER)
    {
        automata = new CellularAutomata<float>(rows,columns);
        for(int i = 0; i < automata->getSize();i++)
            automata->state(i)=i;
//        automata->print();

        allBlocks = new Block* [numProces];
        for(int i = 0; i < numProces; i++){
            allBlocks[i] = new Block(i,numProces,rows,columns);
            //            allBlocks[i] -> print();
        }


        //The MASTER divides the Cellular Automata in BLOCKS and sends them to all the process
        for(int i = 0; i < numProces;i++){
            //DEFINE BLOCK TYPE
            MPI_Datatype blocktype;
            MPI_Type_vector(allBlocks[i]->nRowsWithNeighbours(),allBlocks[i]->nColumnsWithNeighbours(),columns,MPI_FLOAT,&blocktype);
            MPI_Type_commit(&blocktype);

            //Master sends blocks
            MPI_Send(automata->automataStates()+allBlocks[i]->globalFirst(),1,blocktype,i,TAG_INIT_BLOCKS,MPI_COMM_WORLD);

            MPI_Type_free(&blocktype);
        }
    }
    automataBlock = new CellularAutomata<float>(block.nRowsWithNeighbours(),block.nColumnsWithNeighbours());

    //All the process receive the BLOCK from the MASTER
    MPI_Recv(automataBlock->automataStates(),block.nRowsWithNeighbours()*block.nColumnsWithNeighbours()
             ,MPI_FLOAT,MASTER,TAG_INIT_BLOCKS,MPI_COMM_WORLD,&status);

    //DEFINE VERTICAL_NEIGHBOURS TYPE
    MPI_Datatype verticalNeighboursType;
    MPI_Type_vector(block.nRowsWithNeighbours(),1,block.nColumnsWithNeighbours(),MPI_FLOAT,&verticalNeighboursType);
    MPI_Type_commit(&verticalNeighboursType);

    //DEFINE HORIZONTAL_NEIGHBOURS TYPE
    MPI_Datatype horizontalNeighboursType;
    MPI_Type_contiguous(block.nColumnsWithNeighbours(),MPI_FLOAT,&horizontalNeighboursType);
    MPI_Type_commit(&horizontalNeighboursType);

//    while(0)
    {
        CellularAutomata<float>::Cell starter = automataBlock->convert(block.first());

        for(int i = starter.i; i < starter.i+block.nRows(); i++){
            for(int j = starter.j; j < starter.j+block.nColumns(); j++)
                qDebug() <<id << automataBlock->state(i,j);
        }


        //SEND BORDERS

        //if the BLOCK isn't to the TOP of the main Cellular Automata, sends its TOP border to the upstairs BLOCK
        if(!block.is(Block::Positioning::TOP)){
            //        qDebug() <<id<< "Send horizontal top border"<<automataBlock->state(0)<<"to"<<id - 2;
            MPI_Send(automataBlock->automataStates(),1,horizontalNeighboursType,id-2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD);
        }

        //if the BLOCK isn't to the LEFT of the main Cellular Automata, sends its LEFT border to the left BLOCK
        if(!block.is(Block::Positioning::LEFT)){
            //        qDebug() <<id<< "Send vertical left border"<<automataBlock->state(0) << "to"<<id-1;
            MPI_Send(automataBlock->automataStates(),1,verticalNeighboursType,id-1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD);
        }

        //if the BLOCK isn't to the RIGHT of the main Cellular Automata, sends its RIGHT border to the right BLOCK
        if(!block.is(Block::Positioning::RIGHT)){
            //        qDebug() <<id<< "Send vertical right border"<<automataBlock->state(block.nColumnsWithNeighbours()-1) << "to"<<id+1;
            MPI_Send(automataBlock->automataStates()+block.nColumnsWithNeighbours()-1,1,verticalNeighboursType,id+1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD);
        }

        //if the BLOCK isn't to the BOTTOM of the main Cellular Automata, sends its BOTTOM border to the below BLOCK
        if(!block.is(Block::Positioning::BOTTOM)){
            int n = block.nColumnsWithNeighbours()*block.nRowsWithNeighbours()-block.nColumnsWithNeighbours();
            //        qDebug() <<id<< "Send horizontal bottom border"<<automataBlock->state(n)<< "to"<<id+2;
            MPI_Send(automataBlock->automataStates()+n,1,horizontalNeighboursType,id+2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD);
        }

        //RECEIVE BORDERS

        //if the BLOCK isn't to the TOP of the main Cellular Automata, receives from the upstair BLOCK its BOTTOM border
        if(!block.is(Block::Positioning::TOP)){
            float * tmp = new float [block.nColumnsWithNeighbours()];
            MPI_Recv(tmp,block.nColumnsWithNeighbours(),MPI_FLOAT, id-2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD,&status);

            //qDebug()<<id<<"receives from" <<id-2<<arrayToString(tmp,block.nColumnsWithNeighbours()).c_str();
        }

        //if the BLOCK isn't to the LEFT of the main Cellular Automata, receives from the LEFT BLOCK its RIGHT border
        if(!block.is(Block::Positioning::LEFT)){
            float * tmp = new float [block.nRowsWithNeighbours()];
            MPI_Recv(tmp,block.nRowsWithNeighbours(),MPI_FLOAT, id-1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
            //qDebug()<<id<<"receives from" <<id-1<<arrayToString(tmp,block.nRowsWithNeighbours()).c_str();

        }

        //if the BLOCK isn't to the RIGHT of the main Cellular Automata, receives from the RIGHT BLOCK its LEFT border
        if(!block.is(Block::Positioning::RIGHT)){
            float * tmp = new float [block.nRowsWithNeighbours()];
            MPI_Recv(tmp,block.nRowsWithNeighbours(),MPI_FLOAT, id+1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
            //        qDebug()<<id <<"receives from" <<id+1<<arrayToString(tmp,block.nRowsWithNeighbours()).c_str();

        }

        //if the BLOCK isn't to the BOTTOM of the main Cellular Automata, receives from the below BLOCK its TOP border
        if(!block.is(Block::Positioning::BOTTOM)){
            float * tmp = new float [block.nColumnsWithNeighbours()];
            MPI_Recv(tmp,block.nColumnsWithNeighbours(),MPI_FLOAT, id+2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
            //        qDebug()<<id<<"receives from" <<id+2<<arrayToString(tmp,block.nColumnsWithNeighbours()).c_str();
        }

    }
    //    if(id==0)
    //    automataBlock->print();

    MPI_Type_free(&verticalNeighboursType);
    MPI_Type_free(&horizontalNeighboursType);

    MPI_Finalize();

    return a.exec();
}


std::string arrayToString(float* array, int size){
    std::string s("");
    for(int i = 0; i < size; i++){
        s+=" ";
        s+=std::to_string(array[i]);
        s+=" ";
    }
    s+="\n";
    return s;
}

