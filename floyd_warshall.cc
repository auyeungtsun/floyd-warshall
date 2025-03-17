#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

/**
 * @brief Computes the shortest paths between all pairs of vertices in a graph using the Floyd-Warshall algorithm.
 *
 * @param numVertices The number of vertices in the graph.
 * @param edges A vector of tuples, where each tuple represents a directed edge (u, v, weight).
 *              u represents the source vertex, v represents the destination vertex, and weight is the weight (cost) of the edge from u to v.
 *
 * @return A pair containing:
 *         - A pair of matrices:
 *           - dist: A 2D vector representing the distance matrix, where dist[i][j] is the shortest distance from vertex i to vertex j.
 *           - next: A 2D vector representing the next matrix, where next[i][j] is the next vertex on the shortest path from i to j.
 *                   If next[i][j] == -1, it means there's no path or i==j
 *         - A boolean:
 *           - negativeCycle: True if a negative cycle is detected in the graph, false otherwise.
 *
 * @note The vertices are assumed to be labeled from 0 to numVertices - 1.
 *       If there is no path between i to j, dist[i][j] will be INT_MAX.
 * @note Time Complexity: O(V^3), where V is the number of vertices.
 * @note Space Complexity: O(V^2), to store the distance and next matrices.
 */
pair<pair<vector<vector<int>>, vector<vector<int>>>, bool> floydWarshall(int numVertices, const vector<tuple<int, int, int>>& edges) {
    vector<vector<int>> dist(numVertices, vector<int>(numVertices, INT_MAX)); // distance matrix
    vector<vector<int>> next(numVertices, vector<int>(numVertices, -1)); // next matrix for path reconstruction
    
    for (int i = 0; i < numVertices; ++i) {
        dist[i][i] = 0;
    }
    for (const auto& edge : edges) {
        int u, v, weight;
        tie(u, v, weight) = edge;
        dist[u][v] = weight; // initialize dist
        next[u][v] = v;   // initialize next, the next node for the path from u to v is v
    }
    for (int k = 0; k < numVertices; ++k) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k]; // update next, the next node for the path from i to j, the new path is through k, so the next node is the next node for path from i to k.
                }
            }
        }
    }
    bool negativeCycle = false;
     for (int i = 0; i < numVertices; ++i) {
        if (dist[i][i] < 0) {
            negativeCycle = true;
            break;
        }
    }

    return {{dist, next}, negativeCycle};
}

void testFloydWarshall() {
    // Test case 1: Basic graph
    vector<tuple<int, int, int>> edges1 = {
        {0, 1, 10},
        {0, 3, 5},
        {1, 3, 2},
        {1, 2, 1},
        {2, 4, 4},
        {3, 1, 3},
        {3, 2, 9},
        {3, 4, 2},
        {4, 2, 6}
    };
    auto result1 = floydWarshall(5, edges1);
    auto dist1 = result1.first.first;
    auto next1 = result1.first.second;
    bool negativeCycle1 = result1.second;

    assert(!negativeCycle1);
    vector<vector<int>> expectedDist1 = {
        {0, 8, 9, 5, 7},
        {INT_MAX, 0, 1, 2, 4},
        {INT_MAX, INT_MAX, 0, INT_MAX, 4},
        {INT_MAX, 3, 4, 0, 2},
        {INT_MAX, INT_MAX, 6, INT_MAX, 0}
    };
    vector<vector<int>> expectedNext1 = {
        {-1, 3, 3, 3, 3},
        {-1, -1, 2, 3, 3},
        {-1, -1, -1, -1, 4},
        {-1, 1, 1, -1, 4},
        {-1, -1, 2, -1, -1}
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            assert(dist1[i][j] == expectedDist1[i][j]);
            assert(next1[i][j] == expectedNext1[i][j]);
        }
    }

    // Test case 2: Negative cycle
    vector<tuple<int, int, int>> edges2 = {
        {0, 1, -1},
        {1, 2, -2},
        {2, 0, -3}
    };
    auto result2 = floydWarshall(3, edges2);
    auto dist2 = result2.first.first;
    bool negativeCycle2 = result2.second;
    assert(negativeCycle2);
    
    // Test case 3: Negative edges, no negative cycle
    vector<tuple<int, int, int>> edges3 = {
        {0, 1, -2},
        {1, 2, 3},
        {2, 3, -4},
        {0,3, 1}
    };
    auto result3 = floydWarshall(4, edges3);
    auto dist3 = result3.first.first;
    auto next3 = result3.first.second;
    bool negativeCycle3 = result3.second;
    assert(!negativeCycle3);
    vector<vector<int>> expectedDist3 = {
        {0, -2, 1, -3},
        {INT_MAX, 0, 3, -1},
        {INT_MAX, INT_MAX, 0, -4},
        {INT_MAX, INT_MAX, INT_MAX, 0}
    };
    vector<vector<int>> expectedNext3 = {
        {-1, 1, 1, 1},
        {-1, -1, 2, 2},
        {-1, -1, -1, 3},
        {-1, -1, -1, -1}
    };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            assert(dist3[i][j] == expectedDist3[i][j]);
            assert(next3[i][j] == expectedNext3[i][j]);
        }
    }

}

void runFloydWarshallSample() {
    // Sample usage:
    vector<tuple<int, int, int>> edges = {
        {0, 1, 10},
        {0, 3, 5},
        {1, 3, 2},
        {1, 2, 1},
        {2, 4, 4},
        {3, 1, 3},
        {3, 2, 9},
        {3, 4, 2},
        {4, 2, 6}
    };
    auto result = floydWarshall(5, edges);
    auto dist = result.first.first;
    auto next = result.first.second;
    bool negativeCycle = result.second;
    
    // Print the distance matrix
    cout << "Distance Matrix:" << endl;
    for (const auto& row : dist) {
        for (int d : row) {
            cout << (d == INT_MAX ? "INF" : to_string(d)) << " ";
        }
        cout << endl;
    }
    
    // Print the next matrix
    cout << "\nNext Matrix:" << endl;
    for (const auto& row : next) {
        for (int n : row) {
            cout << (n == -1 ? "N/A" : to_string(n)) << " ";
        }
        cout << endl;
    }
    cout << "\nNegative Cycle: " << (negativeCycle ? "Yes" : "No") << endl;
}

int main() {
    testFloydWarshall();
    runFloydWarshallSample();
    return 0;
}
