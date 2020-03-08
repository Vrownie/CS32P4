#include <string>
#include <vector>
#include <functional>
#include <fstream>

#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;

private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_mapping;
};

StreetMapImpl::StreetMapImpl() {}

StreetMapImpl::~StreetMapImpl() {}

bool StreetMapImpl::load(string mapFile)
{
    ifstream ifs;
    ifs.open(mapFile.c_str(), ios::in);
    if(!ifs) {
        cerr << "Bad file name! " << endl;
        return false;
    }
    
    while(true) {
        string name;
        GeoCoord* a;
        GeoCoord* b;
        
        string in;
        if(getline(ifs, in)) name = in;
        else break;
        
        int k;
        ifs >> k;
        ifs.ignore(10000, '\n');
        
        for(int i = 0; i < k; i++) {
            getline(ifs, in);
            int n = 0;
            string sa[4];
            for(int j = 0; j < in.size(); j++) {
                if (in[j] == ' ') n++;
                else sa[n] += in[j];
            }
            a = new GeoCoord(sa[0], sa[1]);
            b = new GeoCoord(sa[2], sa[3]);
            if (m_mapping.find(*a) == nullptr) m_mapping.associate(*a, vector<StreetSegment>());
            m_mapping.find(*a)->push_back(StreetSegment(*a, *b, name));
            if (m_mapping.find(*b) == nullptr) m_mapping.associate(*b, vector<StreetSegment>());
            m_mapping.find(*b)->push_back(StreetSegment(*b, *a, name));
        }
    }
    
//    auto a = m_mapping.dump();
//    int counter = 0;
//    for(auto i = a.begin(); i != a.end(); i++) {
//        for(auto j = (*i).begin(); j != (*i).end(); j++) {
//            counter++;
//            std::cerr << counter << ": " << (*j)->key.latitudeText << " " << (*j)->key.longitudeText << std::endl;
//            for(auto k = (*j)->value.begin(); k != (*j)->value.end(); k++) {
//                std::cerr << (*k).name << endl;
//                std::cerr << (*k).start.latitudeText << " " << (*k).start.longitudeText << std::endl;
//                std::cerr << (*k).end.latitudeText << " " << (*k).end.longitudeText << std::endl;
//                std::cerr << std::endl;
//            }
//        }
//    }
    
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    auto temp = m_mapping.find(gc);
    if(temp == nullptr) return false;
    else {
        segs = *temp;
        return true;
    }
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
