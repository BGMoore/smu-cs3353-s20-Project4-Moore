#include <iostream>     //cout
#include <fstream>      //ostream
#include <vector>       //holding data
#include <queue>        //priority queues
#include <algorithm>    //find
#include <random>       //random_engine

#include <stdio.h>      //NULL 
#include <stdlib.h>     //rand
#include <time.h>       //time
using namespace std;

class Graph {

private:

    class Node {
        public:
        Node(int n,int conn){
            name = n;
            color = 0;
            connections.push_back( conn );
        }
        bool operator== (int n) { return (name==n); }
        int color;
        int name;
        vector < int > connections;
    };

public:

    vector <Node> data;

    int searchData (int n){
        for(int i = 0; i < data.size(); i++){
            if( n == data[i].name ){ return i; }
        }//end for loop
        //case when none is found
        return -1;
    }//end search function

    int searchConnections (int index, int n){
        for(int i = 0; i < data[index].connections.size(); i++){
            if( n == data[index].connections[i] ){ return i; }
        }//end for loop
        //case when none is found
        return -1;
    }//end search function

    void openForReading(char* file_name){

        std::ifstream in;
        in.open(file_name);
        if(in.fail()) { throw runtime_error("File failed to open. Terminating..."); }

        char* buffer = new char [100];
        char* buffer2 = new char [100];
        char* dash = new char[2];
        char* waste = new char [100];

        string size;
        in >> size;
        size.erase(0,1);
        size.erase(size.length()-1,1);
        int data_size = stoi(size);

        //cout << "data set size: " << data_size << " units." << endl;

        for(int i = 0; i < data_size + 1; i++){ in >> buffer; } //writing to buffer through all values

        while( !in.eof() ) {

            in >> buffer;
            in >> dash;
            in >> buffer2;
            int to_insert,to_insert2;

            to_insert = atoi(buffer);
            to_insert2 = atoi(buffer2);

            //cout << to_insert << to_insert2 << endl;

            insert(to_insert,to_insert2);

        }//end of looping over file

        //printGraph();//prints graph to console
        in.close();
    }//end of readin function

    void insert(int new_element, int connected_element){
        int index = searchData(new_element);//checks if new element is in the list

        if(index == -1){ //when no existing node was found

            Node n1 (new_element,connected_element);

            int conn_index = searchData(connected_element);//checks if the connected element is or is not in the Graph

            if(conn_index == -1){//if the second value does not exist in the graph either
                Node n2 (connected_element,new_element);
                data.push_back(n1);
                data.push_back(n2);
            }

            else {
                //if a connection is found, n1 will be added to the list, then the connection will be added at index conn_index
                data.push_back(n1);

                //first, we search data[conn_index] for the connection with new element
                int inner_conn = searchConnections(conn_index,new_element);

                if(inner_conn == -1){
                    data[conn_index].connections.push_back(new_element);//adds new connection if it doesn't exist
                }
                else { cout << "Connection already exists. Not duplicating." << endl; }

            }
            //end of chunk for when first one was not found
        }
        else{

            //first we will see if the connection already exists
            int exists_index = searchConnections(index,connected_element);//checks if connection already exists

            //index is the location of new_element in data
            if(exists_index != -1){return;}

            else{
                int con_index = searchData(connected_element);//checks if second connected item is in graph

                if(con_index != -1){
                    data[con_index].connections.push_back(new_element);
                    data[index].connections.push_back(connected_element);
                }//if it is it just adds at the index

                else{//if it isnt it adds the opposite node, then pushes back the original node with the connected one
                    Node n(connected_element,new_element);
                    data.push_back(n);
                    data[index].connections.push_back(connected_element);
                }
            }
        }

    }//end of insert function

    //just iterates and prints the whole graph
    void printGraph(ofstream& o) {

        if (data.size() == 0){o << "Graph is empty!" << endl;}

        for(int i = 0; i < data.size(); i++){

            o << data[i].name << " - ";
            for(int z = 0; z < data[i].connections.size(); z++){
                o << data[i].connections[z] << " ";
            }
            o << endl;

        }

    }

    //uses a greedy algorithmic approach to color the graph
    void greedy_color(ostream& o = cout){

        data[0].color = 1;
        vector <int> color_list;
        color_list.push_back(1);
        
        //starting on 1 to apply this to all vertices besides the first one, which already received the first color
        for(int i = 1; i < data.size(); i++){

            vector <int> conns = data[i].connections;
            bool assigned = false;

            //outer loop will check each color unless a color is assigned
            for(int z = 0; z < color_list.size(); z++){

                bool colorUsed = false;
                //checks for any connected edge with the same color
                for(int x = 0; x < conns.size(); x++){

                    if(color_list[z] == data[ searchData(conns[x]) ].color ){
                        colorUsed = true;
                        break;
                    }

                }//end of for loop for checking all connecting edges

                //if you got here without continuing, assign the color to this value and break.
                if(!colorUsed){
                    data[i].color = color_list[z];
                    assigned = true;
                    break;
                }

            }//end of for loop for checking all the colors

            //if no pre-existing color was acceptable for this vertice, it adds a new value to the color list 
            if(!assigned) {
                color_list.push_back(color_list.size() + 1);
                data[i].color = color_list.back();
            }

        }//end of for loop iterating over all vertices

        o << "graph found using " << color_list.size() << " colors." << endl;

        //displays all of the edges and their color number to the specified ostream
        o << "all vertices were colored successfully, here is each vertices color (as an integer value):" << endl;
        for(int i = 0; i <data.size(); i++){

            o << data[i].name << " - " << data[i].color << endl;

        }

    }//end of greedy_color function

    //function used in the random graph coloring strategy, which checks if a graph can be generated with the specified number of colors
    bool generate_colors (int colors){

        int trials = data.size() * colors * 10;
        srand (time(NULL));

        for(int t = 0; t < trials; t++){

            //assigns each vertice a color randomly in the range
            for(int i = 0; i < data.size(); i++){
                data[i].color = rand() % colors + 1;
            }

            bool illegal_match = false;
            for(int i = 0; i < data.size(); i++){
                for(int z = 0; z < data[i].connections.size(); z++){
                    if(data[i].color == data[ searchData( (data[i].connections)[z] ) ].color ){
                        illegal_match = true;
                        break;
                    }
                }
                if(illegal_match){break;}
            }//end of loop for checking correctness of graph

            //if all of the connections are valid, therefore the graph is good
            if(!illegal_match){ 
                return true;
            }

        }

        //returns true or false based on whether or not the random was successful.
        return false;

    }

    //uses a random generation to color the graph
    void rand_color(ostream& o = cout){

        bool found = false;
        int counter = 1;

        //repeatedly checks and increases number of colors used to color the graph until one is correctly generated
        while(!found){

            found = generate_colors(counter);
            if(found){o << "graph found using " << counter << " colors." << endl;}
            counter++;

        }

        //displays all of the edges and their color number to the specified ostream
        o << "all vertices were colored successfully, here is each vertices color (as an integer value):" << endl;
        for(int i = 0; i <data.size(); i++){

            o << data[i].name << " - " << data[i].color << endl;

        }

    }

};