#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <numeric>
using namespace std;

// Returns a random value between [1, m]
#define randomValue(m) ((rand() % m) + 1)

//----------------------------------------------------------------------------- 
void printUsage(char *progname) {
  cerr << "Usage: " << progname << " <V> <E> <L> <b> <seed>" << endl;
  cerr << "  V: number of stations (vertices)" << endl;
  cerr << "  E: number of connections (edges) E >= V" << endl;
  cerr << "  L: number of lines (colors) L>1" << endl;
  cerr << "  b: all stations be connected? (optional, 0/1) E >= V" << endl;
  cerr << "  seed: random seed generator (optional)" << endl;
  exit(1);
}

//----------------------------------------------------------------------------- 
int main(int argc, char* argv[]) {
    int V, E, L;
    bool bConnect = false;
    int seed = 0;

    if (argc < 4 || argc > 6) {
        cerr << "ERROR: Wrong number of arguments" << endl;
        printUsage(argv[0]);
    }

    sscanf(argv[1], "%d", &V);
    sscanf(argv[2], "%d", &E);
    sscanf(argv[3], "%d", &L);

    if (argc > 4) {
        int arg;
        sscanf(argv[4], "%d", &arg);
        bConnect = (arg != 0);
    }
    if (argc > 5) {
        sscanf(argv[5], "%d", &seed);
    }

    if (L < 1) {
        cerr << "ERROR: L < 1" << endl;
        printUsage(argv[0]);
    }
    if (bConnect && E < V - 1) {
        cerr << "ERROR: E < V-1 and bConnect" << endl;
        printUsage(argv[0]);
    }
    if (E > V * L) {
        cerr << "ERROR: E > V*L" << endl;
        printUsage(argv[0]);
    }

    // Random number generator setup
    mt19937 rng(seed ? seed : (unsigned int)time(NULL));
    vector<set<int>> lineStations(L);
    vector<int> vAllVs(V);
    iota(vAllVs.begin(), vAllVs.end(), 1);

    vector<int> vAllLs(L);
    iota(vAllLs.begin(), vAllLs.end(), 0);
    shuffle(vAllLs.begin(), vAllLs.end(), rng);

    vector<pair<int, int>> edges; // Store edges to print later

    // If bConnect is true, generate a spanning tree
    if (bConnect) {
        vector<int> connected, unconnected(vAllVs);
        shuffle(unconnected.begin(), unconnected.end(), rng);

        connected.push_back(unconnected.back());
        unconnected.pop_back();

        while (!unconnected.empty()) {
            int u = connected[rng() % connected.size()];
            int v = unconnected.back();
            unconnected.pop_back();

            edges.emplace_back(u, v);
            connected.push_back(v);
        }
    }

    // Add remaining edges randomly
    while ((int)edges.size() < E) {
        int u = randomValue(V);
        int v = randomValue(V);
        if (u != v && find(edges.begin(), edges.end(), make_pair(u, v)) == edges.end() &&
            find(edges.begin(), edges.end(), make_pair(v, u)) == edges.end()) {
            edges.emplace_back(u, v);
        }
    }

    // Assign edges to lines dynamically as E and L grow
    shuffle(edges.begin(), edges.end(), rng);
    for (size_t i = 0; i < edges.size(); ++i) {
        int line = vAllLs[i % L];
        lineStations[line].insert(edges[i].first);
        lineStations[line].insert(edges[i].second);
    }

    cout << V << " " << E << " " << L << endl;

    // Print edges with line assignment
    for (size_t i = 0; i < edges.size(); ++i) {
        int line = (i % L) + 1;
        cout << edges[i].first << " " << edges[i].second << " " << line << endl;
    }

    // Print stats for tests: constant V, varying E and L
    cerr << "TEST SUMMARY:" << endl;
    cerr << "Vertices: " << V << endl;
    cerr << "Edges: " << E << endl;
    cerr << "Lines: " << L << endl;

    return 0;
}
