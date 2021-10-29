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

struct distance_table 
{
  int costs[4][4];
} dt0;

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
  // Initialize all costs to inf
  for(int i = 0; i<4; i++){
    for( int j = 0; j< 4; j++){
        dt0.costs[i][j] = 999;
      }
    }
    
  dt0.costs[0][0] = 0;
  dt0.costs[1][1] = 1;
  dt0.costs[2][2] = 3;
  dt0.costs[3][3] = 7;

  printdt0(&dt0);
  int minCost[4] = {999,999,999,999};
  getMinCost(minCost,dt0.costs);

   for( int i = 1; i<4; i++){
     printf("Updating node %d\n",i);
     sendUpdate(i,minCost,0);
   }
}

void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
   // First update from 0 is [0,1,3,7]
  int update = 0;
  int currentMinCosts [4] = {999,999,999,999};
  int src = rcvdpkt->sourceid;
  getMinCost(currentMinCosts,dt0.costs);

  for(int i = 0; i<4; i++){
    // Update costs based on recieved packet 
    int newCost = rcvdpkt->mincost[i] + currentMinCosts[src];
    if(newCost < dt0.costs[i][src]){
      dt0.costs[i][src] = newCost;
        // Check if after the update we have a new minimum
      if( dt0.costs[i][src] < currentMinCosts[i]){
        update = 1;
        currentMinCosts[i] = dt0.costs[i][src];
      }
    }
  }

  // for each node 
  printdt0(&dt0);
  if(update){
    for(int i = 1; i<4; i++)
      {
        printf("Updating node %d\n",i);
        sendUpdate(i,currentMinCosts,0);
      }
  }
}


void printdt0(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
}

