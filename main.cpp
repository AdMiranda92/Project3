#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <list>
using namespace std;

class Vertex{
public:

    int id;
    string value;
    string discovered = "undiscovered";
    list<Vertex*> adjList;

    Vertex(int id){
        this->id = id;
    }

    void addEdge(Vertex* source){
        for(Vertex* i: this->adjList){
            if(i->id == source->id){
                return;
            }
        }
        this->adjList.push_front(source);
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

    void addEdge(int src, int dest){
        cout << "Adding Edge Between " << src << " and " << dest << '\n';
        Vertices[src]->addEdge(Vertices[dest]);
    }

};
 
 

int main() {
	// stream object to hold input from file for sanitation
	std::vector<char> answer;
	std::stringstream ostringStream;
	std::string line;
	std::ifstream inputFile;
	std::ofstream outputFile;
	inputFile.open("input.txt", std::ios::in);

	
	if (inputFile.is_open()) {
		std::cout << "File successfully opened - continuing program" << std::endl;
		std::getline(inputFile, line);
		ostringStream.str(line);
	}
	else {
		std::cout << "File not found" << std::endl;
		exit(EXIT_FAILURE);
	}


	int  mazeCount, levels, rows, columns;
	int trackx, tracky, trackz;
	std::string data;
	ostringStream >> mazeCount;

	// main loop
	for (int i = 1; i <= mazeCount; i++) {
        cout << "Starting Maze " << i << '\n';
		while (std::getline(inputFile, line))
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
		std::getline(inputFile, line);
		ostringStream.str(line);
		ostringStream >> trackz >> trackx >> tracky;
		int start = (trackz*(rows*columns)) + (trackx*rows) + tracky;

        // get the ending vertex
		ostringStream.clear();
		std::getline(inputFile, line);
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
            cout << "Adding edges for vertex " << x->value << '\n';
             if(x->value[0] == '1'){
                 g->addEdge(x->id, x->id - columns);
            }if(x->value[1] == '1'){
                g->addEdge(x->id, x->id+1);
            }if(x->value[2] == '1'){
                g->addEdge(x->id, x->id + columns);
            }if(x->value[3] == '1'){
                g->addEdge(x->id, x->id-1);
            }if(x->value[4] == '1'){
                g->addEdge(x->id, x->id + (columns*rows));
            }if(x->value[5] == '1'){
                g->addEdge(x->id, x->id - (columns*rows));
            }
        }

        /*
        UNCOMMENT THIS CHUNK OF CODE TO SEE THE EDGES BETWEEN VERTICES
        THIS WILL PRINT A LOT OF INFORMATION TO THE CONSOLE
        for(Vertex* x: g->Vertices){
            cout << "Vertex " << x->id << " connects to: ";
            for(Vertex *z: x->adjList){
                cout << z->id << " -> ";
            }
            cout << '\n';
        }
        */

        /*
        UNCOMMENT THIS CHUNK OF CODE TO VERIFY THAT EACH VERTEX IN EACH MAZE
        WAS ASSIGNED ITS PROPER STRING VALUE: THIS WILL PRINT A LOT OF INFORMATION TO THE CONSOLE
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