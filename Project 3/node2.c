#include <stdio.h>
#include <stdlib.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

struct distance_table dt2;
struct NeighborCosts   *neighbor2;
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

int i,j,k,min2,source_id2,isChanged2,oldcost2,newcost2,initCost2[MAX_NODES];

// initialize the table
void blankTable2(){
  // set all values to 9999
  for( i = 0; i < MAX_NODES; i++ ){
    for( j = 0; j < MAX_NODES; j++ ){
      dt2.costs[i][j] = INFINITY;
    }
    // set costs to neighbor respectively
    neighbor2 = getNeighborCosts(2);
  }
}

// fill the table
void fillTable2(){
  for( i = 0; i < neighbor2->NodesInNetwork; i++ ) {
    dt2.costs[i][i] = neighbor2->NodeCosts[i];
    initCost2[i] = neighbor2->NodeCosts[i];
  }
}


void findmin2(struct RoutePacket *routePacketSend){
  for( j = 0; j < MAX_NODES; j++ ) {
    min2 = INFINITY;
    for( k = 0; k < MAX_NODES; k++ ){
      if( dt2.costs[j][k] < min2 ){
        min2 = dt2.costs[j][k];
      }
      routePacketSend->mincost[j] = min2;
    }
    printf("%d ", min2);
  }
}

// calls the table
void callTableName2(char *table){
  printf("At time t=%f, %s called.\n", clocktime, table);
}

// print sending information
void printSend2(int i, char *node){
  printf("At time t=%f, %s sends packet to node%d with: ", clocktime, node, i);
}

void processData2(struct RoutePacket *routePacketSend){
  // update data
  routePacketSend->sourceid = 2;
  routePacketSend->destid = i;
}


//update min
void updateMin2(int i, struct RoutePacket *rcvdpkt){
  // checks if update needed
  oldcost2 = dt2.costs[i][source_id2];
  newcost2 = dt2.costs[source_id2][source_id2] + rcvdpkt->mincost[i];
  // update new minimum
  if( oldcost2 > newcost2 ) {
    dt2.costs[i][source_id2] = newcost2;
    isChanged2 = YES;
  }
}


void rtinit2() {
  // create blank table with nodes
  blankTable2();
  // fill the table with cost values
  fillTable2();
  // call table name
  callTableName2("rtinit2()");
  // print table
  printdt2(2, neighbor2, &dt2);
  // process data in struct
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));
  // start sending nodes
  for( i = 0; i < neighbor2->NodesInNetwork; i++ ) {

    // ignore sending self
    if(( i == 2 ) || ( initCost2[i] == INFINITY )){
      continue;
    }

    // print node time
    printSend2(i, "node2");
    // process the data
    processData2(routePacketSend);
    // find the min
    findmin2(routePacketSend);
    // send min to layer 2
    toLayer2(*routePacketSend);
    printf("\n");
  }
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
  // process received data
  source_id2 = rcvdpkt->sourceid;
  isChanged2 = NO;
  struct RoutePacket *routePacketSend = malloc(sizeof(struct RoutePacket));

  for(i=0 ; i < MAX_NODES; i++ ) {
    // skip self
    if( i == 2 ){
      continue;
    }
    updateMin2(i,rcvdpkt);
  }
  // send changed nodes
  if( isChanged2 == YES ) {
    // print information
    callTableName2("rtupdate2()");
    // print sender
    printf("Sender id: %d",source_id2);
    // print table
    printdt2(2, neighbor2, &dt2);
    // prepare to send to layer 2
    for( i = 0; i < neighbor2->NodesInNetwork; i++ ) {
      // skip self and not linked
      if(( i == 2 ) || ( initCost2[i] == INFINITY )){
        continue;
      }
      printSend2(i,"node 2");
      // process data
      processData2(routePacketSend);
      // find min2
      findmin2(routePacketSend);
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
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor,
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
            printf("dest %d|", i);
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                  printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt2
