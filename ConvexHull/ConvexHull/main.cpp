// You need to complete this program for your second project.

#include "ConvexHullAlgorithms.h"
// Standard libraries
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <ctime>


int main(int argc, char *argv[])
{
   //Second project starts here
   if (argc < 3)
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];
      std::string outputFile = "";
      vector<iPair> quickHullPoints;
      Point point;
      vector<Point> pointVec;

      //read your data points from dataFile (see class example for the format)
      ifstream dataFile;
      dataFile.open(dataFilename);
      while(!dataFile.eof())
      {
        dataFile >> point.x >> point.y;
        pointVec.push_back(point);
        quickHullPoints.push_back(*new iPair(point.x,point.y));
      }
      int n = pointVec.size();
     






      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
         cout << "Graham Scan" << endl;
         clock_t start = clock();
         grahamConvexHull(pointVec , n, "hull_G.txt");
         double duration = (clock() - start)/double(CLOCKS_PER_SEC);
         cout << "Graham scan on " << dataFilename << " took " << duration*1000 << " ms" << endl;
         
      }
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         cout << "Jarvis March" << endl;
         clock_t start = clock();
         jarvisConvexHull(pointVec, n, "hull_J.txt");
         double duration = (clock() - start)/double(CLOCKS_PER_SEC);
         cout << "Jarvis March on " << dataFilename << " took " << duration*1000 << " ms" << endl;
      }
      else { //default
         cout << "Quick Hull" << endl;
         clock_t start = clock();
         quickHull(quickHullPoints, n, "hull_Q.txt");
         double duration = (clock() - start)/double(CLOCKS_PER_SEC);
         cout << "Quick Hull on " << dataFilename << " took " << duration*1000 << " ms" << endl;
         
      }

      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.

	return 0;
}
}
