/*
Implemeting Graph datastructure using adjacency list 

The code and the explanation for it can be foud in the link https://www.geeksforgeeks.org/cpp/implementation-of-graph-in-cpp/

*/
#include<iostream>
#include<string>
#include<map>
#include<list>
#include<utility> // required for using "pair" 
#include<vector>

using namespace std;

class Graph
{
    /*
    Implementation of undirectional graph structure
    */

    // Adjacency list to store the vertices and linked list to store the other vertices it is connected to.
    // If the edges are not weighted
    // map<int, list<int>> adjList;

    // If the edges are weighted
    map<int, list<pair<int, int>>> adjList;

    public:
        // Function to connected vertices if the edges are un-weighted.
        // void add_edge(int u, int v)
        // {
        //     // Add edge from u to v 
        //     adjList[u].push_back(v);
        //     // Add edge from v to u, undirectional graph.
        //     adjList[v].push_back(u);
        // }

        // Function to connect vertices if the edges are weighted.
        void add_edge(int u, int v, int w)
        {
            // Add edge from u to v
            adjList[u].push_back({v,w});

            // Add edge from v to u, undirectional graph.
            adjList[v].push_back({u,w});
        }

        void printGratph()
        {
            for (auto [key, value] : adjList)
            {
                cout<<key<<" -> ";
                for (auto [list1, list2]: value)
                {
                    cout<<"connected to vertex "<< list1<<" with weight "<<list2<<" and ";
                }
                cout<<endl;
            }
        }
};

int main()
{
    Graph g;

    g.add_edge(1,0,5);
    g.add_edge(2,0,6);
    g.add_edge(1,2,6);
    g.printGratph();
}

