//Gautam Ramachandruni
// C++ program that uses a treap to implement the sweepline algorithm for 2-d closest pairs
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <cmath>
#include <math.h>
#include "Treap.h" // http://www.cs.fiu.edu/~weiss 
using namespace std;
// Quicksort algorithm
static int sort(const void* a, const void* b) // https://www.opengl.org/discussion_boards/showthread.php/180083-sort-on-x-and-y-of-2d-points
{
  int* A = (int*)a;
  int* B = (int*)b;
  int res = A[0] - B[0];
  if (res)
    return res;
  return A[1] - B[1];
}
int main(int argc, char* argv[])
{
    int n, p, q;
    int x1, x2;
    int y1, y2;
    int elements;
    int dist;  // Sweep line shortest distance between two points
    int min_dist = 100000;
    int k = 0;   // Index for line sweep traversal
    clock_t start, finish; // Timer to measure CPU time
    string file;
    ifstream myfile;
    int choice;
    cout << "Please select one of the following:" << endl;
    cout << "1. File 1 (1000)" << endl;
    cout << "2. File 2 (10000)" << endl;
    cout << "3. File 3 (100000)" << endl;
    cout << "4. Preferred file name" << endl;
    cin >> choice;
    switch(choice)
    {
        case 1:
        myfile.open("lab1fall16a.txt");
        break;
        case 2:
        myfile.open("lab1fall16b.txt");
        break;
        case 3:
        myfile.open("lab1fall16c.txt");
        break;
        case 4:
        cin >> file;
        myfile.open(file);
        break;
        default:
        cout << "Invalid choice" << endl;
    }
    if(myfile.is_open())
    {
        myfile >> n;
        int coordinates[n][2]; // Main array that stores x & y coordinates
        int tmp[1][2]; // Temporary array used for sorting
        int flag = 0;
        while(n != 0)
        {
            // Input to the array
            myfile >> coordinates[flag][0] >> coordinates[flag][1];
            flag++;
            n--;
        }
        elements = flag;
        cout << "Quicksort" << endl;
        cout << "---------" << endl;
        start = clock(); // Starting timer for quicksort
        // Quicksorting the ordered pair of coordinates w.r.t. the x coordinate
        std::qsort(coordinates, elements, sizeof coordinates[0], sort);
        finish = clock(); // Ending timer for quicksort
        // Outputting CPU time of the sort
        cout << std::setprecision(2) << "Quicksort CPU time: " << 1000.0 * (finish - start)/ CLOCKS_PER_SEC << " ms" << endl;
        cout << "Distance: " << min_dist << endl;
        p = 0;
        Treap<int> T;
        T.makeEmpty();
        // Brute-force
        cout << "Brute-force" << endl;
        cout << "-----------" << endl;
        start = clock(); // Starting timer for brute-force
        for(int i = 0; i < elements; i++) // http://www.cplusplus.com/forum/general/146237/
		{
            for(int j = i + 1; j < elements; j++)
			{
                dist = ((coordinates[j][1] - coordinates[i][1])*(coordinates[j][1] - coordinates[i][1])) + ((coordinates[j][0] - coordinates[i][0])*(coordinates[j][0] - coordinates[i][0]));
				if (dist < min_dist)
				{
                    min_dist = dist;
					x1 = coordinates[i][0];
                    x2 = coordinates[j][0];
					y1 = coordinates[i][1];
					y2 = coordinates[j][1];
                }
			}
		}
        finish = clock(); // Ending timer for brute-force
        cout << "The closest points are: "<<x1<<","<<y1<<" and "<<x2<<","<<y2 << endl;
        cout << std::setprecision(2) << "Brute-force CPU time: " << 1000.0 * (finish - start)/ CLOCKS_PER_SEC << " ms" << endl;
        cout << "Distance: " << min_dist << endl;
        // Line sweep
        cout << "Sweep Line" << endl;
        cout << "----------" << endl;
        start = clock(); // Starting timer for sweep line
        dist = (((coordinates[1][1]-coordinates[0][1]^2) + (coordinates[1][0] - coordinates[0][0]^2))^1/2);
        x1 = coordinates[1][0];
        x2 = coordinates[0][0];
        y1 = coordinates[1][1];
        y2 = coordinates[0][1];
        T.insert(coordinates[0][1],coordinates[0][0]); // Inserting into treap
        T.insert(coordinates[1][1],coordinates[1][0]);
        for(p = 2; p < elements; p++)
        {
            while(((coordinates[p][0] - coordinates[k][0])) > dist)
            {
                T.remove(coordinates[k][1]);
                k++;
            }
            for(int q = k; q < p ; q++)
            {
                if(((coordinates[p][0]-coordinates[q][0])*(coordinates[p][0]-coordinates[q][0])) <= dist)
                {
                    if(((coordinates[p][0]-coordinates[q][0])*(coordinates[p][0]-coordinates[q][0])) <= dist)
                    {
                        T.insert(coordinates[q][1], coordinates[q][0]);
                    }
                }
                else
                {
                    if(T.contains(coordinates[q][1]))
                    {
                        T.remove(coordinates[q][1]);
                    }
                }
            }

            dist = T.calcDist(coordinates[p][1], k, dist);
        }
        T.insert(coordinates[p][1], coordinates[p][0]);
        finish = clock(); // Ending timer for sweep line
        cout << "The closest points are: "<<x1<<","<<y1<<" and "<<x2<<","<<y2 << endl;
        cout << std::setprecision(2) << "Sweep line CPU time: " << 1000.0 * (finish - start)/ CLOCKS_PER_SEC << " ms" << endl;
        cout << "Distance: " << min_dist << endl;
    }   
    else
        cout << "Invalid file" << endl;  
}
