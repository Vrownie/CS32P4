#include "provided.h"

#include <vector>

using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, double& oldCrowDistance, double& newCrowDistance) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm) {}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl() {}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, double& oldCrowDistance, double& newCrowDistance) const
{
    //before
    oldCrowDistance = (deliveries.size() == 0) ? 0 : distanceEarthMiles(depot, deliveries[0].location);
    for(int i = 1; i < deliveries.size(); i++)
        oldCrowDistance += distanceEarthMiles(deliveries[i-1].location, deliveries[i].location);
    
    //depot->first
    int minI = -1;
    double minD = 9999999;
    for(int i = 0; i < deliveries.size(); i++) {
        if(distanceEarthMiles(depot, deliveries[i].location) < minD) {
            minI = i;
            minD = distanceEarthMiles(depot, deliveries[i].location);
        }
    }
    if (minI != -1) swap(deliveries[0], deliveries[minI]);
    
    //in-between
    for(int i = 1; i < deliveries.size(); i++) {
        int minIndex = -1;
        double minDist = 9999999;
        for(int j = i + 1; j < deliveries.size(); j++) {
            if(distanceEarthMiles(deliveries[i].location, deliveries[j].location) < minDist) {
                minIndex = j;
                minDist = distanceEarthMiles(deliveries[i].location, deliveries[j].location);
            }
        }
        if (minIndex != -1) swap(deliveries[i], deliveries[minIndex]);
    }
    
    //after
    newCrowDistance = (deliveries.size() == 0) ? 0 : distanceEarthMiles(depot, deliveries[0].location);
    for(int i = 1; i < deliveries.size(); i++)
        newCrowDistance += distanceEarthMiles(deliveries[i-1].location, deliveries[i].location);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
