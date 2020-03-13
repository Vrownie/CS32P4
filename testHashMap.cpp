#include <iostream>
#include "provided.h"
#include "ExpandableHashMap.h"

using namespace std;

unsigned int hash(const string& s) { return std::hash<std::string>()(s); }

int main() {
    ExpandableHashMap<string,double> nameToGPA(0.2);
    nameToGPA.associate("Carey", 3.5);
    nameToGPA.associate("David", 3.99);
    nameToGPA.associate("Abe", 3.2);
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr) *davidsGPA = 1.5;
    nameToGPA.associate("Carey", 4.0);
    double* lindasGPA = nameToGPA.find("Linda");
    double* careysGPA = nameToGPA.find("Carey");
    double* abesGPA = nameToGPA.find("Abe");
    davidsGPA = nameToGPA.find("David");
    
    if (lindasGPA == nullptr)
        cerr << "Linda is not in the roster!" << endl;
    else
        cerr << "Linda’s GPA is: " << *lindasGPA << endl;
    
    if (careysGPA == nullptr)
        cerr << "Carey is not in the roster!" << endl;
    else
        cerr << "Carey’s GPA is: " << *careysGPA << endl;
    
    if (abesGPA == nullptr)
        cerr << "Abe is not in the roster!" << endl;
    else
        cerr << "Abe’s GPA is: " << *abesGPA << endl;
    
    if (davidsGPA == nullptr)
        cerr << "David is not in the roster!" << endl;
    else
        cerr << "David’s GPA is: " << *davidsGPA << endl;
    
    cerr << "There are " << nameToGPA.size() << " students. " << endl;
    
    nameToGPA.reset();
    
    cerr << endl << "After resetting: " << endl;
    
    nameToGPA.associate("Young", 1.0);
    double* youngsGPA = nameToGPA.find("Young");
    careysGPA = nameToGPA.find("Carey");
    
    if (youngsGPA == nullptr)
        cerr << "Young is not in the roster!" << endl;
    else
        cerr << "Young’s GPA is: " << *youngsGPA << endl;
    
    if (careysGPA == nullptr)
        cerr << "Carey is not in the roster!" << endl;
    else
        cerr << "Carey’s GPA is: " << *careysGPA << endl;
    
    cerr << "There are " << nameToGPA.size() << " students. " << endl;
}
