#include <climits>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;
int stations, connections, lines;
vector<set<int>> graph;

// Helper function to center-align a string
string center_align(const string &text, int width) {
    int padding = width - static_cast<int>(text.length());
    int padLeft = padding / 2;
    int padRight = padding - padLeft;
    return string(padLeft, ' ') + text + string(padRight, ' ');
}

void print_graph(const vector<set<int>> &graph) {
    if (graph.empty()) {
        cout << "Graph is empty." << endl;
        return;
    }

    int maxNodeWidth = 5;
    int columnWidth = maxNodeWidth + 4;

    // Print header row (Node indices)
    cout << setw(8) << left << "Nodes:" << "|";
    for (size_t i = 0; i < graph.size(); i++) {
        cout << center_align(
                    to_string(i + 1) + (i >= (size_t)stations ? "*" : ""),
                    columnWidth)
             << "|";
    }
    cout << endl;

    // Print separator line
    cout << string(8, '-') << "+";
    for (size_t i = 0; i < graph.size(); i++) {
        cout << string(columnWidth, '-') << "+";
    }
    cout << endl;

    // Print graph content (connections)
    for (size_t i = 0; i < graph.size(); i++) {
        cout << center_align(to_string(i + 1), 8) << "|";
        for (size_t j = 0; j < graph.size(); j++) {
            if (graph[i].find(j) != graph[i].end()) {
                cout << center_align("1", columnWidth) << "|";
            } else {
                cout << center_align("", columnWidth) << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Print the queue
void print_queue(const vector<pair<int, int>> &queue) {
    cout << "Queue: ";
    for (const auto &node : queue) {
        cout << "(" << node.first + 1 << ", " << node.second << ") ";
    }
    cout << endl;
}

int metro_connectivity(int starting_node) {
    int total_changes = 0;
    int total_nodes = stations;
    vector<bool> visited(stations + lines, false);
    queue<pair<int, int>> queue;

    // Initialize the queue
    for (auto &node : graph[starting_node]) {
        queue.emplace(node,0);  // (node, changes)
    }
    visited[starting_node] = true;

    // Do the BFS
    while (!queue.empty()) {
        if(total_nodes == 0) {
            break;
        }
        //print_queue(queue);

        pair<int, int> current = queue.front();
        queue.pop();
        int node = current.first;
        int changes = current.second;
        bool changed = false;

        // Check if the node is visited
        if (visited[node]) {
            continue;
        }
        visited[node] = true;

        if (node < stations) {
            --total_nodes;
            total_changes = changes;
            changed = true;
        }

        // Add the neighbors to the queue
        for (auto &neighbor : graph[node]) {
            if (!visited[neighbor]) {
                queue.emplace(neighbor, changes + changed);
            }
        }
    }

    if (total_nodes != 0) {
        return -1;
    }

    return total_changes;
}

int main() {
    // Stations (nodes)
    cin >> stations;

    // Connections (links)
    cin >> connections;

    // Lines
    cin >> lines;

    // Graph
    graph = vector<set<int>>(stations + lines, set<int>());

    // Fill graph
    for (int i = 0; i < connections; i++) {
        int a, b, l;
        cin >> a >> b >> l;

        // Normalize to 0-based
        --a, --b, --l;
        l += stations;

        // Add the connections
        graph[a].insert(l);
        graph[b].insert(l);
        graph[l].insert(a);
        graph[l].insert(b);
    }

    //print_graph(graph);

    int max_changes = -1;

    // Calculate the connectivity from each line.
    for (int i = stations; i < stations + lines; i++) {
        int changes = metro_connectivity(i);
        if (changes == -1) {
            break;
        }
        max_changes = max(max_changes, changes);
    }

    cout << max_changes << endl;

    return 0;
}
