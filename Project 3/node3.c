#include <stdio.h>
#include <stdlib.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

struct distance_table dt3;
struct NeighborCosts   *neighbor3;
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

int i,j,k,min3,source_id3,isChanged3,oldcost3,newcost3,initCost3[MAX_NODES];

// initialize the table
void blankTable3(){
  // set all values to 9999
  for( i = 0; i < MAX_NODES; i++ ){
    for( j = 0; j < MAX_NODES; j++ ){
      dt3.costs[i][j] = INFINITY;
    }
    // set costs to neighbor respectively
    neighbor3 = getNeighborCosts(3);
  }
}

// fill the table
void fillTable3(){
  for( i = 0; i < neighbor3->NodesInNetwork; i++ ) {
    dt3.costs[i][i] = neighbor3->NodeCosts[i];
    initCost3[i] = neighbor3->NodeCosts[i];
  }
}


void findmin3(struct RoutePacket *routePacketSend){
  for( j = 0; j < MAX_NODES; j++ ) {
    min3 = INFINITY;
    for( k = 0; k < MAX_NODES; k++ ){
      if( dt3.costs[j][k] < min3 ){
        min3 = dt3.costs[j][k];
      }
      routePacketSend->mincost[j] = min3;
    }
    printf("%d ", min3);
  }
}

// calls the table
void callTableName3(char *table){
  printf("At time t=%f, %s called.\n", clocktime, table);
}

// print sending information
void printSend3(int i, char *node){
  printf("At time t=%f, %s sends packet to node%d with: ", clocktime, node, i);
}

// update data
void processData3(struct RoutePacket *routePacketSend){
  routePacketSend->sourceid = 3;
  routePacketSend->destid = i;
}


//update min
void updateMin3(int i,struct RoutePacket *rcvdpkt){
  // checks if update needed
  oldcost3 = dt3.costs[i][source_id3];
  newcost3 = dt3.costs[source_id3][source_id3] + rcvdpkt->mincost[i];
  // update new minimum
  if( oldcost3 > newcost3 ) {
    dt3.costs[i][source_id3] = newcost3;
    isChanged3 = YES;
  }
}

// starts reading update tables
void title3(){
  printf("\n========================= START UPDATES ==========================\n\n");
}


void rtinit3() {
  // create blank table with nodes
  blankTable3();
  // fill the table with cost values
  fillTable3();
  // call table name
  callTableName3("rtinit3()");
  // print table
  printdt3(3, neighbor3, &dt3);
  // process data in struct
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));
  // start sending nodes
  for( i = 0; i < neighbor3->NodesInNetwork; i++ ) {

    // ignore sending self
    if(( i == 3 ) || ( initCost3[i] == INFINITY )){
      continue;
    }

    // print node time
    printSend3(i, "node 3");
    // process the data
    processData3(routePacketSend);
    // find the min
    findmin3(routePacketSend);
    // send min to layer 2
    toLayer2(*routePacketSend);
    printf("\n");
  }
  title3();
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
  // process received data
  source_id3 = rcvdpkt->sourceid;
  isChanged3 = NO;
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));

  for(i=0 ; i < MAX_NODES; i++ ) {
    // skip self
    if( i == 3 ){
      continue;
    }
    updateMin3(i,rcvdpkt);
  }
  // send changed nodes
  if( isChanged3 == YES ) {
    // print information
    callTableName3("rtupdate3()");
    // print sender
    printf("Sender id: %d",source_id3);
    // print table
    printdt3(3, neighbor3, &dt3);
    // prepare to send to layer 3
    for( i = 0; i < neighbor3->NodesInNetwork; i++ ) {
      // skip self and not linked
      if(( i == 3 ) || ( initCost3[i] == INFINITY )){
        continue;
      }
      printSend3(i,"node 3");
      // process data
      processData3(routePacketSend);
      // find min3
      findmin3(routePacketSend);
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
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor,
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
  }    // End of printdt3
