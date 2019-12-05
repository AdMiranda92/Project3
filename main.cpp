#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <list>
#include <limits.h>
#include <set>
#include <deque>
using namespace std;
class Vertex;

class Edge{
public:
    int weight = 1;
    string label;
    Vertex* vertex;
    Edge(string label, Vertex* v){
        this->label = label;
        vertex = v;
    }
};

class Vertex{
public:
    int distance = INT_MAX;
    int id;
    string value;
    string prevLabel;
    string discovered = "undiscovered";
    Vertex* prev;
    list<Edge*> adjList;

    Vertex(int id){
        this->id = id;
    }

    void addEdge(Vertex* source, string label){
        for(Edge* i: this->adjList){
            if(i->vertex->id == source->id){
                return;
            }
        }
        this->adjList.push_front(new Edge(label, source));
    }
};

class Graph{
public:
    int numVertices;
    vector<Vertex*> Vertices;

    Graph(int V){
        numVertices = V;
        for(int i=0; i<V; i++){
            Vertices.push_back(new Vertex(i));
        }
    }

    void addEdge(int src, int dest, string label){
        Vertices[src]->addEdge(Vertices[dest], label);
    }

};
 
 /*
  Function to find shortest path between the start node and every other node until
  the destination node is marked discovered -- essentially a custom Djikstra's algorithm for
  my graph 
 */
 void findExit(Graph* g, int src, int dest, set<Vertex*>* visited){
    set<Vertex*> unvisited;
    Vertex* currentNode = g->Vertices[src];
    currentNode->distance = 0;
    while(g->Vertices[dest]->discovered != "Visited"){
        // Add unvisited vertices of the current node to the unvisited set
        for(Edge* i: currentNode->adjList){
            if(i->vertex->discovered != "Visited"){
                unvisited.emplace(i->vertex);
            }
        }

        for(Edge* i: currentNode->adjList){
            if(currentNode->distance + i->weight < i->vertex->distance){
                i->vertex->distance = currentNode->distance + i->weight;
                i->vertex->prev = currentNode;
            }
        }
        currentNode->discovered = "Visited";
        visited->emplace(currentNode);
        unvisited.erase(currentNode);
        currentNode = *unvisited.begin();
        for(Vertex* i: unvisited){
            if(i->distance < currentNode->distance){
                currentNode = i;
            }
        }
    }
 };

int main() {
	// stream object to hold input from file for sanitation
	set<Vertex*> answer;
	stringstream ostringStream;
	string line;
	ifstream inputFile;
	ofstream outputFile;
	inputFile.open("input.txt", std::ios::in);

	
	if (inputFile.is_open()) {
		cout << "File successfully opened - continuing program" << endl;
		getline(inputFile, line);
		ostringStream.str(line);
	}
	else {
		cout << "File not found" << endl;
		exit(EXIT_FAILURE);
	}


	int  mazeCount, levels, rows, columns;
	int trackx, tracky, trackz;
	string data;
	ostringStream >> mazeCount;

    // Opening the output file
    // FILE IS OPENED IN APPEND MODE AS A PRECAUTION -- OUTPUT FILE SHOULD BE
    // AN EMPTY FILE TO AVOID ANY OUTPUT FORMAT ERRORS
    outputFile.open("output.txt", ios_base::app);

	// main loop    
	for (int i = 1; i <= mazeCount; i++) {
        deque<string> path;
        cout << "Starting Maze " << i << '\n';
		while (getline(inputFile, line))
		{
			if (line == "") { continue; } // Skip blank line
			ostringStream.clear();
			ostringStream.str(line);
			break;

		}
		
        ostringStream >> levels >> rows >> columns;

        // initiate the graph with total vertices
        Graph *g = new Graph(levels*rows*columns);

        // get the starting vertex
		ostringStream.clear();
		getline(inputFile, line);
		ostringStream.str(line);
		ostringStream >> trackz >> trackx >> tracky;
		int start = (trackz*(rows*columns)) + (trackx*rows) + tracky;

        // get the ending vertex
		ostringStream.clear();
		getline(inputFile, line);
		ostringStream.str(line);
		ostringStream >> trackz >> trackx >> tracky;
        int end = (trackz*(rows*columns)) + (trackx*rows) + tracky;
        

        // give each vertex its string value
        for(int k=0; k<levels*rows*columns;){
            std::getline(inputFile, line);
            ostringStream.clear();
            ostringStream.str(line);
            while(ostringStream >> data){
                g->Vertices[k]->value = data;
                k++;
            }
        }

        // connect all vertices together with their proper edges
        for(Vertex* x: g->Vertices){
             if(x->value[0] == '1'){
                 g->addEdge(x->id, x->id - columns, "N");
            }if(x->value[1] == '1'){
                g->addEdge(x->id, x->id+1, "E");
            }if(x->value[2] == '1'){
                g->addEdge(x->id, x->id + columns, "S");
            }if(x->value[3] == '1'){
                g->addEdge(x->id, x->id-1, "W");
            }if(x->value[4] == '1'){
                g->addEdge(x->id, x->id + (columns*rows), "U");
            }if(x->value[5] == '1'){
                g->addEdge(x->id, x->id - (columns*rows), "D");
            }
        }

        /*
        // UNCOMMENT THIS CHUNK OF CODE TO SEE THE EDGES BETWEEN VERTICES
        // THIS WILL PRINT A LOT OF INFORMATION TO THE CONSOLE
        for(Vertex* x: g->Vertices){
            cout << "Vertex " << x->id << " connects to: ";
            for(Edge *z: x->adjList){
                cout << z->label << " " << z->vertex->id << " -> ";
            }
            cout << '\n';
        }
        */

        /*
        // UNCOMMENT THIS CHUNK OF CODE TO VERIFY THAT EACH VERTEX IN EACH MAZE
        // WAS ASSIGNED ITS PROPER STRING VALUE: THIS WILL PRINT A LOT OF INFORMATION TO THE CONSOLE
        for(Vertex* x: g->Vertices){
            cout << "Vertex " << x->id << " has value " << x->value << '\n';
        }
        */
        
        findExit(g, start, end, &answer);

        // At this point, every node should have a shortest path from the start now
        // and every node should have its pointer set to a proper previous node 
        // we can then set a node pointer to our destination node and traverse
        // backwards until we hit the start node
        Vertex* travNode = g->Vertices[end];
        Vertex* next;
        while(travNode->id != start){
            next = travNode;
            travNode = travNode->prev;

            for(Edge* i: travNode->adjList){
                if(i->vertex->id == next->id){
                    path.emplace_front(i->label);
                }
            }

        }

        /*
        //UNCOMMENT THIS CHUNK OF CODE TO READ THE EXIT PATH IN CONSOLE BEFORE WRITING TO FILE
        cout << "Start: " << start << " End: " << end << endl;
        cout << "Exit Path: ";
        for(string i: path){
            cout << i << " ";
        }
        */


        // writing the path to the output file 
        for(string i: path){
            outputFile << i << " ";
        }
        outputFile << '\n';
        path.empty();
        answer.empty();
        delete g;
        
	}


    outputFile.close();

    
    return 0;
}