#include "Instance.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>

using namespace std;

Instance::Instance() : N(0), M(0) {}

bool Instance::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return false;
    }

    file >> N;
    file >> M;

    if (N <= 0 || M <= 0) {
        cout << "Error: invalid instance dimensions\n";
        return false;
    }

    influencerCosts.clear();
    influencerSegments.clear();

    string line;
    getline(file, line); // limpiar salto de línea

    for (int i = 0; i < M; i++) {
        getline(file, line);
        stringstream ss(line);
        string token;

        int cost;
        vector<int> segments;

        // primer número = costo
        if (getline(ss, token, ',')) {
            cost = stoi(token);
        } else {
            cout << "Error: missing cost at influencer " << i << endl;
            return false;
        }

        // resto = segmentos cubiertos
        while (getline(ss, token, ',')) {
            int segment = stoi(token);
            segments.push_back(segment);
        }

        influencerCosts.push_back(cost);
        influencerSegments.push_back(segments);
    }

    file.close();
    return true;
}

void Instance::setData(int N_, int M_, const vector<int>& costs, const vector<vector<int>>& segments) {
    N = N_;
    M = M_;
    influencerCosts = costs;
    influencerSegments = segments;
}

int Instance::getNumSegments() const { return N; }
int Instance::getNumInfluencers() const { return M; }

void Instance::printInstance() const {
    cout << "N = " << N << ", M = " << M << "\n";
    for (int i = 0; i < M; i++) {
        cout << "Influencer " << i << " | Costo: " << influencerCosts[i] << " | Segmentos: {";
        for (size_t j = 0; j < influencerSegments[i].size(); j++) {
            cout << influencerSegments[i][j];
            if (j + 1 < influencerSegments[i].size()) cout << ", ";
        }
        cout << "}\n";
    }
}

// Divide la instancia en micro y macro influencers según k
pair<Instance, Instance> Instance::splitBySegmentCount(int k) const {
    Instance micro, macro;
    micro.N = N;
    macro.N = N;

    for (int i = 0; i < M; i++) {
        int cost = influencerCosts[i];
        const vector<int>& segments = influencerSegments[i];
        if ((int)segments.size() <= k) {
            micro.influencerCosts.push_back(cost);
            micro.influencerSegments.push_back(segments);
        } else {
            macro.influencerCosts.push_back(cost);
            macro.influencerSegments.push_back(segments);
        }
    }

    micro.M = (int)micro.influencerCosts.size();
    macro.M = (int)macro.influencerCosts.size();

    return {micro, macro};
}