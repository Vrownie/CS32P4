#include <iostream>
#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

int main() {
    StreetMap* sp = new StreetMap();
    sp->load("/Users/vrownie/Desktop/CS things/CS32/Project 4/Project 4/mapdata.txt");
    
    DeliveryPlanner dp(sp);
    GeoCoord depot("34.0625329", "-118.4470263");
    vector<DeliveryRequest> drv = {DeliveryRequest("Chicken tenders (Sproul Landing)", GeoCoord("34.0712323", "-118.4505969")), DeliveryRequest("B-Plate salmon (Eng IV)", GeoCoord("34.0687443", "-118.4449195")), DeliveryRequest("Pabst Blue Ribbon beer (Beta Theta Pi)", GeoCoord("34.0685657", "-118.4489289"))};
    vector<DeliveryRequest> drv2 = {DeliveryRequest("Sardines", GeoCoord("34.0636671", "-118.4461842"))};
    vector<DeliveryRequest> drv3 = {DeliveryRequest("Dub Dub", GeoCoord("34.0625329", "-118.4470263"))};
    vector<DeliveryCommand> dcv;
    double d;
    dp.generateDeliveryPlan(depot, drv, dcv, d);
    for(int i = 0; i < dcv.size(); i++) {
        cout << dcv[i].description() << endl;
    }
    cout << "Distance: " << d << endl;
}
