// Justin Gnatiuk
// Algorithms 
// Project 3: Seam Carving


#include <algorithm>
#include <stdlib.h>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <bits/stdc++.h>
#include <utility>

// Utility Functions

// Print matrix
void printMatrix(std::vector<std::vector<int> > &matrix, std::string name) 
{
    int height = matrix.size();
    int width = matrix[0].size();
    std::cout << name << std::endl;
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Size the matrix based on input image width and height
void buildMatrix(std::vector<std::vector<int> > &matrix, int width, int height) 
{
    matrix.resize(height);
    for (int i = 0; i < height; i++) 
    {
        matrix[i].resize(width);
    }
}

// Transpose the matrix
std::vector<std::vector<int> > transpose(std::vector<std::vector<int> > &matrix) 
{
    int width = matrix[0].size();
    int height = matrix.size();

    std::vector<std::vector<int> > transposedMatrix;
    buildMatrix(transposedMatrix, height, width);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            transposedMatrix[j][i] = matrix[i][j];
    return transposedMatrix;
}


// MinFind Functions


// Returns the value of the cell with the smallest values of the 3 above the given point
int minFromAbove(std::vector<std::vector<int> > &cumulative, int i, int j) 
{
    int min = INT_MAX;

    if (j != 0)
        min = cumulative[i-1][j-1];
    if (cumulative[i-1][j] < min)
        min = cumulative[i-1][j];
    if (j != cumulative[i].size() - 1)
        if (cumulative[i-1][j+1] < min)
            min = cumulative[i-1][j+1];
    
    return min;
}

// Returns the coordinates in the cumulative energy matrix of the least of the 3 entries above the number at i, j
std::pair<int, int> minCoodsFromAbove(std::vector<std::vector<int> > &cumulative, int i, int j) 
{
    std::pair<int, int> min;
    min.first = i - 1;
    // There will always be an element above this one, so default min j to it just in case
    min.second = j;
    if (j != 0)
        min.second = j - 1;
    if (cumulative[i-1][j] < cumulative[min.first][min.second])
        min.second = j;
    if (j != cumulative[i].size() - 1)
        if (cumulative[i-1][j+1] < cumulative[min.first][min.second])
            min.second = j + 1;

    return min;
}

// Returns the coordinates in the cumulative energy matrix of the minimum value in one row
std::pair<int, int> minCoords(std::vector<std::vector<int> > &cumulative, int i) 
{
    std::pair<int, int> min;
    min.first = i;
    min.second = 0;
    for (int j = 1; j < cumulative[i].size(); j++)
        if (cumulative[i][j] < cumulative[i][min.second])
            min.second = j;

    return min;
}


// Matrix Calculation Functions


// Calculate the energy matrix
std::vector<std::vector<int> > calculateEnergyMatrix(std::vector<std::vector<int> > &matrix) 
{   
    int left, right, top, bottom;
    int height = matrix.size();
    int width = matrix[0].size();
    std::vector<std::vector<int> > energyMatrix;
    buildMatrix(energyMatrix, width, height);
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if (j == 0) left = 0; 
            else left = abs(matrix[i][j] - matrix[i][j-1]);;

            if (j == width - 1) right = 0;
            else right = abs(matrix[i][j] - matrix[i][j+1]);

            if (i == 0) top = 0;
            else top = abs(matrix[i][j] - matrix[i-1][j]);

            if (i == height - 1) bottom = 0;
            else bottom = abs(matrix[i][j] - matrix[i+1][j]);

            energyMatrix[i][j] = left + right + top + bottom;
        }
    }

    return energyMatrix;
}

// Calculate cumulative matrix
std::vector<std::vector<int> > calculateCumulativeMatrix(std::vector<std::vector<int> > &energyMatrix) 
{

    int height = energyMatrix.size();
    int width = energyMatrix[0].size();
    std::vector<std::vector<int> > cumulativeMatrix;
    
    buildMatrix(cumulativeMatrix, width, height);

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if (i == 0) 
            {
                cumulativeMatrix[i][j] = energyMatrix[i][j];  
            }
            else 
            {
                cumulativeMatrix[i][j] = energyMatrix[i][j] + minFromAbove(cumulativeMatrix, i, j);     
            }
        }
    }

    return cumulativeMatrix;
}

// Trace up from bottom of the matrix and find path of the lowest numbers
std::vector<std::pair<int, int> > findMinPath(std::vector<std::vector<int> > &cumulativeMatrix) 
{
    int height = cumulativeMatrix.size();
    std::vector<std::pair<int, int> > path;
    path.push_back(minCoords(cumulativeMatrix, height - 1));
    for (int i = height - 1; i > 0; i--)
        path.push_back(minCoodsFromAbove(cumulativeMatrix, path.back().first, path.back().second));

    return path;
}

// Remove elements in the minPath
void removeMinPath(std::vector<std::vector<int> > &matrix, std::vector<std::pair<int, int> > &minPath) 
{
    for (int i = 0; i < minPath.size(); i++)
        matrix[minPath[i].first].erase(matrix[minPath[i].first].begin() + minPath[i].second);
}

// Function to remove Seams
void removeSeams(std::vector<std::vector<int> > &matrix, int seams) 
{
    std::vector<std::vector<int> > energyMatrix, cumulativeMatrix;
    std::vector<std::pair<int, int> > minPath;

    // iterate through and remove each seam
    for (int count = 0; count < seams; count++) 
    {
        energyMatrix = calculateEnergyMatrix(matrix);
        cumulativeMatrix = calculateCumulativeMatrix(energyMatrix);
        minPath = findMinPath(cumulativeMatrix);
        removeMinPath(matrix, minPath);
    }
}

int main(int argc, char*argv[]) 
{

    if (argc < 4)   std::cout << "Incorrect format: should be \"a.exe [image.pgm] [vertical seams] [horizontal seams]\"" << std::endl;
        
    else 
    {
        
        int maxValue = 0;
        int width = 0; 
        int height = 0;
        std::vector<std::vector<int> > valueMatrix;
        std::string topText;

        // Store arguments 
        std::string imageFilename = argv[1];
        int vertical = std::stoi(argv[2]);
        int horizontal = std::stoi(argv[3]);

        
    

        std::string outputFilename;
        outputFilename = imageFilename.substr(0, imageFilename.size()-4) + "_processed.pgm";
        

        std::ifstream file;
        file.open(imageFilename);
        
        if (file.is_open()) 
        {
            std::string block;
            int count = 0;
            // get first 3 values
            while (maxValue == 0) 
            {
                file >> block;
                
                // if '#', then skip the line
                if (block[0] == '#') 
                {
                    continue;
                }

                std::stringstream val(block);

                if (topText.empty()) topText = block;
                else if (width == 0) val >> width;
                else if (height == 0) val >> height;
                else val >> maxValue;
            }

                // build matrix from color values
                buildMatrix(valueMatrix, width, height);
                
                // fill matrix
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        file >> block;
                        std::stringstream myVal(block);
                        myVal >> valueMatrix[i][j];
                    }
                }
            


        } 
        else 
        {
            std::cout << "Failed to open file \"" << imageFilename << "\"" << std::endl;
        }

        if (maxValue != 0) {
            
            file.close();

            // remove vertical seams
            removeSeams(valueMatrix, vertical);

            // transpose the matrix for horizontal seam removal
            valueMatrix = transpose(valueMatrix);

            // remove horizontal seams
            removeSeams(valueMatrix, horizontal);

            // transpose back to normal
            valueMatrix = transpose(valueMatrix);

            // output valueMatrix to file
            std::ofstream outfile;
            outfile.open(outputFilename.c_str(), std::ofstream::out | std::ofstream::trunc);

                
            outfile << topText << "\n";
            outfile << "#  " << vertical << " vertical and " << horizontal << " seams removed from " << imageFilename << "\n";
            outfile << valueMatrix[0].size() << " " << valueMatrix.size() << "\n";
            outfile << maxValue << "\n";

            // export matrix 
            for (int i = 0; i < valueMatrix.size(); i++) 
            {
                for (int j = 0; j < valueMatrix[0].size(); j++) 
                {
                    outfile << valueMatrix[i][j] << " ";
                }
                outfile << "\n";
            }
            
            outfile.close();
            
            std::cout << "Output file exported to " << outputFilename << std::endl;
        }
    }
}