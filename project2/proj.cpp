#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

// Helper function to center-align a string
string center_align(const string &text, int width) {
    int padding = width - static_cast<int>(text.length());
    int padLeft = padding / 2;
    int padRight = padding - padLeft;
    return string(padLeft, ' ') + text + string(padRight, ' ');
}

void print_graph(const vector<vector<pair<int, int>>> &graph) {
    if (graph.empty()) {
        cout << "Graph is empty." << endl;
        return;
    }

    int maxNodeWidth = 5;
    int maxPairWidth = 0;
    for (const auto &nodes : graph) {
        for (const auto &edge : nodes) {
            maxPairWidth =
                max(maxPairWidth,
                    static_cast<int>(to_string(edge.first).length() + 3 +
                                     to_string(edge.second).length()));
        }
    }

    int columnWidth = max(maxNodeWidth, maxPairWidth + 4);

    // Print header row (Node indices)
    cout << setw(8) << left << "Nodes:" << "|";
    for (size_t i = 0; i < graph.size(); i++) {
        cout << center_align(to_string(i + 1), columnWidth) << "|";
    }
    cout << endl;

    // Print separator line
    cout << string(8, '-') << "+";
    for (size_t i = 0; i < graph.size(); i++) {
        cout << string(columnWidth, '-') << "+";
    }
    cout << endl;

    // Print graph content (lines and edges)
    for (size_t line = 0; line < graph[0].size(); line++) {
        cout << setw(8) << left << "Line " + to_string(line + 1) + ":" << "|";
        for (const auto &nodes : graph) {
            if (nodes[line].first != -1 && nodes[line].second != -1) {
                cout << center_align(to_string(nodes[line].first + 1) + " " +
                                         to_string(nodes[line].second + 1),
                                     columnWidth)
                     << "|";
                continue;
            } else if (nodes[line].first != -1) {
                cout << center_align(to_string(nodes[line].first + 1),
                                     columnWidth)
                     << "|";
            } else if (nodes[line].second != -1) {
                cout << center_align(to_string(nodes[line].second + 1),
                                     columnWidth)
                     << "|";
            } else {
                cout << setw(columnWidth) << "" << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Min priority queue comparator
auto cmp = [](const tuple<int, int, int> &node1,
              const tuple<int, int, int> &node2) {
    return get<0>(node1) > get<0>(node2);
};

// void print_priority_queue(
//     priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
//                    decltype(cmp)>
//         pq) {
//     cout << "Priority Queue Contents:" << endl;
//     while (!pq.empty()) {
//         tuple<int, int, int> current = pq.top();
//         int changes = get<0>(current);
//         int node = get<1>(current);
//         int line = get<2>(current);
//         pq.pop();
//         cout << changes << " " << node + 1 << " " << line + 1 << endl;
//     }
// }

int metro_connectivity(vector<vector<pair<int, int>>> &graph, int starting_node) {
    int total_nodes = graph.size();
    int total_lines = graph[0].size();
    int total_changes = 0;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
                   decltype(cmp)>
        pq(cmp);  // changes, node, line

    // visited nodes
    vector<vector<bool>> line_visited(total_nodes, vector<bool>(total_lines, false));
    vector<bool> visited(total_nodes, false);

    // Start from node 0
    for (size_t i = 0; i < graph[starting_node].size(); i++) {
        pair<int, int> next_line = graph[starting_node][i];
        if (next_line.first != -1) {
            pq.push({0, next_line.first, i});
        }
        if (next_line.second != -1) {
            pq.push({0, next_line.second, i});
        }
        line_visited[starting_node][i] = true;
    }
    visited[starting_node] = true;
    total_nodes--;

    while (!pq.empty()) {
        // Exit if all paths to nodes been found.
        if (total_nodes == 0) {
            break;
        }
        //print_priority_queue(pq);

        // Get the next node
        tuple<int, int, int> current = pq.top();
        total_changes = get<0>(current);
        int node = get<1>(current);
        int line = get<2>(current);
        pq.pop();

        line_visited[node][line] = true;

        // Add the next nodes to the queue
        for (size_t i = 0; i < graph[node].size(); i++) {
            pair<int, int> next_line = graph[node][i];
            bool changed = (unsigned long)line != i ? true : false;

            if (next_line.first != -1 && !line_visited[next_line.first][i]) {
                pq.push({total_changes + changed, next_line.first, i});
            }
            if (next_line.second != -1 && !line_visited[next_line.second][i]) {
                pq.push({total_changes + changed, next_line.second, i});
            }
        }

        if(!visited[node]){
            total_nodes--;
            visited[node] = true;
        }
    }

    if (total_nodes != 0) {
        return -1;
    }

    return total_changes;
}

int main() {
    // Stations (nodes)
    int stations;
    cin >> stations;

    // Connections (links)
    int connections;
    cin >> connections;

    // Lines
    int lines;
    cin >> lines;

    // Graph
    vector<vector<pair<int, int>>> graph(
        stations, vector<pair<int, int>>(lines, {-1, -1}));

    // Fill graph
    for (int i = 0; i < connections; i++) {
        int a, b, l;
        cin >> a >> b >> l;

        // Normalize to 0-based
        --a, --b, --l;

        // Link a to b with l
        if (graph[a][l].first == -1) {
            graph[a][l].first = b;
        } else {
            graph[a][l].second = b;
        }

        if (graph[b][l].first == -1) {
            graph[b][l].first = a;
        } else {
            graph[b][l].second = a;
        }
    }

    //print_graph(graph);

    int max_changes = -1;
    for(size_t i = 0; i < graph.size(); i++){
        //cout << max_changes << endl;
        //cout << "Starting from node " << i + 1 << endl;
        int changes = metro_connectivity(graph, i);
        if(changes == -1){
            break;
        }
        max_changes = max(max_changes, changes);
    }

    cout << max_changes << endl;

    return 0;
}
