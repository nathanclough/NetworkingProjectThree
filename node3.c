#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;
int neighbors3[2] = {0,2};

struct distance_table 
{
  int costs[4][4];
} dt3;

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  // Initialize to infinity
  for(int i = 0; i<4; i++){
    for( int j = 0; j< 4; j++){
        dt3.costs[i][j] = 999;
      }
  }

  // Set known neighbors 
  dt3.costs[0][0] = 7;
  dt3.costs[1][1] = 999;
  dt3.costs[2][2] = 2;
  dt3.costs[3][3] = 0;
  printdt3(&dt3);

  // Update neighbors with the initial state 
  int minCost[4] = {999,999,999,999};
  getMinCost(minCost,dt3.costs);
  
  for(int i = 0; i<sizeof(neighbors3)/sizeof(neighbors3[1]); i++)
  {
    printf("Updating node %d\n",i);
    sendUpdate(neighbors3[i],minCost,3);
  } 
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
 // First update from 0 is [0,1,3,7]
  int update = 0;
  int currentMinCosts [4] = {999,999,999,999};
  int src = rcvdpkt->sourceid;
  getMinCost(currentMinCosts,dt3.costs);

 for(int i = 0; i<4; i++){
    // Update costs based on recieved packet 
    int newCost = rcvdpkt->mincost[i] + currentMinCosts[src];
    if(newCost < dt3.costs[i][src]){
      dt3.costs[i][src] = newCost;
        // Check if after the update we have a new minimum
      if( dt3.costs[i][src] < currentMinCosts[i]){
        update = 1;
        currentMinCosts[i] = dt3.costs[i][src];
      }
    }
  }

  // for each node 
  printdt3(&dt3);
  printf("Current min costs (dest,cost): \n");
  for(int i = 0; i< 4; i++){
    printf("(%d,%d) \n",i, currentMinCosts[i]);
  }

  if(update){
    for(int i = 0; i<sizeof(neighbors3)/sizeof(neighbors3[1]); i++)
      {
        printf("Updating node %d\n",i);
        sendUpdate(neighbors3[i],currentMinCosts,3);
      }
  }


}


printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







