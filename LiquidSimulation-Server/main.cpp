#include <QCoreApplication>
#include<mpi.h>
#include<string.h>
#include<stdio.h>

#define MAXSIZE 1000000000
#define MASTER 0
#define SEND_DATA 0
#define SEND_START 1
#define SEND_CHUNK 2
#define SEND_RES 3

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    int id,numProces;
    int* data,i,width,start,s,e,end,pres,res,chunk;
    int dest,source;

    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &numProces);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    res=0,start=0;
    pres=0;

    if(id == MASTER){

        data = new int[MAXSIZE];

        for(int i=0; i<MAXSIZE; i++)
            data[i] = 1;

        width = MAXSIZE/numProces;

        for(int i=1; i<numProces; i++){
            start+=width;
            end=start+width;
            if(i == numProces-1)
                end=MAXSIZE;
            chunk=end-start;

            MPI_Send(&chunk,1,MPI_INT,i,SEND_CHUNK,MPI_COMM_WORLD);
            MPI_Send(data+start,chunk,MPI_INT,i,SEND_DATA,MPI_COMM_WORLD);
        }

        chunk=width;
        start=0;

    }else{
        MPI_Recv(&chunk,1,MPI_INT,MASTER,SEND_CHUNK,MPI_COMM_WORLD,&status);
        data = new int[chunk];
        MPI_Recv(data,chunk,MPI_INT,MASTER,SEND_DATA,MPI_COMM_WORLD,&status);
    }
    //printf("Start %d chunk %d data %d\n",start,chunk,data[0]);

    for(i=0;i<chunk;i++)
        pres+=data[i];

    //printf("Partial res of %d is : %d\n",id,pres);
    if(id==MASTER){
        int tmp=0;
        for(int i=1; i<numProces; i++){
            MPI_Recv(&tmp,1,MPI_INT,i,SEND_RES,MPI_COMM_WORLD,&status);
            //printf("Partial res recived from %d is : %d\n",i,tmp);
            res+=tmp;
        }
        res+=pres;

        printf("The sum of all the elements is : %d\n",res);

    }else{
        MPI_Send(&pres,1,MPI_INT,MASTER,SEND_RES,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    delete data;

    return a.exec();
}
