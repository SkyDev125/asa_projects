#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
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
  cerr << "  seed: random seed generator (opcional)" << endl;
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
      bConnect = (arg!=0);
    }
    if (argc > 5) {
      sscanf(argv[5], "%d", &seed);
      srand(seed);
    } else { // pseudo-random seed
      srand((unsigned int)time(NULL));
    }

    if (L < 1) {
      cerr << "ERROR: L < 1" << endl;
      printUsage(argv[0]);
    }
    if (bConnect && E < V) {
      cerr << "ERROR: E < V and bConnect" << endl;
      printUsage(argv[0]);
    }
    if (E > (V*L-V/2)) { // should be E>V*L but for quick convergence...
      cerr << "ERROR: E > (V*L-V/2)" << endl;
      printUsage(argv[0]);
    }

    //-------------------------------------------------------------------------
    mt19937 rng(seed);
    vector<set<int>> lineStations(L);
    // All stations
    vector<int> vAllVs = vector<int>(V);
    for (int i = 0; i < V; ++i) vAllVs[i] = i+1;
    // All lines
    vector<int> vAllLs = vector<int>(L);
    for (int i = 0; i < L; ++i) vAllLs[i] = i;
    shuffle(vAllLs.begin(), vAllLs.end(), rng);

    // Each line is contiguous
    int lineEdges = V / L; // min # edges for a line
    if (lineEdges < 1) lineEdges = 1;
    for (int line = 0; line < L; line++) { // for each line
        shuffle(vAllVs.begin(), vAllVs.end(), rng);
        for (int i = 0; i < lineEdges; i++) { // add stations to line
          lineStations[line].insert(vAllVs[i]);
        }
    }

    // force that all nodes are connected
    if (bConnect) {
      set<int> totalStations(vAllVs.begin(), vAllVs.end());
      for (int l = 0; l < L; l++) {
        for (const int e : lineStations[l])
          totalStations.erase(e);
      }
      // totalStations has all stations not used yet
      int lpos = 0;
      while (!totalStations.empty()) {
        int e = *totalStations.begin();
        lineStations[lpos].insert(e);
        totalStations.erase(e);
        lpos = (lpos+1) % L; // round-robin on the lines
      }
    }

    // Check current edges
    int currE = 0;
    for (int l = 0; l < L; ++l) { // |E| = |V| - 1
      size_t sz = 0;
      // it's not really an edge. Need to add a random station
      while ((sz = lineStations[l].size()) == 1) {
        lineStations[l].insert(randomValue(V));
      }
      currE += (sz - 1);
    }

    // shuffle everything again, just because we can...
    shuffle(vAllLs.begin(), vAllLs.end(), rng);
    shuffle(vAllVs.begin(), vAllVs.end(), rng);

    // Do we need to add more edges?
    int lpos = 0, spos = 0;
    while (currE < E) {
      size_t sz = lineStations[lpos].size();
      lineStations[lpos].insert(vAllVs[spos]);
      if (lineStations[lpos].size() > sz) currE++; // if it was added
      lpos = (lpos+1) % L; // the round-robin way
      spos = (spos+1) % V; // the round-robin way
    }

    cout << V << " " << currE << " " << L << endl;
    // Print the edges
    for (int l = 0; l < L; ++l) {
      // linearly shuffle stations of line l
      vector<int> stationList(lineStations[l].begin(), lineStations[l].end());
      shuffle(stationList.begin(), stationList.end(), rng);
      // print edge (e1, e2) -> l
      for (size_t i = 1; i < stationList.size(); ++i) {
        int e1 = stationList[i - 1];
        int e2 = stationList[i];
        cout << e1 << " " << e2 << " " << (l+1) << std::endl;
      }
    }

    return 0;
}
