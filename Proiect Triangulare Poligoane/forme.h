#ifndef FORME_H_INCLUDED
#define FORME_H_INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <fstream>
#include <algorithm>
#include <string>
#include "doubly_linked_list.h"

using namespace std;

class outputJson;
class DList;

class punct{
    double x,y;
public:
    ///Constructori
    punct();
    punct(double val_x, double val_y);
    punct(const punct&);
    ///Functii getter setter
    double getx();
    double gety();
    void setx(double val);
    void sety(double val);
    void setcoords(double val_x, double val_y);

    ///Operators
    friend istream& operator>>(istream &in, punct&);//citirea primeste doi parametri: coordonatele punctului in XoY;
    friend ostream& operator<<(ostream &out, punct&);//afiseaza coordonatele punctului;
    bool operator<(punct& B);//compara punctele in functie de coordonata x; daca A.x == B.x, compara dupa coordonata y;
    bool operator>(punct& B);//analog operator<;
    bool operator<=(punct& B);
    bool operator>=(punct& B);
    bool operator!=(punct& B);
    punct &operator=(const punct &source);
};

class dreapta{
    /*  Dreptele sunt reprezentate sub forma ax + by + c = 0;
        Deci a = coeficientul lui x,
             b = coeficientul lui y,
             c = termenul liber;    */
protected:
    double a,b,c;
public:
    ///Constructori
    dreapta();
    dreapta(double val_a, double val_b, double val_c);
    dreapta(const dreapta &soruce);
    ///Functii getter setter
    double geta(){return a;}
    double getb(){return b;}
    double getc(){return c;}
    void seta(double val){a = val;}
    void setb(double val){b = val;}
    void setc(double val){c = val;}
    void setvalues(double val_a, double val_b, double val_c);
    friend dreapta get_eq(punct A, punct B);
    friend ostream& operator<<(ostream &out, dreapta&);
    ///Operators
    dreapta &operator=(const dreapta &source);
};

class segment:public dreapta{
    punct p1, p2;
public:
    ///Constructori
    segment(){}
    segment(const segment &);
    segment(const punct &_p1, const punct &_p2);
    ///Functii getter, setter
    punct getp1(){return p1;}
    punct getp2(){return p2;}
    void setpoints(const punct &_p1, const punct &_p2);
    friend ostream& operator<<(ostream &out, segment &);
    segment& operator=(segment&);
};

class triunghi{
    punct p1, p2, p3;
    segment p12, p23, p31;
    bool hasPoint = false;
public:
    //Constr Destr
    triunghi();
    triunghi(const punct&, const punct&, const punct&);
    ///Getter setter
    punct getp1(){return p1;}
    punct getp2(){return p2;}
    punct getp3(){return p3;}
    ///Function
    void isInterior(punct&);
    double surface();
    bool getHasPoint();
    ///Operators
    friend ostream& operator<<(ostream &out, triunghi &);
};

class poligon{
    int n;//Nr. de varfuri din poligon
    vector<punct> varfuri;
    vector<segment> seg;
    vector<bool> convexe;
    vector<triunghi> tri;
    friend class outputJson;
public:
    void citire_date(istream &in);
    friend ostream &operator<<(ostream &out, poligon &P);
    void partitieConvex();
    /*
        primeste punctul !CONVEX! de verificat currP
        punctul anterior si urmatorul prevP, nextP
    */
    bool isEar(punct prevP, punct currP, punct nextP);
    bool isConvex(punct prevP, punct currP, punct nextP);


    void triangulare(punct toFind);
    vector<triunghi>& getTriangulare();
};


#endif // FORME_H_INCLUDED
