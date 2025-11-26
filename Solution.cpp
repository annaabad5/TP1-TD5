#include "Solution.h"
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

Solution::Solution() {
    //Inicializo los valores ya que todavia no tengo ningun influencer.
    _costoTotal = 0; 
    _influencersUsados = {}; 
    _segmentosCount = {};
    _coveredCount = 0;
}

Solution::Solution(int numSegments) {
    _costoTotal = 0; 
    _influencersUsados = {}; 
    _segmentosCount.assign(numSegments + 1, 0); // +1 porque los segmentos van de 1 a N
    _coveredCount = 0;
}

void Solution::addInfluencer(int influencer, int costo, const vector<int>& segmentos) {
    _influencersUsados.push_back(influencer);
    _costoTotal = _costoTotal + costo;
    for (int s : segmentos) {
        if (_segmentosCount[s] == 0){ //si ningún influencer lo cubía
             _coveredCount++;
        }
        _segmentosCount[s]++; //aumento en 1 cuántos influencers lo cubren ahora
    }
}

bool Solution::containsInfluencer(int influencer) const {
    return find(_influencersUsados.begin(), _influencersUsados.end(), influencer) != _influencersUsados.end();
}

void Solution::printSolution() const {
    cout << "Influencers usados: ";
    for (size_t i = 0; i < _influencersUsados.size(); i++) {
        cout << _influencersUsados[i];
        if (i + 1 < _influencersUsados.size()) {
            cout << " ";
        }
    }
    cout << "\n";
    cout << "Costo total: " << _costoTotal << endl;
}

void Solution::initCover(int N) {
    _segmentosCount.assign(N + 1, 0); // +1 porque los segmentos van de 1 a N
    _coveredCount = 0;
}

int Solution::getCoveredCount() const{
    return _coveredCount;
}

int Solution::InfluencerSize() const{ //cant de influencers para usar en mi sol parcial
    return _influencersUsados.size();
}

int Solution::getInfluencerPos(int influencer) const{ //devuelve el valor asociado que es el influencer
    return _influencersUsados[influencer];
}