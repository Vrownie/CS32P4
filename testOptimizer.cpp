#include <iostream>
#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

int main() {
    StreetMap* sp = new StreetMap();
    sp->load("/Users/vrownie/Desktop/CS things/CS32/Project 4/Project 4/mapdata.txt");
    
    DeliveryOptimizer optimizer(sp);
    GeoCoord depot("34.0625329", "-118.4470263");
    vector<DeliveryRequest> drv = {DeliveryRequest("Chicken tenders (Sproul Landing)", GeoCoord("34.0712323", "-118.4505969")), DeliveryRequest("B-Plate salmon (Eng IV)", GeoCoord("34.0687443", "-118.4449195")), DeliveryRequest("Pabst Blue Ribbon beer (Beta Theta Pi)", GeoCoord("34.0685657", "-118.4489289"))};
    cerr << "Before: " << endl;
    cerr << "Depot @ " << depot.latitudeText << " " << depot.longitudeText << endl;
    for(int i = 0; i < drv.size(); i++) {
        cerr << drv[i].item << " @ " << drv[i].location.latitudeText << " " << drv[i].location.longitudeText << endl;
    }
    double oldD, newD;
    optimizer.optimizeDeliveryOrder(depot, drv, oldD, newD);
    cerr << oldD << " " << newD << endl;
    cerr << "After: " << endl;
    cerr << "Depot @ " << depot.latitudeText << " " << depot.longitudeText << endl;
    for(int i = 0; i < drv.size(); i++) {
        cerr << drv[i].item << " @ " << drv[i].location.latitudeText << " " << drv[i].location.longitudeText << endl;
    }
}
