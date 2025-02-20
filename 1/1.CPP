#include <iostream.h>
#include <conio.h>
#define INT_MAX 99

//using namespace std;

class Dijkstra {
    int n, cost[10][10], dist[10], parent[10], visited[10];

public:
    void readMatrix();
    void findShortestPath(int);
    void display(int);
};

void Dijkstra::readMatrix() {
    cout << "Enter the number of vertices: ";
    cin >> n;

    cout << "Enter the cost adjacency matrix (use a high value like 99 for no direct edge):\n";
    for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
	    cin >> cost[i][j];
	}
    }
}

void Dijkstra::findShortestPath(int src) {
    int i;
    for (i = 0; i < n; i++) {
	dist[i] = cost[src][i]; // Initialize distance array
	visited[i] = 0;         // Mark all vertices as unvisited
	parent[i] = src;        // Set the parent node
    }

    visited[src] = 1; // Mark the source as visited

    for (i = 1; i < n; i++) { // Find shortest path for all vertices
	int min = INT_MAX, u = -1;

	for (int j = 0; j < n; j++) {
	    if (!visited[j] && dist[j] < min) {
		min = dist[j];
		u = j;
	    }
        }

        if (u == -1) return; // No more reachable nodes
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && cost[u][v] != 999 && (dist[u] + cost[u][v] < dist[v])) {
                dist[v] = dist[u] + cost[u][v];
                parent[v] = u;
            }
        }
    }
}

void Dijkstra::display(int src) {
    for (int i = 0; i < n; i++) {
        if (i == src)
            continue;

        cout << "Shortest path from " << src << " to " << i << ":\n";
        int k = i;
        cout << k;

        while (parent[k] != src) {
            cout << " <-- " << parent[k];
            k = parent[k];
        }
        cout << " <-- " << src << endl;
        cout << "Total distance: " << dist[i] << endl;
    }
}

int main() {
    clrscr();
    Dijkstra dij;
    int source;

    dij.readMatrix();
    cout << "Enter the source vertex: ";
    cin >> source;

    dij.findShortestPath(source);
    dij.display(source);

    getch();

    return 0;
}
