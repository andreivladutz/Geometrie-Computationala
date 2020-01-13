#include "js_output.h"


outputJson::outputJson(const string filename):out(filename.c_str(), ios::out){ out << "[\n"; }
outputJson::~outputJson(){
    out << "]\n";
    out.close();
}

void outputJson::printVirgula() {
    out << ",\n";
}

void outputJson::printPunct(punct pct){
    out<<"{\"x\" : \""<<pct.getx()<<"\", \"y\" : \""<<pct.gety()<<"\"}";
}
void outputJson::printSegment(segment segm){
    out<<"\n[ ";
    printPunct(segm.getp1());
    out<<", ";
    printPunct(segm.getp2());
    out<<" ]\n";
}
void outputJson::printTriunghi(triunghi tri){
    out<<"\n[ ";
    printPunct(tri.getp1());
    out<<", ";
    printPunct(tri.getp2());
    out<<", ";
    printPunct(tri.getp3());
    out <<", ";
    if (tri.getHasPoint())
        out << "true";
    else
        out << "false";
    out<<" ]\n";
}


void outputJson::printVectTriunghi(vector <triunghi> &V) {
    out<<"\n[ ";

    for (int i = 0; i < V.size() - 1; i++) {
        printTriunghi(V[i]);
        out << ",\n";
    }

    printTriunghi(V[V.size() - 1]);

    out<<" ]\n";
}

void outputJson::printPoligon(poligon pol){
    out<<"\n[ ";
    for(int i = 0;i < pol.n-1; i++){
        printPunct(pol.varfuri[i]);
        out<<", ";
    }
    printPunct(pol.varfuri[pol.n-1]);
    out<<" ]\n";
}
