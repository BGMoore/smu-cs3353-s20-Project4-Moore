#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include "graph.cpp"
//#include "search.h"

using namespace std;

int main () {

    //here we will intialize the graph 
    Graph graph;

    //reads in data from TXT here
    char* filename = new char [50];
    filename = "karate.txt";
    graph.openForReading(filename);

    //declares output file for writing out to TXT
    ofstream o("testout.txt");

    //declares timing variable
    chrono::time_point<std::chrono::system_clock> start_greedy, end_greedy, start_rand, end_rand;
    
    //displays the initial graph
    graph.printGraph(o);

    //first time marker
    start_greedy = chrono::system_clock::now();

    //performs greedy graph coloring
    graph.greedy_color(o);

    //takes ending time, then sets starting time of random to the same point
    end_greedy = chrono::system_clock::now();
    start_rand = end_greedy;
    
    //performs random graph coloring
    graph.rand_color(o);

    //takes final time mark, then computes the times.
    end_rand = chrono::system_clock::now();
    chrono::duration<double> greedy_time = end_greedy - start_greedy;
    chrono::duration<double> rand_time = end_rand - end_greedy;

    //outputs operation time in seconds
    o << "The greedy graph coloring took " << greedy_time.count() << " seconds." << endl
        << "The random graph coloring took " << rand_time.count() << " seconds." << endl;

    return 0;
}