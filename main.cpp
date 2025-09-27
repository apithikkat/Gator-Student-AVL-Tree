#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;


// Read and insert edges exactly ‘no_lines’ times
void insertEdges(int no_lines, map<string, vector<string>>& graph) {
    for (int i = 0; i < no_lines; ++i) {
        string from, to;
        cin >> from >> to;                 // skips stray \n

        if (graph.find(from) == graph.end())
            graph[from] = {};              // checks if key exists
        graph[from].push_back(to);

        if (graph.find(to) == graph.end())
            graph[to] = {};                // every page appears as a key
    }
}


// Start every page with rank 1 / |V|
map<string, double> initializeRanks(const map<string, vector<string>>& graph) {
    map<string, double> currentRank;
    double initialRank = 1.0 / graph.size();
    for (map<string, vector<string>>::const_iterator it = graph.begin();
         it != graph.end(); ++it) {
        currentRank[it->first] = initialRank;
    }
    return currentRank;
}


// Print alphabetically, two decimals
void outputURLs(const map<string, double>& finalRanks) {
    vector<string> URLs;
    for (map<string, double>::const_iterator it = finalRanks.begin();
         it != finalRanks.end(); ++it) {
        URLs.push_back(it->first);
    }
    sort(URLs.begin(), URLs.end());

    cout << fixed << setprecision(2);
    for (size_t i = 0; i < URLs.size(); ++i) {
        cout << URLs[i] << " " << finalRanks.at(URLs[i]) << '\n';
    }
}


int main() {

    int no_lines, p;                       // edges, power-iterations
    cin >> no_lines >> p;

    map<string, vector<string>> graph;
    insertEdges(no_lines, graph);

    map<string, double> currentRank = initializeRanks(graph);

    // p = 1  =  zero iterations
    for (int iter = 1; iter < p; ++iter) {
        map<string, double> newRank;
        for (map<string, double>::iterator it = currentRank.begin();
             it != currentRank.end(); ++it) {
            newRank[it->first] = 0.0;
        }

        for (map<string, vector<string>>::iterator it = graph.begin();
             it != graph.end(); ++it) {

            const string& from = it->first;
            const vector<string>& outlinks = it->second;
            int outDegree = static_cast<int>(outlinks.size());

            if (outDegree > 0) {
                double share = currentRank[from] / outDegree;
                for (int i = 0; i < outDegree; ++i) {
                    const string& dest = outlinks[i];
                    newRank[dest] += share;
                }
            }
        }
        currentRank.swap(newRank);
    }

    outputURLs(currentRank);
    return 0;
}