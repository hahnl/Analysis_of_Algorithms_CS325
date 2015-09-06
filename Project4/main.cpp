#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <limits>
#include <cassert>
#include <cmath>
#include <time.h>
#include <algorithm>
#include <vector>

using namespace std;

//max # of cities
int MAX = 5000;

int TSP(int[], int[][3], int);
int distanceBetweenCities(int[][3], int, int);

int main(int argc, char *argv[]) {

    if(argc != 2) {
        cout << "Usage: " << argv[0] << " [filename].txt\n";
    }

    string inputfile_name = argv[1];
    //string inputfile_name = "test-input-7.txt";

    //make int array
    int cityArray[MAX][3];
    int count = 0;

    //open file for reading
    ifstream inputfile(inputfile_name.c_str());
    if(inputfile.is_open()) {
        string line;    //line to be read from file
        char * numline; //c-string copy of line
        char * num;     //number from line

        while(!inputfile.eof()) {
            //get denominations
            getline(inputfile, line);
            //rest variables
            numline = NULL;
            num = NULL;

            //convert line to numline
            numline = new char [line.size()+1];
            strcpy (numline, line.c_str());

            num = strtok (numline, " ");
            int i = 0;
            while (num != NULL) {
                cityArray[count][i] = atoi(num);
                num = strtok (NULL, " ");
                if(i != 2)
                    ++i;
                //move to next city
                else {
                    i = 0;
                    count++;
                }
            }
        }
        inputfile.close();
    }
    else {
        cout <<"Unable to open file";
    }

    int tourArray[count];

    //run TSP algorithm
    int score = TSP(tourArray, cityArray, count);

    //open file for writing (data will be appended to end of file)
    string outputfile_name = inputfile_name.append(".tour");
    ofstream outputfile (outputfile_name.c_str(), ios::app);
    if(outputfile.is_open()) {
        //outputfile << "Tour Length: ";
        outputfile << score;
        for(int i=0; i< count; i++) {
            outputfile << endl << tourArray[i];
        }
    }
    else {
        cout <<"Unable to open file";
    }

    return 0;
}

/*
*   Description:    Determines a solution for the Traveling Salesman problem
*                   using a nearest neighbor algorithm
*
*   Input:          Cities (and their x/y coordinates), number of cities
*
*   Output:         solution
*/
int TSP(int tourArray[], int arr[][3], int size) {
    //cities not traveled to yet
    vector<int> citiesNotUsed;
    for(int i = 0; i < size; ++i) {
        citiesNotUsed.push_back(arr[i][0]);
    }
    //cities traveled to
    vector<int> citiesUsed;
    //first city
    citiesUsed.push_back(arr[0][0]);
    //start at the first city
    int startCity = 0;
    int legDist = 0;
    int cityUsed = 0;
    int tourLength = 0;
    int legs = size-1;
    //continue until all cities have been visited
    while(legs > 0) {
        int lowestDistance = 999999;
        for(int i = 1; i < size; ++i) {
            //skip if same city
            if((citiesUsed[startCity] != citiesNotUsed[i]) && (citiesNotUsed[i] != -1)) {
                legDist = distanceBetweenCities(arr, citiesUsed[startCity], citiesNotUsed[i]);
                if(legDist < lowestDistance) {
                    lowestDistance = legDist;
                    cityUsed = i;
                }
            }
        }
        //increase tour length
        tourLength += lowestDistance;
        ++startCity;
        //add city to list of those used
        citiesUsed.push_back(arr[cityUsed][0]);
        //take out city from list of not visited cities
        citiesNotUsed.at(cityUsed) = -1;
        --legs;
    }

    //length from last city to first city
    tourLength += distanceBetweenCities(arr, citiesUsed[startCity], 0);
    //copy tour sequence into array
    for(int i = 0; i < size; ++i)
        tourArray[i] = citiesUsed[i];

    return tourLength;
}

/*
*   Description:    Determines the Euclidian distance between two cities
*
*   Input:          Cities (and their x/y coordinates), starting city, ending city
*
*   Output:         distance between two cities
*/
int distanceBetweenCities(int cities[][3], int start, int dest) {
    //get x/y coordinates of start and dest cities
    int startX = cities[start][1];
    int destX = cities[dest][1];
    int startY = cities[start][2];
    int destY = cities[dest][2];

    //find euclidian distance
    int distance = round(sqrt(pow((startX - destX), 2.0) + pow((startY - destY), 2.0)));
    return distance;
}















