#include <vector>
#include <list>
#include "provided.h"

using namespace std;

inline string calcDir(const double& angle)
{
    if (angle > 360)
        return "";
    else if (angle >= 337.5)
        return "east";
    else if (angle >= 292.5)
        return "southeast";
    else if (angle >= 247.5)
        return "south";
    else if (angle >= 202.5)
        return "southwest";
    else if (angle >= 157.5)
        return "west";
    else if (angle >= 112.5)
        return "northwest";
    else if (angle >= 67.5)
        return "north";
    else if (angle >= 22.5)
        return "northeast";
    else if (angle >= 0)
        return "east";
    else
        return "";
}

bool ParseOneDelivery(const bool& isFinal, const std::string& item, const std::list<StreetSegment>& ssl, std::vector<DeliveryCommand>& commands, double& dist)
{
    if(ssl.size()==0) { //empty
        if(!isFinal) {
            DeliveryCommand t;
            t.initAsDeliverCommand(item);
            commands.push_back(t);
        }
        return false;
    }
    
    double totalDist = 0;
    string streetNow = ssl.front().name, dir;
    GeoCoord startCoord = ssl.front().start, endCoord = ssl.front().end;
    double angleFirst = angleOfLine(ssl.front()), distNow = distanceEarthMiles(ssl.front().start, ssl.front().end);
    DeliveryCommand currentCommand;
    for(auto ii = next(ssl.begin()); ii != ssl.end(); ii++) {
        if(ii->name == streetNow) //same street
            distNow += distanceEarthMiles(ii->start, ii->end);
        else { //new street
            endCoord = ii->start;
            dir = calcDir(angleFirst);
            if(dir == "") return false;
            currentCommand.initAsProceedCommand(dir, streetNow, distNow);
            commands.push_back(currentCommand);
            
            //turn command
            streetNow = ii->name;
            double angleNew = angleBetween2Lines(*prev(ii),*ii);
            if (angleNew >= 1 && angleNew < 180) {
                currentCommand.initAsTurnCommand("left", streetNow);
                commands.push_back(currentCommand);
            } else if (angleNew >= 180 && angleNew < 359) {
                currentCommand.initAsTurnCommand("right", streetNow);
                commands.push_back(currentCommand);
            }
            //update for next command
            angleFirst = angleOfLine(*ii);
            totalDist += distNow;
            distNow = distanceEarthMiles(ii->start, ii->end);
        }
    }
    
    //for the last command
    dir = calcDir(angleFirst);
    currentCommand.initAsProceedCommand(dir, streetNow, distNow);
    commands.push_back(currentCommand);
    totalDist += distNow;
    
    //check if it's the final
    if(!isFinal) {
        currentCommand.initAsDeliverCommand(item);
        commands.push_back(currentCommand);
    }
    dist += totalDist;
    return true;
}

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
    vector<DeliveryRequest> deli(deliveries); //so we don't modify the original
    double distanceOld = 0, distanceNew = 0, distTotal = 0, d = 0;
    m_optimizer.optimizeDeliveryOrder(depot, deli, distanceOld, distanceNew);
    
    list<StreetSegment> ssl;
    DeliveryResult status;
    if (deli.size() != 0) {
        //depot->first
        status = m_ptp.generatePointToPointRoute(depot, deli[0].location, ssl, d);
        if(status != DELIVERY_SUCCESS) return status;
        ParseOneDelivery(false, deli[0].item, ssl, commands, distTotal);
        
        //in between
        for (int i = 1; i < deli.size(); i++) {
            status = m_ptp.generatePointToPointRoute(deli[i-1].location, deli[i].location, ssl, d);
            if(status != DELIVERY_SUCCESS) return status;
            ParseOneDelivery(false, deli[i].item, ssl, commands, distTotal);
        }
        
        //last->depot
        status = m_ptp.generatePointToPointRoute(deli[deli.size()-1].location, depot, ssl, d);
        if(status != DELIVERY_SUCCESS) return status;
        ParseOneDelivery(true, "", ssl, commands, distTotal);
    }
    
    totalDistanceTravelled = distTotal;
    return DELIVERY_SUCCESS;
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
