#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const double PI = 3.1415926;

struct punct {
    double x, y;
} p1, p2, p3, p4;

void read(const char *filename)
{
    ifstream in(filename);
    in >> p1.x >> p1.y;
    in >> p2.x >> p2.y;
    in >> p3.x >> p3.y;
    in >> p4.x >> p4.y;
    in.close();
}

void print()
{
    cout << p1.x << " " << p1.y << endl;
    cout << p2.x << " " << p2.y << endl;
    cout << p3.x << " " << p3.y << endl;
    cout << p4.x << " " << p4.y << endl;
}

double calcDistanta(punct p1, punct p2) {
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

bool aproximativ (double val1, double val2) {
    return (fabs(val1 - val2) <= 0.1);
}

double produsScalar(punct vect1, punct vect2) {
    return vect1.x * vect2.x + vect1.y * vect2.y;
}

double norma(punct vect) {
    return sqrt(produsScalar(vect, vect));
}

//verific pozitia pct-ului  A4 fata de cercul circumscris
//triunghiului format de A1 A2 si A3
void pctFataDeCerc() {
    double mUnghiA2, mUnghiA4, cosA2, cosA4;

    punct vectA2A1 = {p1.x - p2.x, p1.y - p2.y}, vectA2A3 = {p3.x - p2.x, p3.y - p2.y},
        vectA4A1 = {p1.x - p4.x, p1.y - p4.y}, vectA4A3 = {p3.x - p4.x, p3.y - p4.y};

    cosA2 = produsScalar(vectA2A1, vectA2A3) / (norma(vectA2A1) * norma(vectA2A3));
    cosA4 = produsScalar(vectA4A1, vectA4A3) / (norma(vectA4A1) * norma(vectA4A3));

    mUnghiA2 = acos(cosA2);
    mUnghiA4 = acos(cosA4);

    if (aproximativ(mUnghiA2 + mUnghiA4, PI))
        cout << "Punctul (" << p4.x << ", " << p4.y << ") se afla pe cercul circumscris triunghiului A1A2A3\n";
    else if (mUnghiA2 + mUnghiA4 < PI)
        cout << "Punctul (" << p4.x << ", " << p4.y << ") se afla in afara cercului circumscris triunghiului A1A2A3\n";
    else if (mUnghiA2 + mUnghiA4 > PI)
        cout << "Punctul (" << p4.x << ", " << p4.y << ") se afla inauntrul cercului circumscris triunghiului A1A2A3\n";
}

bool patrulaterCircumscriptibil() {
    double segA1A2 = calcDistanta(p1, p2), segA3A4 = calcDistanta(p3, p4),
        segA1A4 = calcDistanta(p1, p4), segA2A3 = calcDistanta(p2, p3);

    return aproximativ(segA1A2 + segA3A4, segA1A4 + segA2A3);
}

int main() {
    read("puncte.in");
    pctFataDeCerc();

    if (patrulaterCircumscriptibil())
        cout << "Patrulaterul este circumscriptibil\n";
    else
        cout << "Patrulaterul nu este circumscriptibil\n";
}
