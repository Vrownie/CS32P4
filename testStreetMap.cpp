#include <iostream>
#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

int main() {
    StreetMap s;
    s.load("/Users/vrownie/Desktop/CS things/CS32/Project 4/Project 4/mapdata.txt");
    
    vector<StreetSegment> ssv;
    cerr << s.getSegmentsThatStartWith(GeoCoord("34.0625329", "-118.4470263"), ssv) << endl;
    for(int i = 0; i < ssv.size(); i++) {
        cerr << ssv[i].name << endl;
        cerr << ssv[i].start.latitudeText << ", " << ssv[i].start.longitudeText << endl;
        cerr << ssv[i].end.latitudeText << ", " << ssv[i].end.longitudeText << endl;
        cerr << endl;
    }
}

