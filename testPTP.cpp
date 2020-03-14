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
    switch (ptp.generatePointToPointRoute(
        GeoCoord("34.0436968", "-118.4800519"),
        GeoCoord("34.0721826", "-118.4435144"),
    ssl, dist))
    {
        case DELIVERY_SUCCESS:
            cerr << "Delivery Success" << endl;
            break;
        case NO_ROUTE:
            cerr << "No Route" << endl;
            return 1;
        case BAD_COORD:
            cerr << "Bad Coord" << endl;
            return 1;
    }
    cout << "Total distance: " << dist << endl;
    cerr << ssl.begin()->start.latitudeText << ", " << ssl.begin()->start.longitudeText << endl;
    for(auto ii = ssl.begin(); ii != ssl.end(); ii++) {
        cerr << ii->end.latitudeText << ", " << ii->end.longitudeText << endl;
    }
}
