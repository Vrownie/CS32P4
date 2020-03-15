#include <unordered_set>
#include <list>
#include <queue>

#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const;
private:
    struct GeoHash {
        unsigned int operator()(const GeoCoord& g) const { return std::hash<string>()(g.latitudeText + g.longitudeText); }
    };
    const StreetMap* m_smp;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm) { m_smp = sm; }

PointToPointRouterImpl::~PointToPointRouterImpl() {}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const
{
    if (start == end) {
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    
    vector<StreetSegment> ssv;
    if (!m_smp->getSegmentsThatStartWith(start, ssv) || !m_smp->getSegmentsThatStartWith(end, ssv)) return BAD_COORD;
    
    //BFS
    queue<GeoCoord> cq;
    unordered_set<GeoCoord, GeoHash> visited;
    ExpandableHashMap<GeoCoord, pair<GeoCoord, string>> map;
    
    cq.push(start);
    visited.insert(start);
    while (!cq.empty()) {
        GeoCoord current = cq.front();
        cq.pop();
        
        //if we reached the end
        if (current == end) {
            GeoCoord coord = end;
            pair<GeoCoord, string>* cpair;
            list<StreetSegment> l;
            double dist = 0;
            do {
                cpair = map.find(coord);
                if(cpair != nullptr) {
                    l.push_front(StreetSegment(cpair->first, coord, cpair->second));
                    dist += distanceEarthMiles(cpair->first, coord);
                    coord = cpair->first;
                }
                else break;
            } while(true);
            route = l;
            totalDistanceTravelled = dist;
            return DELIVERY_SUCCESS;
        }
        
        //if we didn't reach the end yet
        m_smp->getSegmentsThatStartWith(current, ssv);
        for(int i = 0; i < ssv.size(); i++) {
            if (visited.find(ssv[i].end) == visited.end()) { //if we have not visited theis coordinate
                pair<GeoCoord, string> p(current, ssv[i].name);
                map.associate(ssv[i].end, p);
                cq.push(ssv[i].end);
                visited.insert(current);
            }
        }
    }
    return NO_ROUTE; //cannot find any path
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
