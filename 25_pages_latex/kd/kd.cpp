#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <queue>

using namespace std;

// Set the number of dimensions you need for you points
#define NDIM 3

struct point_t{
  double x[NDIM];  // co-ordinates of the point
  double distance; // distance is used by the priority queue
};

// This comparison operator is used by the priority queue
struct pointcompare{
  bool operator()(const point_t &lhs, const point_t &rhs) const{
    return lhs.distance < rhs.distance;
  }
};

// The point with the GREATEST distance value is at the head of the
// priority queue.
typedef vector<point_t> point_vec;
typedef priority_queue<point_t, point_vec, pointcompare > pointQ_t;
// node_t is a node in the K-D tree 
struct node_t{
  point_t *p; // for a leaf, p refers to the point contained in the leaf
  double val; // if internal node, the value that dim is split at
  int dim;    // if internal node, the dimension that it splits on
  struct node_t *left, *right; // I love children!!
};

// Utility function to swap two points
void swap(point_t *l, point_t *r)
{
  point_t tmp;
  tmp = *l;
  *l = *r;
  *r = tmp;
}

// Utility function to get the squared distance between points a and b
double distsq(point_t *a,point_t *b, int dim)
{
  double t, d = 0;
  do
    {
      --dim;
      t = a->x[dim] - b->x[dim];
      d += t * t;
    }
  while (dim > 0);
  return d;
}

/* The partition function is a variation on quicksort. It sorts the
   array on dimension 'dim' until everything between index 'lm' and
   index 'hm' are sorted, but does not care about anything outside
   that range. This lets it stop much earlier than if it sorted the
   whole array.  Uses Dijkstra's 3-way pivot.  Dual-pivot would
   probably work better, but it takes more code.
*/
void partition(point_t *a, int left, int right, int dim, int lm, int hm)
{
  if(right <= left)
    return;
  else
    {
      int i = left;
      int lt = left;
      int gt = right;
      double pivot = a[left].x[dim];
      do
	{
	  if(a[i].x[dim] < pivot)
	    swap(a+lt++, a+i++);
	  else
	    if(a[i].x[dim] > pivot)
	      swap(a+i, a+gt--);
	    else
	      i++;
	}
      while(i<=gt);
      if((lt >= lm)) // only recurse if some are between lm and hm
	partition(a,left,lt-1,dim,lm,hm);
      if((gt <= hm)) // only recurse if some are between lm and hm
	partition(a,gt+1,right,dim,lm,hm);
    }
}

/* Builds a K-D tree using the (possibly unsorted) points in array a,
   starting at left and ending at right. It first partitions the
   points so that the median value(s) are in the middle, then splits
   at the median.  If there are an even number of points, then two
   points are used to calculate the median.  Otherwise, a single point
   is used. The bounding box for each node is calculated and
   saved. The initial bounds should be -MAXDOUBLE for Bmin[0] throungh
   Bmin[NDIM-1] and MAXDOUBLE for Bmax[0] throung Bmax[NDIM-1].
*/
node_t *make_tree(point_t *a,int left, int right, int dim)
{
  node_t *n;
  int lm,rm;
  double median;
  
  if(left>right) /* degenerate case -- no points in the array */
    n = NULL;
  else
    {
      /* allocate a node and check for error */
      if((n = (node_t*)malloc(sizeof(node_t)))==NULL)
	exit(5);
      if(left == right)
	{ /* leaf node -- one point in the array */
	  n->left=n->right=NULL;
	  n->dim=-1;
	  n->val=0.0;
	  n->p = a+left;
	}
      else
	{ /* interior node -- split at median on dimension dim */
	  lm = (right+left)/2;
	  rm = (right+left+1)/2;
	  partition(a,left,right,dim,lm,rm);
	  median = (a[lm].x[dim]+a[rm].x[dim])/2.0;
	  n->p = NULL;
	  n->dim = dim;
	  n->val = median;
	  dim = (dim+1) % NDIM;
	  n->left = make_tree(a,left,lm,dim);
	  if(lm<rm) /* the tree may lean to the left a bit */
	    n->right = make_tree(a,rm,right,dim);
	  else
	    if(rm<right)
	      n->right = make_tree(a,rm+1,right,dim);
	}
    }
  return n;
}

/* k_nearest creates a priority queue (pq) contianing the k nearest
   neighbors to point P, with the farthest point from P at the head of
   the queue.  If first uses recursion to find the leaf node that
   contains point P, then starts recursively searching other promising
   branches as it unwinds the stack.  A branch is promising if it
   could contain a point that is closer than the current worst point.
   If 'selfmatch' is 0, then any point matching P will be excluded.
*/
void k_nearest(node_t *n, point_t *P,pointQ_t &pq,unsigned k,int selfmatch)
{
  // If we don't have k points, then add some points that are at
  // infinite distance.  Those points will be replaced as we find
  // closer ones, or we will end up with some infinite points in the
  // queue.  You may need to check the points that are returned.
  if(pq.size() < k)
    { point_t tmp;
      for(int i=0;i<NDIM;i++)
	tmp.x[i] = INFINITY;
      tmp.distance = INFINITY;
      while(pq.size() < k)
	pq.push(tmp);
    }
  
  if(n->p != NULL) // if n is a leaf node
    {
      n->p->distance = distsq(n->p,P,NDIM); // get distance to P
      /* If the new point is closer than head of queue, then replace */
      if(((n->p->distance > 0.0)||selfmatch) && // may want to exclude P
	 (n->p->distance < pq.top().distance))
	{
	  pq.pop();
	  pq.push(*(n->p));
	}
    }
  else
    {
      if(n->val >= P->x[n->dim])
	{ // Left appears closer.  Go that way first.
	  k_nearest(n->left,P,pq,k,selfmatch);
	  // Now check to see if we need to go right (distance from
	  // P to split plane is closer than distance from P to
	  // worst point in the set). 
         if(pow(n->val - P->x[n->dim],2) <= pq.top().distance)
	   k_nearest(n->right,P,pq,k,selfmatch);
	}
      else
	{ // Right appears closer.  Go that way first.
	  k_nearest(n->right,P,pq,k,selfmatch);
	  // Now check to see if we need to go left (distance from
	  // P to split plane is closer than distance from P to
	  // worst point in the set).
	  if(pow(n->val - P->x[n->dim],2) <= pq.top().distance)
	    k_nearest(n->left,P,pq,k,selfmatch);
	}
    }
}

/* find_in_sphere finds all points that lie within the hypersphere
   given by P (the center) and r (the radius).  Points are returned in
   a priority queue 'qt' with the farthest point from P at the head of
   the queue.  If first uses recursion to find the leaf node that
   contains point P, then starts recursively searching other promising
   branches as it unwinds the stack.  A branch is promising if it
   could contain a point that lies within the sphere.  If 'selfmatch'
   is 0, then any point matching P will be excluded.
*/
void find_in_sphere(node_t *n,point_t *P,double radius,
		    pointQ_t &pq,int selfmatch)
{
  if(n->p != NULL) // if n is a leaf node
    { /* If the point is within sphere, then add to queue */
      n->p->distance = distsq(n->p,P,NDIM); // get distance to P
      if(((n->p->distance > 0.0)||selfmatch) && // may want to exclude P
	 (n->p->distance <= radius*radius)) // on surface == inside
	pq.push(*(n->p));
    }
  else
    {
      if(n->val >= P->x[n->dim])
	{ // Left appears closer.  Go that way first.
	  find_in_sphere(n->left,P,radius,pq,selfmatch);
	  // Now check to see if we need to go right (distance from
	  // P to split plane is less or equal to radius). 
	  if(pow(n->val - P->x[n->dim],2) <= radius*radius)
	    find_in_sphere(n->right,P,radius,pq,selfmatch);
	}
      else
	{ // Right appears closer.  Go that way first.
	  find_in_sphere(n->right,P,radius,pq,selfmatch);
	  // Now check to see if we need to go left (distance from
	  // P to split plane is closer than distance from P to
	  // worst point in the set).
	  if(pow(n->val - P->x[n->dim],2) <= radius*radius)
	    find_in_sphere(n->left,P,radius,pq,selfmatch);
	}
    }
}

/* find_in_range finds all points that lie within the axis-aligned
   hypercube given by Bmin (the corner closest to the origin) and Bmax
   (the corner farthest from the origin).  Points are returned in
   vector If first uses recursion to find the leaf node that contains
   some point near the box, then starts recursively searching other
   promising branches as it unwinds the stack.  A branch is promising
   if it could contain a point that lies within the box.
*/
void find_in_range(node_t *n,point_t &Bmin, point_t &Bmax,pointQ_t &pq)
{
  // if(n->p != NULL) // if n is a leaf node
  //   { /* If the point is within sphere, then add to queue */
  //     n->p->distance = distsq(n->p,P,NDIM); // get distance to P
  //     if(((n->p->distance > 0.0)||selfmatch) && // may want to exclude P
  // 	 (n->p->distance <= radius*radius)) // on surface == inside
  // 	pq.push(*(n->p));
  //   }
  // else
  //   {
  //     if(n->val >= P->x[n->dim])
  // 	{ // Left appears closer.  Go that way first.
  // 	  k_nearest(n->left,P,pq,k,selfmatch);
  // 	  // Now check to see if we need to go right (distance from
  // 	  // P to split plane is less or equal to radius). 
  // 	  if(pow(n->val - P->x[n->dim],2) <= radius*radius)
  // 	    k_nearest(n->right,P,pq,k,selfmatch);
  // 	}
  //     else
  // 	{ // Right appears closer.  Go that way first.
  // 	  k_nearest(n->right,P,pq,k,selfmatch);
  // 	  // Now check to see if we need to go left (distance from
  // 	  // P to split plane is closer than distance from P to
  // 	  // worst point in the set).
  // 	  if(pow(n->val - P->x[n->dim],2) <= radius*radius)
  // 	    k_nearest(n->left,P,pq,k,selfmatch);
  // 	}
  //   }
}

// Utility function to print a point
void printpoint(const point_t *p,int newline)
{
  int i;
  printf("%lf",p->x[0]);
  for(i=1;i<NDIM;i++)
    printf(" %lf",p->x[i]);
if(newline)
  printf("\n");
}


// Utility function to get a random point
void rand_pt(point_t *v)
{
  int i;
  for(i=0;i<NDIM;i++)
    v->x[i] = (drand48()-0.5)*100000;
}


