#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <list>
#include <limits.h>
using namespace std;
class Vertex;

class Edge{
public:
    int weight = INT_MAX;
    string label;
    Vertex* vertex;
    Edge(string label, Vertex* v){
        this->label = label;
        vertex = v;
    }
};

class Vertex{
public:
    int id;
    string value;
    string discovered = "undiscovered";
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
 
 

int main() {
	// stream object to hold input from file for sanitation
	vector<char> answer;
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

	// main loop
	for (int i = 1; i <= mazeCount; i++) {
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
        delete g;
        
	}

    /* 
    this chunk of code checks to see if output.txt is an empty file
    if it is empty, then it creates an output file stream to it
    otherwise it creates a new temporary file, deletes output
    and renames temp to output - this is to make sure we are writing to a clean output.txt
    that doesn't have information from a previous run of the code
    */
    inputFile.open("output.txt");
    if(inputFile.peek() == std::ifstream::traits_type::eof()){
        inputFile.close();
        outputFile.open("output.txt");
    }else{
        inputFile.close();
        outputFile.open("temp.txt");
        remove("output.txt");
        outputFile.close();
        rename("temp.txt", "output.txt");
        outputFile.open("output.txt");
    }
    
    // write results to the output file and close it
  
    outputFile.close();

    
    return 0;
}