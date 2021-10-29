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

int connectcosts1[4] = { 1,  0,  1, 999 };
int neighbors[2] = {0,2};

struct distance_table 
{
  int costs[4][4];
} dt1;

/* students to write the following two routines, and maybe some others */


rtinit1() 
{
  // Initialize all costs to inf
  for(int i = 0; i<4; i++){
    for( int j = 0; j< 4; j++){
        dt1.costs[i][j] = 999;
      }
    }

  // Add initial costs 
  dt1.costs[0][0] = 1;
  dt1.costs[1][1] = 0;
  dt1.costs[2][2] = 1;
  dt1.costs[3][3] = 999;
  printdt1(&dt1);
  int minCost[4] = {999,999,999,999};
  getMinCost(minCost,dt1.costs);

  printf("Updating node 0\n");
  sendUpdate(0,minCost,1);
  printf("Updating node 2\n");
  sendUpdate(2,minCost,1);
}

rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  // First update from 0 is [0,1,3,7]
  int update = 0;
  int currentMinCosts [4] = {999,999,999,999};
  int src = rcvdpkt->sourceid;
  getMinCost(currentMinCosts,dt1.costs);

 for(int i = 0; i<4; i++){
    // Update costs based on recieved packet 
    int newCost = rcvdpkt->mincost[i] + currentMinCosts[src];
    if(newCost < dt1.costs[i][src]){
      dt1.costs[i][src] = newCost;
        // Check if after the update we have a new minimum
      if( dt1.costs[i][src] < currentMinCosts[i]){
        update = 1;
        currentMinCosts[i] = dt1.costs[i][src];
      }
    }
  }

  // for each node 
  printdt1(&dt1);
  printf("Current min costs (dest,cost): \n");
  for(int i = 0; i< 4; i++){
    printf("(%d,%d) \n",i, currentMinCosts[i]);
  }

  if(update){
    for(int i = 0; i<sizeof(neighbors)/sizeof(neighbors[1]); i++)
      {
        printf("Updating node %d\n",i);
        sendUpdate(neighbors[i],currentMinCosts,1);
      }
  }


}

int printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



linkhandler1(linkid, newcost)   
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}


