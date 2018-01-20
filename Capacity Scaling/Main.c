// Gautam Ramachandruni
// C program that uses capacity scaling to solve maximum flow problem
// code borrowed from http://ranger.uta.edu/~weems/NOTES5311/cse5311.html
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#include<sys/resource.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define oo 1000000000
int n;  // number of nodes
int residualEdges;  // number of edges in residual network
int maxcap = 0; // maximum capacity
int parameter; // capacity scaling parameter (Δ)
struct edge // declaring edge properties
{
  int tail, head;
  int capacity, flow;
  int inverse;
};
typedef struct edge edgeType;
edgeType *edgeTab;
int *firstEdge;  // table indicating first in range of edges with a common tail
int *color;      // needed for breadth-first search
int *pred;       // array to store augmenting path
int *predEdge;   // edgeTab subscript of edge used to reach vertex i in breadth-first search
float CPUtime()
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  return rusage.ru_utime.tv_sec+rusage.ru_utime.tv_usec/1000000.0 + rusage.ru_stime.tv_sec+rusage.ru_stime.tv_usec/1000000.0;
}
int min(int x, int y)
{
  return x < y ? x : y;  // returns minimum of x and y
}
// breadth-first search queue
int head, tail;
int *q;
void enqueue(int x)
{
  q[tail] = x;
  tail++;
  color[x] = GRAY;
}
int dequeue()
{
  int x = q[head];
  head++;
  color[x] = BLACK;
  return x;
}
// breadth-first search for an augmenting path
// searches for path from start to target
// returns 1 if found otherwise 0
int bfs(int start, int target)
{
  int u, v;
  int i;
  for(u = 0; u < n; u++)
    color[u] = WHITE;
  head = tail = 0;  // since q is not circular it is reinitialized for each breadth-first search
  enqueue(start);
  pred[start] = -1;
  while(head != tail)
  {
    u = dequeue();
    if(u==target)
      return 1;
    // search all adjacent white nodes v
    // if the residual capacity from u to v in the residual network is positive enqueue v
    for(i = firstEdge[u]; i < firstEdge[u+1]; i++)
    {
      v = edgeTab[i].head;
      if(color[v] == WHITE && edgeTab[i].capacity - edgeTab[i].flow > 0 && edgeTab[i].capacity - edgeTab[i].flow >= parameter)
      {
        enqueue(v);
        pred[v] = u;
        predEdge[v] = i;
      }
    }
  }
  // no augmenting path remains, so a maximum flow and minimum cut have been found
  // black vertices are in the source side (S) of the minimum cut while white vertices are in the sink side (T)
  return 0;
}
// Ford-Fulkerson Algorithm
int max_flow(int source, int sink)
{
  int i,j;
  int u;
  int max_flow;
  int APcount = 0;
  color = (int*)malloc(n*sizeof(int));
  pred = (int*)malloc(n*sizeof(int));
  predEdge = (int*)malloc(n*sizeof(int));
  q = (int*)malloc(n*sizeof(int));
  if(!color || !pred || !predEdge || !q)
  {
    printf("malloc failed %d\n",__LINE__);
    exit(0);
  }
  // initialize empty flow
  max_flow = 0;
  for(i = 0; i < residualEdges; i++)
    edgeTab[i].flow = 0;
    double exp = log((double)maxcap)/log(2); // computing capacity scaling parameter
    parameter = pow(2, (int)exp + 1);
  // while there exists an augmenting path increment the flow along this path
  while(parameter >= 1)
  {
    printf("\nThe capacity scaling parameter is: %d\n", parameter);
    while(bfs(source, sink))
    {
      // determine the amount by which we can increment the flow
      int increment = oo;
      APcount++;
      for(u = sink; pred[u]!= (-1); u = pred[u])
      {
        i = predEdge[u];
        increment = min(increment, edgeTab[i].capacity - edgeTab[i].flow);
      }
      // now increment the flow
      for(u=sink; pred[u]!= (-1); u = pred[u])
      {
        i = edgeTab[predEdge[u]].inverse;
        edgeTab[predEdge[u]].flow += increment;
        edgeTab[i].flow -= increment;  // reverse in residual
      }
      if(n<=1000)
      {
        // path trace
        for(u = sink; pred[u]!= (-1); u = pred[u])
        printf("%d<-", u);
        printf("%d adds %d incremental flow\n", source, increment);
      }
      max_flow += increment;
    }
    parameter = parameter / 2;
  }
  printf("%d augmenting paths\n", APcount);
  // no more augmenting paths so cut is based on reachability from last BFS
  if(n<=20)
  {
    printf("S side of min-cut:\n");
    for(u = 0; u < n; u++)
      if(color[u] == BLACK)
        printf("%d\n", u);
    printf("T side of min-cut:\n");
    for(u = 0; u < n; u++)
      if(color[u] == WHITE)
        printf("%d\n", u);
  }
  free(color);
  free(pred);
  free(predEdge);
  free(q);
  return max_flow;
}
// reading the input file and organize adjacency lists for residual network
int tailThenHead(const void* xin, const void* yin) // used in calls to qsort() and bsearch() for read_input_file()
{
  int result;
  edgeType *x,*y;
  x = (edgeType*) xin;
  y = (edgeType*) yin;
  result = x->tail - y->tail;
  if(result != 0)
    return result;
  else
    return x->head - y->head;
}
void dumpEdges(int count)
{
  int i;
  printf("i tail head cap\n");
  for(i = 0; i < count; i++)
    printf("%3d %3d  %3d %5d\n", i, edgeTab[i].tail, edgeTab[i].head, edgeTab[i].capacity);
}
void dumpFinal()
{
  int i;
  printf("Initialized residual network:\n");
  printf("Vertex firstEdge\n");
  for(i = 0; i < n; i++)
    printf(" %3d    %3d\n", i, firstEdge[i]);
  printf("=================\n");
  printf(" %3d    %3d\n", n, firstEdge[n]);
  printf("i tail head cap inv\n");
  for(i = 0; i < residualEdges; i++)
    printf("%3d %3d  %3d %5d  %3d\n", i, edgeTab[i].tail, edgeTab[i].head, edgeTab[i].capacity, edgeTab[i].inverse);
}
void read_input_file(FILE* file)
{
  int tail, head;
  int capacity;
  int i,j;
  int inputEdges;     // number of edges in input file.
  int workingEdges;   // number of residual network edges initially generated from input file
  edgeType work;
  edgeType *ptr;
  float startCPU,stopCPU;
  // table is allocated at worst-case size, since space for inverses is needed.
  edgeTab = (edgeType*)malloc(2*inputEdges*sizeof(edgeType));
  // read number of nodes and edges
  // taking file name as argument
  fscanf(file, "%d %d", &n, &inputEdges);
  if(!edgeTab)
  {
    printf("edgeTab malloc failed %d\n",__LINE__);
    exit(0);
  }
  // read edges each with a capacity
  workingEdges=0;
  for(i = 0; i < inputEdges; i++)
  {
    fscanf(file,"%d %d %d", &tail, &head, &capacity);
    // test for illegal edge including incoming to source and outgoing from sink to find the max capacity of the graph
    if(maxcap < capacity)
      maxcap = capacity;
    if(tail < 0 || tail >= n-1 || head < 1 || head >= n || capacity <= 0)
    {
      printf("Invalid input %d %d %d at %d\n",tail,head,capacity,__LINE__);
      exit(0);
    }
    // save input edge
    edgeTab[workingEdges].tail = tail;
    edgeTab[workingEdges].head = head;
    edgeTab[workingEdges].capacity = capacity;
    workingEdges++;
    // save inverse of input edge
    edgeTab[workingEdges].tail = head;
    edgeTab[workingEdges].head = tail;
    edgeTab[workingEdges].capacity = 0;
    workingEdges++;
  }
  if(n <= 20)
  {
    printf("Input & inverses:\n");
    dumpEdges(workingEdges);
  }
  // sort edges to make edges with common tail contiguous in edgeTab along with making parallel edges contiguous
  // a faster sort is unlikely to speed-up substantially
  startCPU = CPUtime();
  qsort(edgeTab, workingEdges, sizeof(edgeType), tailThenHead);
  stopCPU = CPUtime();
  printf("qsort CPU %f\n", stopCPU-startCPU);
  if(n <= 20)
  {
    printf("Sorted edges:\n");
    dumpEdges(workingEdges);
  }
  // coalesce parallel edges into a single edge by adding their capacities
  residualEdges=0;
  for(i = 1; i < workingEdges; i++)
    if(edgeTab[residualEdges].tail == edgeTab[i].tail &&  edgeTab[residualEdges].head == edgeTab[i].head)
      edgeTab[residualEdges].capacity += edgeTab[i].capacity;  // || case
    else
    {
      residualEdges++;
      edgeTab[residualEdges].tail = edgeTab[i].tail;
      edgeTab[residualEdges].head = edgeTab[i].head;
      edgeTab[residualEdges].capacity = edgeTab[i].capacity;
    }
  residualEdges++;
  if(n <= 20)
  {
    printf("Coalesced edges:\n");
    dumpEdges(residualEdges);
  }
  // set field in each edgeTab struct to point to inverse
  startCPU = CPUtime();
  for(i = 0; i < residualEdges; i++)
  {
    work.tail = edgeTab[i].head;
    work.head = edgeTab[i].tail;
    ptr = (edgeType*)bsearch(&work, edgeTab, residualEdges, sizeof(edgeType), tailThenHead);
    if (ptr == NULL)
    {
      printf("bsearch %d failed %d\n",i,__LINE__);
      exit(0);
    }
    edgeTab[i].inverse = ptr - edgeTab;  // ptr arithmetic to get subscript
  }
  stopCPU = CPUtime();
  printf("set inverses CPU %f\n", stopCPU - startCPU);
  // for each vertex i determine first edge in edgeTab with a tail >= i
  firstEdge = (int*)malloc((n + 1)*sizeof(int));
  if(!firstEdge)
  {
    printf("malloc failed %d\n",__LINE__);
    exit(0);
  }
  j=0;
  for(i = 0; i < n; i++)
  {
    firstEdge[i]=j;
    // skip over edges with vertex i as their tail
    for( ; j < residualEdges && edgeTab[j].tail == i; j++);
  }
  firstEdge[n] = residualEdges;  // sentinel
  if(n <= 20)
    dumpFinal();
}
int main(int argc, char* argv[])
{
  int i,j;
  float startCPU, stopCPU;
  FILE* file = fopen(argv[1], "r");
  if(!file)
  {
    printf("Invalid input file!\n");
    exit(0);
  }
  read_input_file(file);
  startCPU = CPUtime();
  printf("total flow is %d\n", max_flow(0, n - 1));  // 0 = source, n - 1 = sink
  stopCPU = CPUtime();
  printf("Ford-Fulkerson time %f\n", stopCPU - startCPU);
  free(edgeTab);
  free(firstEdge);
}
