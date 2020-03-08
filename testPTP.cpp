#include <iostream>
#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

int main() {
    StreetMap* sp = new StreetMap();
    sp->load("/Users/vrownie/Desktop/CS things/CS32/Project 4/Project 4/mapdata.txt");
    
    PointToPointRouter ptp(sp);
    list<StreetSegment> ssl;
    double dist;
    switch (ptp.generatePointToPointRoute(GeoCoord("34.0745410", "-118.4498928"), GeoCoord("34.0784189", "-118.4751579"), ssl, dist))
    {
        case DELIVERY_SUCCESS:
            cerr << "Delivery Success" << endl;
            break;
        case NO_ROUTE:
            cerr << "No Route" << endl;
            break;
        case BAD_COORD:
            cerr << "Bad Coord" << endl;
    }
    cout << dist << endl << endl;
    cerr << ssl.begin()->start.latitudeText << ", " << ssl.begin()->start.longitudeText << endl;
    for(auto i = ssl.begin(); i != ssl.end(); i++) {
        cerr << (*i).end.latitudeText << ", " << (*i).end.longitudeText << endl;
    }
}
