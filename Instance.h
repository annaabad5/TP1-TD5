#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>

class Instance {
private:
    int N; // Numero de segmentos
    int M; // Numero de influencers
   
    std::vector<int> influencerCosts; // Costos de cada influencer
    std::vector<std::vector<int>> influencerSegments; // Segmentos cubiertos por cada influencer


public:
    // Constructor
    Instance();
    
    std::pair<Instance, Instance> splitBySegmentCount(int k) const;

    // Cargar instancia desde archivo
    bool loadFromFile(const std::string& filename);
    
    // Getters
    int getNumSegments() const;
    int getNumInfluencers() const;

    // Getters para costos y segmentos
    int getInfluencerCost(int idx) const { return influencerCosts[idx]; }
    const std::vector<int>& getInfluencerSegments(int idx) const { return influencerSegments[idx]; }
    
    // Setter para establecer datos
    void setData(int N, int M, const std::vector<int>& costs, const std::vector<std::vector<int>>& segments);
    
    // Mostrar informacion de la instancia
    void printInstance() const;

};

#endif // INSTANCE_H