#include "provided.h"
#include <list>

#ifndef support_h
#define support_h

std::string calcDir(const double& angle) {
    if (angle > 360) {
        return "";
    } else if (angle >= 337.5) {
        return "east";
    } else if (angle >= 292.5) {
        return "southeast";
    } else if (angle >= 247.5) {
        return "south";
    } else if (angle >= 202.5) {
        return "southwest";
    } else if (angle >= 157.5) {
        return "west";
    } else if (angle >= 112.5) {
        return "northwest";
    } else if (angle >= 67.5) {
        return "north";
    } else if (angle >= 22.5) {
        return "northeast";
    } else if (angle >= 0) {
        return "east";
    } else {
        return "";
    }
}

bool ParseOneDelivery(const bool& isFinal, const std::string& item, const std::list<StreetSegment>& ssl, std::vector<DeliveryCommand>& commands, double& dist) {
    double totalDist = 0;
    
    std::string streetNow = ssl.front().name, dir;
    GeoCoord startCoord = ssl.front().start, endCoord = ssl.front().end;
    double angleFirst = angleOfLine(ssl.front()), distNow = distanceEarthMiles(ssl.front().start, ssl.front().end);
    DeliveryCommand currentCommand;
    for(auto ii = next(ssl.begin()); ii != ssl.end(); ii++) {
        if((*ii).name == streetNow) { //same street
            distNow += distanceEarthMiles((*ii).start, (*ii).end);
        } else { //new street
            endCoord = (*ii).start;
            dir = calcDir(angleFirst);
            if(dir == "") return false;
            currentCommand.initAsProceedCommand(dir, streetNow, distNow);
            commands.push_back(currentCommand);
            streetNow = (*ii).name;
            double angleNew = angleBetween2Lines(*prev(ii),*ii);
            if (angleNew >= 1 && angleNew < 180) {
                currentCommand.initAsTurnCommand("left", streetNow);
            } else if (angleNew >= 180 && angleNew < 359) {
                currentCommand.initAsTurnCommand("right", streetNow);
            }
            commands.push_back(currentCommand);
            angleFirst = angleOfLine(*ii);
            totalDist += distNow;
            distNow = distanceEarthMiles((*ii).start, (*ii).end);;
        }
    }
    dir = calcDir(angleFirst);
    currentCommand.initAsProceedCommand(dir, streetNow, distNow);
    commands.push_back(currentCommand);
    if(!isFinal) {
        currentCommand.initAsDeliverCommand(item);
        commands.push_back(currentCommand);
    }
    dist = totalDist;
    return true;
}

#endif /* support_h */
