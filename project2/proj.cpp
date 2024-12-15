#include <iomanip>
#include <iostream>
#include <vector>
#include <queue>

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
        cout << center_align(to_string(i+1), columnWidth) << "|";
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
        cout << setw(8) << left << "Line " + to_string(line+1) + ":" << "|";
        for(const auto &nodes : graph) {
            if (nodes[line].first != -1 && nodes[line].second != -1) {
                cout << center_align(to_string(nodes[line].first + 1) + " " +
                                       to_string(nodes[line].second + 1),
                                   columnWidth)
                     << "|";
                continue;
            } else if (nodes[line].first != -1) {
                cout << center_align(to_string(nodes[line].first+1), columnWidth) << "|";
            } else if (nodes[line].second != -1) {
                cout << center_align(to_string(nodes[line].second+1), columnWidth) << "|";
            } else {
                cout << setw(columnWidth) << "" << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int metro_connectivity(vector<vector<pair<int, int>>> &graph) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int connectivity = 0;

    return 0;
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

    print_graph(graph);

    return metro_connectivity(graph);
}
