#include <stdio.h>
#include <stdlib.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

struct distance_table dt1;
struct NeighborCosts   *neighbor1;
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor1, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

int i,j,k,min1,source_id1,isChanged1,oldcost1,newcost1,initCost1[MAX_NODES];

// initialize the table
void blankTable1(){
  // set all values to 9999
  for( i = 0; i < MAX_NODES; i++ ){
    for( j = 0; j < MAX_NODES; j++ ){
      dt1.costs[i][j] = INFINITY;
    }
    // set costs to neighbor respectively
    neighbor1 = getNeighborCosts(1);
  }
}

// fill the table
void fillTable1(){
  for( i = 0; i < neighbor1->NodesInNetwork; i++ ) {
    dt1.costs[i][i] = neighbor1->NodeCosts[i];
    initCost1[i] = neighbor1->NodeCosts[i];
  }
}


void findmin1(struct RoutePacket *routePacketSend){
  for( j = 0; j < MAX_NODES; j++ ) {
    min1 = INFINITY;
    for( k = 0; k < MAX_NODES; k++ ){
      if( dt1.costs[j][k] < min1 ){
        min1 = dt1.costs[j][k];
      }
      routePacketSend->mincost[j] = min1;
    }
    printf("%d ", min1);
  }
}

// calls the table
void callTableName1(char *table){
  printf("At time t=%f, %s called.\n", clocktime, table);
}

// print sending information
void printSend1(int i, char *node){
  printf("At time t=%f, %s sends packet to node%d with: ", clocktime, node, i);
}

void processData1(struct RoutePacket *routePacketSend){
  // update data
  routePacketSend->sourceid = 1;
  routePacketSend->destid = i;
}


//update min
void updateMin1(int i,struct RoutePacket *rcvdpkt){
  // checks if update needed
  oldcost1 = dt1.costs[i][source_id1];
  newcost1 = dt1.costs[source_id1][source_id1] + rcvdpkt->mincost[i];
  // update new minimum
  if( oldcost1 > newcost1 ) {
    dt1.costs[i][source_id1] = newcost1;
    isChanged1 = YES;
  }
}


void rtinit1() {
  // create blank table with nodes
  blankTable1();
  // fill the table with cost values
  fillTable1();
  // call table name
  callTableName1("rtinit1()");
  // print table
  printdt1(1, neighbor1, &dt1);
  // process data in struct
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));
  // start sending nodes
  for( i = 0; i < neighbor1->NodesInNetwork; i++ ) {

    // ignore sending self
    if(( i == 1 ) || ( initCost1[i] == INFINITY )){
      continue;
    }

    // print node time
    printSend1(i, "node1");
    // process the data
    processData1(routePacketSend);
    // find the min
    findmin1(routePacketSend);
    // send min to layer 2
    toLayer2(*routePacketSend);
    printf("\n");
  }
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
  // process received data
  source_id1 = rcvdpkt->sourceid;
  isChanged1 = NO;
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));

  for(i=0 ; i < MAX_NODES; i++ ) {
    // skip self
    if( i == 1 ){
      continue;
    }
    updateMin1(i,rcvdpkt);
  }
  // send changed nodes
  if( isChanged1 == YES ) {
    // print information
    callTableName1("rtupdate1()");
    // print sender
    printf("Sender id: %d",source_id1);
    // print table
    printdt1(1, neighbor1, &dt1);
    // prepare to send to layer 2
    for( i = 0; i < neighbor1->NodesInNetwork; i++ ) {
      // skip self and not linked
      if(( i == 1 ) || ( initCost1[i] == INFINITY )){
        continue;
      }
      printSend1(i,"node 1");
      // process data
      // set information
      processData1(routePacketSend);
      // find min1
      findmin1(routePacketSend);
      // send layer 2
      toLayer2(*routePacketSend);
      printf("\n");
    }
  }
}

/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor,
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt1
