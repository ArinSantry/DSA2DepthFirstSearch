#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
#include <queue>
using namespace std;

struct vNode
{
	int dest;
	vNode* next;
};

struct AdjList
{
	vNode* head;
};

class Graph
{
private:
	int V;
	AdjList* array;
public:
	Graph(int size)
	{
		V = size;
		array = new AdjList[V];
		for (int i = 0; i < V; i++)
			array[i].head = NULL;
	}

	Graph(string inputFile)
	{
		std::ifstream inFile;
		inFile.open(inputFile, ios::in);
		if (inFile.is_open())
		{
			int size;
			inFile >> size;
			if (size > 0)
			{
				V = size;
				array = new AdjList[V];
				for (int i = 0; i < V; i++)
					array[i].head = NULL;

				int src, dest;

				while (inFile >> src >> dest)
				{
					cout << "Adding edge between " << src << " and " << dest << endl;
					addEdge(src, dest);
				}

				inFile.close();
			}
			else
			{
				cerr << "Incorrect graph size" << endl;
			}
		}
		else
		{
			cerr << "Cannot open file" << endl;
		}
	}

	stack <vNode*> dfsv;
	queue <vNode*> bfsv;
	vNode* createNode(int dest);
	queue <int> dfsOrdered;
	queue <int> bfsOrdered;
	bool visited[10];
	void addEdge(int src, int dest);
	void displayGraph();
	void dfs(int start);
	void bfs(int start);
	void printBfsOrdered();
	void printDfsOrdered();
	void resetVisited();
};

vNode* Graph::createNode(int dest)
{
	vNode* newNode = new vNode;
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

void Graph::addEdge(int v, int w)
{
	vNode* newNode = createNode(v);
	newNode->next = array[w].head;
	array[w].head = newNode;

	newNode = createNode(w);
	newNode->next = array[v].head;
	array[v].head = newNode;
}

void Graph::displayGraph()
{
	for (int i = 0; i < V; i++)
	{
		vNode* curr = array[i].head;
		cout << "Vertex[" << i << "]:";
		while (curr != NULL)
		{
			cout << "-> " << curr->dest;
			curr = curr->next;
		}
		cout << endl;
	}
	//dfs(array[0].head);
	bfs(0);
	printBfsOrdered();
	resetVisited();
	cout << endl;
	dfs(0);
	printDfsOrdered();
}

void Graph::dfs(int start)
{
	vNode* startNode = array[start].head;
	vNode* curr = startNode;

	// curr keeps track of the current node without losing startNode
	while (curr != NULL)
	{
		if (!visited[startNode->dest])
		{
			dfsOrdered.push(startNode->dest); // add to ordered list
			dfsv.push(startNode); // add node to the dfs stack
			visited[startNode->dest] = true; // mark node as visited
			dfs(startNode->dest); // start with next adjacent node
		}
		else if (startNode->next != NULL) // find next adjacent node
			startNode = startNode->next;
		else
			curr = startNode->next; // signal to exit the loop
	}

	// backtrack
	if (!dfsv.empty())
	{
		dfsv.pop();
		if (!dfsv.empty()) // to avoid reading data from an empty stack
		{
			startNode = dfsv.top();
			start = startNode->dest;
			dfs(start);
		}
	}

	// check for disconnected
	for (int i = 0; i < 10; i++)
	{
		if (visited[array[i].head->dest] == false)
		{
			dfs(i);
		}
	}
}

void Graph::bfs(int start)
{
	vNode* startNode = array[start].head;
	// to keep track of which vertices have been visited, an array of booleans
	// the index represents the vertex
	vNode* curr = startNode;

	// check if there is a next unvisited vertex
	if (startNode->next != NULL && visited[startNode->next->dest] == false)
	{
		// visit the vertices until there are no more
		while (curr != NULL)
		{
			// if the node hasn't been visited yet, add it to the queue
			if (visited[startNode->dest] == false)
			{
				visited[startNode->dest] = true;
				bfsv.push(startNode); // add node to the bfs queue
				bfsOrdered.push(startNode->dest); // add to ordered list
			}
			// if there is a next node, continue
			if (startNode->next != NULL)
			{
				startNode = startNode->next;
			}
			// to let the program know when to exit the loop without losing startNode
			else
			{
				curr = startNode->next;
			}
		}
	}
	else if (visited[startNode->dest] == false) // if there is not a next node
	{
		visited[startNode->dest] = true;
		bfsv.push(startNode);
		bfsOrdered.push(startNode->dest);
	}

	if (!bfsv.empty())
	{
		// no more adjacent vertices, backtrack one
		startNode = bfsv.front();
		bfsv.pop();
		bfs(startNode->dest);
	}

	// check for disconnected
	for (int i = 0; i < 10; i++)
	{
		if (visited[array[i].head->dest] == false)
		{
			bfs(i);
		}
	}
}

void Graph::printBfsOrdered()
{
	cout << "Bredth-first search: ";
	while (!bfsOrdered.empty())
	{
		cout << bfsOrdered.front() << ", ";
		bfsOrdered.pop();
	}
}

void Graph::printDfsOrdered()
{
	cout << "Depth-first search: ";
	while (!dfsOrdered.empty())
	{
		cout << dfsOrdered.front() << ", ";
		dfsOrdered.pop();
	}
}

void Graph::resetVisited()
{
	for (int i = 0; i < 10; i++)
	{
		visited[i] = false;
	}
}

int main()
{
	Graph myGraph("input.txt");
	myGraph.displayGraph();
	myGraph.resetVisited();

	// my visual studio is dumb and needs this or it closes the window
	cout << "\nPress Enter to Continue";
	cin.ignore();
	return 0;
}