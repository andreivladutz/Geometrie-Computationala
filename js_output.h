#ifndef JS_OUTPUT_H_INCLUDED
#define JS_OUTPUT_H_INCLUDED

#include "forme.h"

using namespace std;

class punct;
class segment;
class triunghi;
class poligon;

class outputJson{
    ofstream out;
public:
    ///Constr destr
    outputJson(const string _filename);
    ~outputJson();
    ///Functions
    void printPunct(punct pct);
    void printSegment(segment segm);
    void printTriunghi(triunghi tri);
    void printVectTriunghi(vector <triunghi>&);

    void printPoligon(poligon pol);
    void printVirgula();
};



#endif // JS_OUTPUT_H_INCLUDED
