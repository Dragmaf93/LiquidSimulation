#include <QCoreApplication>
#include <mpi.h>
#include <iostream>
#include <string>

#include "LiquidSimulation.h"
#include "Block.h"
#include "CommandLineParser.h"
#include "TcpServer.h"

#define TAG_INIT_BLOCKS 1
#define TAG_SEND_VERTICAL_NEIGHBOURS 2
#define TAG_SEND_HORIZONTAL_NEIGHBOURS 3
#define TAG_SEND_BLOCK 4

#define PRINT_CA -1
#define DIM1 8
#define DIM2 8
#define MAX_STEP 30
#define NS_BLOCK_SEND 5

#define SERVER 1

std::string arrayToString(float* array, int size);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer* server;

    //Get arguments
    CommandLineParser parser;
    parser.process(a);

    int portNumber;
    int rows=0,columns=0;
    int step=0, updateStep=0,maxStep;
    QString initCA;
    //Init simumaltion parameters
    parser.rows(rows);
    parser.columns(columns);
    parser.steps(maxStep);
    parser.updateSteps(updateStep);
    parser.port(portNumber);
    parser.data(initCA);

    if(parser.isThereAnError()) return -1;



    int id,numProces;
    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &numProces);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int MASTER = numProces-1;

    if(id == MASTER)
    {
#ifdef SERVER
        server = new TcpServer();
        if(!server->openServer(portNumber)) a.exit(-1);
#endif

    }


    CellularAutomata<float> *automata;
    CellularAutomata<float> *automataBlock;

    //MASTER STUFF
    Block** allBlocks = NULL;

    //DEFINE BLOCK TYPE
    MPI_Datatype* blockstype;

    //DEFINE VERTICAL_NEIGHBOURS TYPE
    MPI_Datatype verticalNeighboursType;

    //DEFINE HORIZONTAL_NEIGHBOURS TYPE
    MPI_Datatype horizontalNeighboursType;


    Block* block;


    if(id != MASTER){
        block = new Block(id,numProces-1,rows,columns);

        MPI_Type_vector(block->nRowsWithNeighbours(),2,block->nColumnsWithNeighbours(),MPI_FLOAT,&verticalNeighboursType);
        MPI_Type_commit(&verticalNeighboursType);

        MPI_Type_vector(2,block->nColumnsWithNeighbours(),block->nColumnsWithNeighbours(),MPI_FLOAT,&horizontalNeighboursType);
        //MPI_Type_contiguous(block->nColumnsWithNeighbours(),MPI_FLOAT,&horizontalNeighboursType);
        MPI_Type_commit(&horizontalNeighboursType);
    }

    MPI_Barrier(MPI_COMM_WORLD);


    if(id == MASTER)
    {
        automata = new CellularAutomata<float>(rows,columns);
        for(int i = 0; i < automata->getSize();i++)
            automata->state(i)=0;
        parser.data(*automata);
//        automata->print();

        //        automata->state(37)=LiquidSimulation::AddWater;

        allBlocks = new Block* [numProces-1];
        for(int i = 0; i < numProces-1; i++){
            allBlocks[i] = new Block(i,numProces-1,rows,columns);
            //            allBlocks[i] -> print();
        }



        //        qDebug() <<"Here";
        //The MASTER divides the Cellular Automata in BLOCKS and sends them to all the process
        blockstype = new MPI_Datatype[numProces-1];
        for(int i = 0; i < numProces-1;i++){
            //DEFINE BLOCK TYPE
            //            MPI_Datatype blocktype;
            MPI_Type_vector(allBlocks[i]->nRowsWithNeighbours(),allBlocks[i]->nColumnsWithNeighbours(),columns,MPI_FLOAT,&blockstype[i]);
            MPI_Type_commit(&blockstype[i]);

            //Master sends blocks
            MPI_Send(automata->automataStates()+allBlocks[i]->globalFirst(),1,blockstype[i],i,TAG_INIT_BLOCKS,MPI_COMM_WORLD);

            //            MPI_Type_free(&blocktype);
        }
    }else{


        automataBlock = new CellularAutomata<float>(block->nRowsWithNeighbours(),block->nColumnsWithNeighbours());

        //All the process receive the BLOCK from the MASTER
        MPI_Recv(automataBlock->automataStates(),block->nRowsWithNeighbours()*block->nColumnsWithNeighbours()
                 ,MPI_FLOAT,MASTER,TAG_INIT_BLOCKS,MPI_COMM_WORLD,&status);

        LiquidSimulation::initSimulator(*automataBlock);

        //Find eventually Source water in the automata
        CellularAutomata<float>::Cell starter = automataBlock->convert(block->first());
        for(int i = starter.i; i < starter.i+block->nRows(); i++)
            for(int j = starter.j; j < starter.j+block->nColumns(); j++)
                if(automataBlock->state(i,j) == LiquidSimulation::SOURCE_WATER){
                    LiquidSimulation::SourcesWater.append(CellularAutomata<float>::Cell(i,j));
                    automataBlock->state(i,j)=0;
                }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    //    if(id == PRINT_CA)
    //        block->print();
    //Computation liquid simulation
    while(step < maxStep)
    {
        if(id!=MASTER){

            for(int s = 0; s < LiquidSimulation::SourcesWater.size();s++)
                automataBlock->state(LiquidSimulation::SourcesWater[s]) = LiquidSimulation::AddWater;

            CellularAutomata<float>::Cell starter = automataBlock->convert(block->first());

            //            for(int i = 0; i < automataBlock->getDim1();i++)
            //                for(int j = 0; j < automataBlock->getDim2();j++)
            //                    LiquidSimulation::tmpAutomata->state(i,j)=0;

            for(int i = starter.i; i < starter.i+block->nRows(); i++)
                for(int j = starter.j; j < starter.j+block->nColumns(); j++)
                    LiquidSimulation::transitionFunction(*automataBlock,i,j);

            if(id==PRINT_CA){
                automataBlock->print();
            }
            //SEND BORDERS

            //if the BLOCK isn't to the TOP of the main Cellular Automata, sends its TOP border to the upstairs BLOCK
            if(!block->is(Block::Positioning::TOP)){
                MPI_Send(LiquidSimulation::tmpAutomata->automataStates()+block->borderTop(),1,horizontalNeighboursType,id-2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD);
            }

            //if the BLOCK isn't to the LEFT of the main Cellular Automata, sends its LEFT border to the left BLOCK
            if(!block->is(Block::Positioning::LEFT)){
                MPI_Send(LiquidSimulation::tmpAutomata->automataStates()+block->borderLeft(),1,verticalNeighboursType,id-1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD);
            }

            //if the BLOCK isn't to the RIGHT of the main Cellular Automata, sends its RIGHT border to the right BLOCK
            if(!block->is(Block::Positioning::RIGHT)){
                MPI_Send(LiquidSimulation::tmpAutomata->automataStates()+block->borderRight(),1,verticalNeighboursType,id+1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD);
            }

            //if the BLOCK isn't to the BOTTOM of the main Cellular Automata, sends its BOTTOM border to the below BLOCK
            if(!block->is(Block::Positioning::BOTTOM)){
                MPI_Send(LiquidSimulation::tmpAutomata->automataStates()+block->borderBottom(),1,horizontalNeighboursType,id+2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD);
            }

            //RECEIVE BORDERS

            //if the BLOCK isn't to the TOP of the main Cellular Automata, receives from the upstair BLOCK its BOTTOM border
            if(!block->is(Block::Positioning::TOP)){
                float * tmp = new float [block->nColumnsWithNeighbours()*2];
                //                qDebug()<<id<<"receives from"<<id-2;
                MPI_Recv(tmp,block->nColumnsWithNeighbours()*2,MPI_FLOAT, id-2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD,&status);

                //                                qDebug()<<id<<"receives from" <<id-2<<arrayToString(tmp,block->nColumnsWithNeighbours()*2).c_str();
                for(int i = block->borderTop();i < block->nColumnsWithNeighbours()*2; i++)
                    LiquidSimulation::tmpAutomata->state(i)+=tmp[i];

                delete tmp;
            }

            //if the BLOCK isn't to the LEFT of the main Cellular Automata, receives from the LEFT BLOCK its RIGHT border
            if(!block->is(Block::Positioning::LEFT)){
                float * tmp = new float [block->nRowsWithNeighbours()*2];
                MPI_Recv(tmp,block->nRowsWithNeighbours()*2,MPI_FLOAT, id-1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
                //                                qDebug()<<id<<"receives from" <<id-1<<arrayToString(tmp,block->nRowsWithNeighbours()*2).c_str();
                for(int i=block->borderLeft(),j=0; j < block->nRowsWithNeighbours()*2;j++)
                {
                    LiquidSimulation::tmpAutomata->state(i)+=tmp[j];
                    if(j%2 == 0) i++;
                    else i+=block->nColumnsWithNeighbours()-1;
                }
                delete tmp;
            }

            //if the BLOCK isn't to the RIGHT of the main Cellular Automata, receives from the RIGHT BLOCK its LEFT border
            if(!block->is(Block::Positioning::RIGHT)){
                float * tmp = new float [block->nRowsWithNeighbours()*2];
                MPI_Recv(tmp,block->nRowsWithNeighbours()*2,MPI_FLOAT, id+1,TAG_SEND_VERTICAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
                //                                qDebug()<<id <<"receives from" <<id+1<<arrayToString(tmp,block->nRowsWithNeighbours()*2).c_str();

                for(int i=block->borderRight(),j=0; j < block->nRowsWithNeighbours()*2;j++)
                {
                    LiquidSimulation::tmpAutomata->state(i)+=tmp[j];
                    if(j%2 == 0) i++;
                    else i+=block->nColumnsWithNeighbours()-1;
                }
                delete tmp;
            }

            //if the BLOCK isn't to the BOTTOM of the main Cellular Automata, receives from the below BLOCK its TOP border
            if(!block->is(Block::Positioning::BOTTOM)){
                float * tmp = new float [block->nColumnsWithNeighbours()*2];
                MPI_Recv(tmp,block->nColumnsWithNeighbours()*2,MPI_FLOAT, id+2,TAG_SEND_HORIZONTAL_NEIGHBOURS,MPI_COMM_WORLD,&status);
                //                                qDebug()<<id<<"receives from" <<id+2<<arrayToString(tmp,block->nColumnsWithNeighbours()*2).c_str();

                for(int i = block->borderBottom(),j=0;j < block->nColumnsWithNeighbours()*2; i++,j++)
                    LiquidSimulation::tmpAutomata->state(i)+=tmp[j];
                delete tmp;
            }

            for(int i = 0; i < automataBlock->getSize();i++)
            {
                automataBlock->state(i) += LiquidSimulation::tmpAutomata->state(i);
                LiquidSimulation::tmpAutomata->state(i)=0.0f;
            }

            if(id==PRINT_CA)
                automataBlock->print();
        }

        //Send updated block to MASTER, in order to recompose the original Cellular Automata
        if(step % updateStep == 0){

            //MASTER receives sended blocks and recomposes the original Cellular Automata
            if(id==MASTER){

                for(int i=0;i<numProces-1;i++)
                {
                    MPI_Recv(automata->automataStates()+allBlocks[i]->globalFirst(),1,blockstype[i],i,TAG_SEND_BLOCK,MPI_COMM_WORLD,&status);
                }
                std::cout <<"step "<<step<<std::endl;
                //automata->print();
#ifdef SERVER
                server->sendCellullarAutomata(*automata);
#endif

            }
            else
                MPI_Send(automataBlock->automataStates(),automataBlock->getSize(),MPI_FLOAT,MASTER,TAG_SEND_BLOCK,MPI_COMM_WORLD);

            MPI_Barrier(MPI_COMM_WORLD);
        }
        step++;
    }

    //MASTER memory deallocation
    if(id==MASTER){
#ifdef SERVER
        server->closeConnection();
#endif
        for(int i=0; i < numProces-1; i++){
            MPI_Type_free(&blockstype[i]);
            delete allBlocks[i];
        }
        delete blockstype;
        delete allBlocks;
        delete automata;
    }else{
        //ALL  memory deallocation
        MPI_Type_free(&verticalNeighboursType);
        MPI_Type_free(&horizontalNeighboursType);

        delete automataBlock;
        delete block;

    }


    MPI_Finalize();

    a.exit(0);
    std::cout<<"End\n";
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

