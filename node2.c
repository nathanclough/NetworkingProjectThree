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

int neighbors2 [3] = {0,1,3};
struct distance_table 
{
  int costs[4][4];
} dt2;


/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
  // Initialize to infinity
  for(int i = 0; i<4; i++){
    for( int j = 0; j< 4; j++){
        dt2.costs[i][j] = 999;
      }
  }

  // Set known neighbors 
  dt2.costs[0][0] = 3;
  dt2.costs[1][1] = 1;
  dt2.costs[2][2] = 0;
  dt2.costs[3][3] = 2;

  printdt2(&dt2);

  // Update neighbors with the initial state 
  int minCost[4] = {999,999,999,999};
  getMinCost(minCost,dt2.costs);
  for(int i = 0; i<sizeof(neighbors2)/sizeof(neighbors2[1]); i++)
      {
        printf("Updating node %d\n",i);
        sendUpdate(neighbors2[i],minCost,2);
      } 

}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  int update = 0;
  int currentMinCosts [4] = {999,999,999,999};
  int src = rcvdpkt->sourceid;
  getMinCost(currentMinCosts,dt2.costs);

 for(int i = 0; i<4; i++){
    // Update costs based on recieved packet 
    int newCost = rcvdpkt->mincost[i] + currentMinCosts[src];
    if(newCost < dt2.costs[i][src]){
      dt2.costs[i][src] = newCost;
        // Check if after the update we have a new minimum
      if( dt2.costs[i][src] < currentMinCosts[i]){
        update = 1;
        currentMinCosts[i] = dt2.costs[i][src];
      }
    }
  }

  printdt2(&dt2);
  printf("Current min costs (dest,cost): \n");
  for(int i = 0; i< 4; i++){
    printf("(%d,%d) \n",i, currentMinCosts[i]);
  }

  
  // Update if there was a new shortest path 
  if(update){
    for(int i = 0; i<sizeof(neighbors2)/sizeof(neighbors2[1]); i++)
      {
        printf("Updating node %d\n",i);
        sendUpdate(neighbors2[i],currentMinCosts,2);
      }
  }  
}


printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







