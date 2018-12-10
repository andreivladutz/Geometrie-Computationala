#include "forme.h"
#include "js_output.h"
using namespace std;

void read(punct &A, poligon &P){
    ifstream in("date.in");
    in>>A;//Punctul care trebuie testat
    P.citire_date(in);
    in.close();
}

int main()
{
    punct A;
    poligon P;
    read(A, P);

    P.triangulare(A);

    outputJson f("Grafica/data.json");
    f.printPoligon(P);
    f.printVirgula();
    f.printPunct(A);
    f.printVirgula();
    f.printVectTriunghi(P.getTriangulare());

    return 0;
}
