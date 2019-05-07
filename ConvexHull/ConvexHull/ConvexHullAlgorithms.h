#include <vector>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <bits/stdc++.h> 
#include <set>
#define iPair std::pair<int,int>

using namespace std;

struct Point 
{ 
    int x, y; 
}; 

// ----------------------------------------------
// | Utility functions for different algorithms |
// ----------------------------------------------

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clock or counterclock wise 
}; 

// A global point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
Point p0;

// A utility function to find next to top in a stack
Point nextToTop(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

// A utility function to swap two points
int swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;

   // Find orientation
   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

   return (o == 2)? -1: 1;
}

// Returns side of a point that joins p1 and p2
int findSide(iPair p1, iPair p2, iPair p) {
  int val = (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
  if (val > 0) return 1;
  if (val < 0) return -1;
  return 0;
}

// Return value proportional to distance between p and the line 
// connecting p1 and p2
int lineDist(iPair p1, iPair p2, iPair p) {
  return abs((p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first));
}


// Graham Scan
// A C++ program to find convex hull of a set of points. Refer
// https://www.geeksforgeeks.org/orientation-3-ordered-points/
// for explanation of orientation()

// Prints convex hull of a set of n points via Graham Scan
void grahamConvexHull(vector<Point> points, int n, string outputFile)
{   
    
   // Find the bottommost point
   int ymin = points[0].y, min = 0;
   for (int i = 1; i < n; i++)
   {
     int y = points[i].y;

     // Pick the bottom-most or chose the left
     // most point in case of tie
     if ((y < ymin) || (ymin == y &&
         points[i].x < points[min].x))
        ymin = points[i].y, min = i;
   }

   // Place the bottom-most point at first position
   swap(points[0], points[min]);

   // Sort n-1 points with respect to the first point.
   // A point p1 comes before p2 in sorted ouput if p2
   // has larger polar angle (in counterclockwise
   // direction) than p1
   p0 = points[0];
   qsort(&points[1], n-1, sizeof(Point), compare);

   // If two or more points make same angle with p0,
   // Remove all but the one that is farthest from p0
   // Remember that, in above sorting, our criteria was
   // to keep the farthest point at the end when more than
   // one points have same angle.
   int m = 1; // Initialize size of modified array
   for (int i=1; i<n; i++)
   {
       // Keep removing i while angle of i and i+1 is same
       // with respect to p0
       while (i < n-1 && orientation(p0, points[i],
                                    points[i+1]) == 0)
          i++;


       points[m] = points[i];
       m++;  // Update size of modified array
   }

   // If modified array of points has less than 3 points,
   // convex hull is not possible
   if (m < 3)
   {
    cout << "Convex Hull not possible!" << endl;
    return;   
   } 

   // Create an empty stack and push first three points
   // to it.
   stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);

   // Process remaining n-3 points
   for (int i = 3; i < m; i++)
   {
      // Keep removing top while the angle formed by
      // points next-to-top, top, and points[i] makes
      // a non-left turn
      while (orientation(nextToTop(S), S.top(), points[i]) != 2)
         S.pop();
      S.push(points[i]);
   }

   // Now stack has the output points, print contents of stack and output to file
   std::ofstream file;
   file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   while (!S.empty())
   {
       Point p = S.top();
       file << p.x << "\t" << p.y << "\n"; 
       cout << "(" << p.x << ", " << p.y <<")" << endl;
       S.pop();
   }
   file.close();
   
}

// Jarvis March
// A C++ program to find convex hull of a set of points. Refer 
// https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
// for explanation of orientation() 


// Prints convex hull of a set of n points via Jarvis March 
void jarvisConvexHull(vector<Point> points, int n, string outputFile) 
{ 
    // There must be at least 3 points 
    if (n < 3) return; 
  
    // Initialize Result 
    vector<Point> hull; 
  
    // Find the leftmost point 
    int l = 0; 
    for (int i = 1; i < n; i++) 
        if (points[i].x < points[l].x) 
            l = i; 
  
    // Start from leftmost point, keep moving counterclockwise 
    // until reach the start point again.  This loop runs O(h) 
    // times where h is number of points in result or output. 
    int p = l, q; 
    do
    { 
        // Add current point to result 
        hull.push_back(points[p]); 
  
        // Search for a point 'q' such that orientation(p, x, 
        // q) is counterclockwise for all points 'x'. The idea 
        // is to keep track of last visited most counterclock- 
        // wise point in q. If any point 'i' is more counterclock- 
        // wise than q, then update q. 
        q = (p+1)%n; 
        for (int i = 0; i < n; i++) 
        { 
           // If i is more counterclockwise than current q, then 
           // update q 
           if (orientation(points[p], points[i], points[q]) == 2) 
               q = i; 
        } 
  
        // Now q is the most counterclockwise with respect to p 
        // Set p as q for next iteration, so that q is added to 
        // result 'hull' 
        p = q; 
  
    } while (p != l);  // While we don't come to first point 
  
    // Print Result and output to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc); 
    for (int i = 0; i < hull.size(); i++) 
    {
        file << hull[i].x << "\t" << hull[i].y << "\n";   
        cout << "(" << hull[i].x << ", "
              << hull[i].y << ")\n";
    }
    file.close(); 
}

// QuickHull

set<iPair> quick_hull;
vector<Point> quickHullVector;


// Recursive helper for Quickhull function
void hullHelper(std::vector<iPair> points, int n, iPair p1, iPair p2, int side) {
  int index = -1;
  int max_dist = 0;

  for (int i = 0; i < n; i++) {
    int temp = lineDist(p1, p2, points[i]);
    if (findSide(p1, p2, points[i]) == side && temp > max_dist) {
      index = i;
      max_dist = temp;
    }
  }

  if (index == -1) {
    quick_hull.insert(p1);
    quick_hull.insert(p2);
    return;
  }

  // recurse twice with smaller sets of points 
  hullHelper(points, n, points[index], p1, -findSide(points[index], p1, p2));
  hullHelper(points, n, points[index], p2, -findSide(points[index], p2, p1));
}

// Find a convex hull using Quickhull
void quickHull(std::vector<iPair> points, int n, std::string outputFile) {
  if (n < 3) {
    std::cout << "Must be more than 3 points" << std::endl;
    return;
  }

  // Find the largest and smallest X values
  int minX = 0, maxX = 0;
  for (int i = 1; i < n; i++) {
    if (points[i].first < points[minX].first)
      minX = i;
    if (points[i].first > points[maxX].first)
      maxX = i;
  }

  // Fun recursive function on each side of the dataset
  hullHelper(points, n, points[minX], points[maxX], 1);
  hullHelper(points, n, points[minX], points[maxX], -1);

  // Hull is done, now put in order to draw it right
  // **This is a time consuming process, and can skew results on hulls with many points**
  std::set<iPair>::iterator it;
  for (it = quick_hull.begin(); it != quick_hull.end(); ++it) {
      iPair point = *it;
      Point myPoint;
      myPoint.x = point.first;
      myPoint.y = point.second;
      quickHullVector.push_back(myPoint);
  }

  int ymin = quickHullVector[0].y, min = 0;

  // Go through all points and find one with least y value
  for (int i = 1; i < quickHullVector.size(); i++) {
    int y = quickHullVector[i].y;

    if ((y < ymin) || (ymin == y && quickHullVector[i].x < quickHullVector[min].x))
      ymin = quickHullVector[i].y, min = i;
  }

  // Set p0 to the lowest point we just found, then sort the rest of them
  swap(quickHullVector[0], quickHullVector[min]);
  p0 = quickHullVector[0];

  // Sort newly converted vector of points, just like beginning of graham scan
  qsort(&quickHullVector[1], quickHullVector.size()-1, sizeof(Point), compare);
  
  // Export hull to file
  std::ofstream file;
  file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
  for (int i = 0; i < quickHullVector.size(); i++) {
    cout << "(" << quickHullVector[i].x << ", " << quickHullVector[i].y <<")" << endl;
    file << quickHullVector[i].x << "\t" << quickHullVector[i].y << "\n";
  }
  file.close();
}
