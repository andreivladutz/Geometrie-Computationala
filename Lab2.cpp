#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

struct punct {
    double x, y;
} p1, p2, p3, p4;

//multime de puncte pentru a paritiona
//punctele in multimi disjuncte
class multime {
    punct* M[4];
    int sz;

    bool punctInMultime(punct &P) {
        for (int i = 0; i < sz; i++)
            if (M[i] == &P)
                return true;
        return false;
    }

public :
    multime() : sz(0) {}
    void push(punct &P) {
        M[sz++] = &P;
    }
    multime getComplementara() {
        multime m_noua;

        bool apare[4] = {false};

        if (punctInMultime(p1)) apare[0] = true;
        if (punctInMultime(p2)) apare[1] = true;
        if (punctInMultime(p3)) apare[2] = true;
        if (punctInMultime(p4)) apare[3] = true;

        if (!apare[0]) m_noua.push(p1);
        if (!apare[1]) m_noua.push(p2);
        if (!apare[2]) m_noua.push(p3);
        if (!apare[3]) m_noua.push(p4);

        return m_noua;
    }

    friend ostream& operator<< (ostream&, const multime&);
};

ostream& operator<< (ostream &out, const multime &mt) {
    for (int i = 0; i < mt.sz; i++)
        out << "(" << mt.M[i]->x << ", " << mt.M[i]->y << ") ";
    out << "\n";

    return out;
}

void read(const char *filename)
{
    ifstream in(filename);
    in >> p1.x >> p1.y;
    in >> p2.x >> p2.y;
    in >> p3.x >> p3.y;
    in >> p4.x >> p4.y;
    in.close();
}

double calcDistanta(punct p1, punct p2) {
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

//calculez egalitate cu eroare
bool aproximativ (double val1, double val2) {
    return (fabs(val1 - val2) <= 0.1);
}

/*calculez determinantul de ordin 3 pt arie si returnez aria
    |x1 y1 1|
    |x2 y2 1|
    |x3 y3 1|
*/
double arie(punct a, punct b, punct c) {
    double det = a.x * b.y + b.x * c.y + c.x * a.y -
            b.y * c.x - c.y * a.x - a.y * b.x;

    return 0.5 * fabs(det);
}

/* P = (p1, p2, 0) Q = (q1, q2, 0) si R = (r1, r2, 0) sunt puncte din "planul orizontal"
    produsul lor vectorial va fi de forma (0, 0, |q1 - p1  r1 - p1|)
                                                 |q2 - p2  r2 - p2|
    , unde:
    |q1 - p1  r1 - p1|
    |q2 - p2  r2 - p2|  este determinantul care-mi spune cum este pozitionat punctul R
                        fata de vectorul PQ
    returnez acest determinant, notat DELTA(P, Q, R), unde
        daca DELTA(P, Q, R) > 0, i.e. produsul vectorial este in "sus"
            atunci R este in stanga lui PQ
        daca DELTA(P, Q, R) < 0, i.e. produsul vectorial este in "jos"
            atunci R este in dreapta lui PQ
*/
double deltaBurghiu(punct P, punct Q, punct R) {
    return (Q.x - P.x) * (R.y - P.y) - (R.x - P.x) * (Q.y - P.y);
}

//returnez adevarat daca P apartine segmentului [cpt1cpt2]
bool pctPeSegment(punct cpt1, punct cpt2, punct P) {
    return (calcDistanta(cpt1, P) + calcDistanta(P, cpt2) - calcDistanta(cpt1, cpt2)) <= 0.1;
}

//functie ce verifica daca cele patru puncte formeaza un triunghi
//primind cele 4 arii si returneaza aria triunghiului mare
//altfel returneaza 0
double verifTriunghi(double arii[4]) {
    int indexMax = 0;
    for (int i = 1; i < 4; i++){
        if (arii[indexMax] < arii[i])
            indexMax = i;}

    double sumFaraMax = 0;

    for (int i = 0; i < 4; i++)
        if(i != indexMax)
            sumFaraMax += arii[i];

    if (aproximativ(sumFaraMax, arii[indexMax]))
        return arii[indexMax];
    else
        return 0;
}

//partitionez punctele in doua multimi disjuncte astfel incat
//acoperirea convexa a celor doua multimi sa se intersecteze
void partitionezPuncte(multime &M1, multime &M2) {
    //calculez ariile tuturor triunghiurilor posibile
    //obtinute din cele patru puncte
    double arie123 = arie(p1, p2, p3), arie134 = arie(p1, p3, p4),
        arie124 = arie(p1, p2, p4), arie234 = arie(p2, p3, p4);

    /*CAZ 1 : cele patru puncte sunt coliniare*/
    if (arie123 == 0 && arie134 == 0 && arie124 == 0 && arie234 == 0) {
        M1.push(p1);

        //iau p1 ca un capat si caut celalat capat astfel incat segmentul sa se
        //intersecteze cu segmentul format de celelalte doua puncte
        if (pctPeSegment(p1, p2, p3) || pctPeSegment(p1, p2, p4))
            M1.push(p2);
        else if (pctPeSegment(p1, p3, p2) || pctPeSegment(p1, p3, p4))
            M1.push(p3);
        else if (pctPeSegment(p1, p4, p2) || pctPeSegment(p1, p4, p3))
            M1.push(p4);

        M2 = M1.getComplementara();

        cout << "punctele formeaza un segment\n";
    }
    else {
        /*CAZ 2: trei pcte formeaza un triunghi, al patrulea este in interiorul triunghiului*/
        double vectorArii[4] = {arie123, arie134, arie124, arie234}, maxArie = verifTriunghi(vectorArii);

        //daca maxArie diferit de 0 atunci suntem in cazul 2
        if (maxArie) {
            cout << "punctele formeaza un triunghi cu al patrulea in interior\n";

            if (maxArie == arie123) M1.push(p4);
            if (maxArie == arie134) M1.push(p2);
            if (maxArie == arie124) M1.push(p3);
            if (maxArie == arie234) M1.push(p1);

            M2 = M1.getComplementara();
        }
        /*CAZ 3 : punctele formeaza un patrulater*/
        else {
            cout << "punctele formeaza un patrulater\n";
            //iau vectorul p1p2 si verific cum sunt pozitionate punctele p3 si p4 fata de el

            //p3 si p4 sunt de o parte si de alta a vectorului p1p2
            if (deltaBurghiu(p1, p2, p3) * deltaBurghiu(p1, p2, p4) < 0) {
                M1.push(p1);
                M1.push(p2);
                M2.push(p3);
                M2.push(p4);
            }
            //p3 si p4 sunt de aceeasi parte a vectorului p1p2
            else {
                //iau vectorul p1p4 si verific pozitia lui p2 si p3 fata de el

                //daca p2 si p3 sunt de parti diferite iau multimile {p1, p4} si {p2, p3}
                if (deltaBurghiu(p1, p4, p2) * deltaBurghiu(p1, p4, p3) < 0) {
                    M1.push(p1);
                    M1.push(p4);
                    M2.push(p2);
                    M2.push(p3);
                }
                //daca p2 si p3 sunt de aceeasi parte iau multimile {p1, p3} si {p2, p4}
                else {
                    M1.push(p1);
                    M1.push(p3);
                    M2.push(p2);
                    M2.push(p4);
                }
            }
        }
    }
}


int main() {
    read("puncte.in");
    multime M1, M2;

    partitionezPuncte(M1, M2);

    cout << "prima multime : " << M1 << "a doua multime : " << M2;
}
