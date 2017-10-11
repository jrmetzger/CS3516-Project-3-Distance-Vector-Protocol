#include <stdio.h>
#include <stdlib.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

struct distance_table dt0;
struct NeighborCosts   *neighbor0;
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor0, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

int i,j,k,min0,source_id0,isChanged0,oldcost0,newcost0,initCost0[MAX_NODES];

// initialize the table
void blankTable0(){
  // set all values to 9999
  for( i = 0; i < MAX_NODES; i++ ){
    for( j = 0; j < MAX_NODES; j++ ){
      dt0.costs[i][j] = INFINITY;
    }
    // set costs to neighbor respectively
    neighbor0 = getNeighborCosts(0);
  }
}

// fill the table
void fillTable0(){
  for( i = 0; i < neighbor0->NodesInNetwork; i++ ) {
    dt0.costs[i][i] = neighbor0->NodeCosts[i];
    initCost0[i] = neighbor0->NodeCosts[i];
  }
}


void findmin0(struct RoutePacket *routePacketSend){
  for( j = 0; j < MAX_NODES; j++ ) {
    min0 = INFINITY;
    for( k = 0; k < MAX_NODES; k++ ){
      if( dt0.costs[j][k] < min0 ){
        min0 = dt0.costs[j][k];
      }
      routePacketSend->mincost[j] = min0;
    }
    printf("%d ", min0);
  }
}

// calls the table
void callTableName0(char *table){
  printf("At time t=%f, %s called.\n", clocktime, table);
}

// print sending information
void printSend0(int i, char *node){
  printf("At time t=%f, %s sends packet to node%d with: ", clocktime, node, i);
}

// starts reading initial tables
void title0(){
  printf("\n========================= START INITIAL ==========================\n\n");
}

// process the data
void processData0(struct RoutePacket *routePacketSend){
  // update data
  routePacketSend->sourceid = 0;
  routePacketSend->destid = i;
}


//update min
void updateMin0(int i,struct RoutePacket *rcvdpkt){
  // checks if update needed
  oldcost0 = dt0.costs[i][source_id0];
  newcost0 = dt0.costs[source_id0][source_id0] + rcvdpkt->mincost[i];
  // update new minimum
  if( oldcost0 > newcost0 ) {
    dt0.costs[i][source_id0] = newcost0;
    isChanged0 = YES;
  }
}


void rtinit0() {
  // starts reading initial tables
  title0();
  // create blank table with nodes
  blankTable0();
  // fill the table with cost values
  fillTable0();
  // call table name
  callTableName0("rtinit0()");
  // print table
  printdt0(0, neighbor0, &dt0);
  // process data in struct
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));
  // start sending nodes
  for( i = 0; i < neighbor0->NodesInNetwork; i++ ) {

    // ignore sending self
    if(( i == 0 ) || ( initCost0[i] == INFINITY )){
      continue;
    }

    // print node time
    printSend0(i, "node0");
    // process the data
    processData0(routePacketSend);
    // find the min
    findmin0(routePacketSend);
    // send min to layer 2
    toLayer2(*routePacketSend);
    printf("\n");
  }
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
  // process received data
  source_id0 = rcvdpkt->sourceid;
  isChanged0 = NO;
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));

  for(i=0 ; i < MAX_NODES; i++ ) {
    // skip self
    if( i == 0 ){
      continue;
    }
    updateMin0(i,rcvdpkt);
  }
  // send changed nodes
  if( isChanged0 == YES ) {
    // print information
    callTableName0("rtupdate0()");
    // print sender
    printf("Sender id: %d",source_id0);
    // print table
    printdt0(0, neighbor0, &dt0);
    // prepare to send to layer 2
    for( i = 0; i < neighbor0->NodesInNetwork; i++ ) {
      // skip self and not linked
      if(( i == 0 ) || ( initCost0[i] == INFINITY )){
        continue;
      }
      printSend0(i,"node0");
      // set information
      processData0(routePacketSend);
      // find min0
      findmin0(routePacketSend);
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
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor,
  struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determin0e our neighbors
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
  }    // End of printdt0
