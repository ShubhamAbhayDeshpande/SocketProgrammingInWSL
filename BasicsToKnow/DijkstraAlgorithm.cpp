/*
Implementation of Dijkstra's algorithm for finding the shortest path in a graph using 2D array implementation of 
graph. The Adjacent list implementation of the graph is not considered here because it is too complicated. 

*/
#include<iostream>
#include<limits.h>

// Define the value for infinity and size of an array (number of vertices in graph) 
#define INFI 999 
#define V 6 // Vertices

using namespace std;

void printGraph(int graph[V][V])
{
    for (int i=0; i<V; i++)
    {
        for (int j=0; j<V; j++)
        if (i!=j && graph[i][j] != 0)
        {
            cout<<i<<" is connected to "<<j<<" with edge weight: "<<graph[i][j]<<endl;
        }
    }
}

int findMinDistance(int distance[], bool visited[])
{
    int min = INFI;
    int minIndex = -1;

    for (int i=0; i<V; i++)
    {
        if(!visited[i] && distance[i]<min)
        {
            min = distance[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void printPath(int parentNode[], int destinationNode)
{
    if (parentNode[destinationNode] == -1)
    {
        cout<<destinationNode;
        return;
    }

    printPath(parentNode, parentNode[destinationNode]);
    cout<<" -> "<<destinationNode;
}


void printSolution(int dist[], int parent[], int source, int destination)
{
    cout << "\n=== RESULT ===\n";
    cout << "Shortest distance from " << source << " to " << destination;

    cout<<" = "<< dist[destination]<<endl;

    cout << "Path: ";
    printPath(parent, destination);
    cout << "\n\n";


}
void dijkstraAlgo(int graph[V][V], int startNode, int endNode)
{
    // Define 3 arrays for distance, parent node and visited node
    int distance[V]; 
    int parent[V]; 
    bool visited[V]; 

    // Set the distance for the source node with itself to 0 and all other distances to infinity
    for (int node = 0; node<V; node++)
    {
        if (node == startNode)
        {
            distance[node] = 0;
        }
        else
        {       
        distance[node] = INFI; // Initially all nodes are at a infinite distance from start node
        }
        parent[node] = -1; // Initially there is no parent for any node
        visited[node] = false; // Initially no node is visited
    }

    for(int count=0; count<V; count++)
    {
        int u = findMinDistance(distance, visited);

        // break if the value of u is -1.
        if (u == -1) break;

        // mark the vertex as visited
        visited[u] = true;

        // Handling special case where we either reach destination node or source and destination node are the same.
        if (u == endNode) break;

        // Adjust the distances of other vertices
        for (int v=0; v<V; v++)
        {
            // 'v' represents any other vertex. We will update distance[v] if:
            // 1. v is not visited yet.
            // 2. There is a direct edge between u and v
            // 3. Total weight from source to v through u is smaller than the value already present at distance[v].

            if (!visited[v] && graph[u][v]!=0 && 
                distance[u] != INFI && 
                distance[u]+graph[u][v] < distance[v])
            {
                distance[v] = distance[u] + graph[u][v];
                parent[v] = u;
            }
        }
        cout<<endl;
    }
    printSolution(distance, parent, startNode, endNode);

}
int main()
{
    // Matrix (2D array) representation of given graph
    int graph[V][V]={
        //  0, 1, 2, 3, 4, 5
        {   0, 4, 2, 0, 0, 0 },  // From vertex 0
        {   0, 0, 1, 5, 0, 0 },  // From vertex 1
        {   0, 0, 0, 8, 10, 0 }, // From vertex 2
        {   0, 0, 0, 0, 2, 6 },  // From vertex 3
        {   0, 0, 0, 0, 0, 3 },  // From vertex 4
        {   0, 0, 0, 0, 0, 0 }
    };

    printGraph(graph);

    // Define start and end nodes
    int startNode = 0;
    int endNode = 5;

    //Function call Dijkstra Algorithm 
    cout<<endl;
    cout<<"======================================="<<endl;
    cout<<"Implementing Dijkstra Algorithm"<<endl;
    cout<<"======================================="<<endl;

    dijkstraAlgo(graph, startNode, endNode);

    return 0;
}


