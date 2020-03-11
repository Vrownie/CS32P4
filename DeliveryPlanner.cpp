#include "provided.h"
#include "support.h"
#include <vector>
#include <list>

using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(const GeoCoord& depot, const vector<DeliveryRequest>& deliveries, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const;
    
private:
    PointToPointRouter m_ptp;
    DeliveryOptimizer m_optimizer;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm) : m_ptp(sm), m_optimizer(sm) {}

DeliveryPlannerImpl::~DeliveryPlannerImpl() {}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(const GeoCoord& depot, const vector<DeliveryRequest>& deliveries, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const
{
    vector<DeliveryRequest> deli(deliveries);
    double distanceOld = 0, distanceNew = 0, distTotal = 0;
    m_optimizer.optimizeDeliveryOrder(depot, deli, distanceOld, distanceNew);
    list<StreetSegment> ssl;
    if (deli.size() != 0) {
        m_ptp.generatePointToPointRoute(depot, deli[0].location, ssl, distTotal);
        ParseOneDelivery(false, deli[0].item, ssl, commands, distTotal);
        for (int i = 1; i < deli.size(); i++) {
            m_ptp.generatePointToPointRoute(deli[i-1].location, deli[i].location, ssl, distTotal);
            ParseOneDelivery(false, deli[i].item, ssl, commands, distTotal);
        }
        m_ptp.generatePointToPointRoute(deli[deli.size()-1].location, depot, ssl, distTotal);
        ParseOneDelivery(true, "", ssl, commands, distTotal);
    }
    
    return NO_ROUTE;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
