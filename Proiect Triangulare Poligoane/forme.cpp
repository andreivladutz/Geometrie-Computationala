#include "forme.h"

punct::punct(){
    x = y = 0;
}
punct::punct(double val_x, double val_y){
    x = val_x;
    y = val_y;
}
punct::punct(const punct& source){
    x = source.x;
    y = source.y;
}
double punct::getx(){
    return x;
}
double punct::gety(){
    return y;
}
void punct::setx(double val){
    x = val;
}
void punct::sety(double val){
    y = val;
}
void punct::setcoords(double val_x, double val_y){
    x = val_x;
    y = val_y;
}

bool punct::operator<(punct &B){
    if(x == B.getx())
        return y < B.gety();
    return x < B.getx();
}
bool punct::operator>(punct &B){
    if(x == B.getx())
        return y > B.gety();
    return x > B.getx();
}
bool punct::operator<=(punct &B){
    return !(*this>B);
}
bool punct::operator>=(punct &B){
    return !(*this<B);
}

bool punct::operator!=(punct& B) {
    return ((x != B.x) || (y != B.y));
}

istream& operator>>(istream &in, punct& pct){
    int a,b;
    in>>a>>b;
    pct.setx(a);
    pct.sety(b);
    return in;
}
ostream& operator<<(ostream &out, punct& pct){
    out<<"("<<pct.x<<", "<<pct.y<<")";
    return out;
}
punct &punct::operator=(const punct &source){
    x = source.x;
    y = source.y;
    return *this;
}

double dist(punct A, punct B){
    //Calculeaza distanta dintre doua puncte A si B
    return sqrt( (A.getx()-B.getx())*(A.getx()-B.getx()) + (A.gety()-B.gety())*(A.gety()-B.gety()) );
}


dreapta::dreapta(){
    a = b = c = 0;
}
dreapta::dreapta(const dreapta& source){
    a = source.a;
    b = source.b;
    c = source.c;
}
dreapta::dreapta(double val_a, double val_b, double val_c){
    a = val_a;
    b = val_b;
    c = val_c;
}
void dreapta::setvalues(double val_a, double val_b, double val_c){
    a = val_a;
    b = val_b;
    c = val_c;
}
dreapta &dreapta::operator=(const dreapta &source){
    a = source.a;
    b = source.b;
    c = source.c;
    return *this;
}
ostream& operator<<(ostream &out, dreapta& d){
    bool check = false;
    if(d.a < 0){
        out<<"("<<d.a<<")x";
        check = true;
    }
    if(d.a > 0){
        out<<d.a<<"x";
        check = true;
    }

    if(d.b < 0 || !check){
        out<<d.b<<"y";
        check = true;
    }
    else if(d.b > 0)
        out<<"+"<<d.b<<"y";

    if(d.c < 0 || !check)
        out<<d.c<<"=0\n";
    else if(d.c > 0)
            out<<"+"<<d.c<<"=0\n";
        else
            out<<"=0\n";

    return out;
}

dreapta get_eq(punct A, punct B){
    dreapta d;
    /*Calculand determinantul dreptei formate de A(x1, y1) si B(x2, y2):
    |x  y  1|
    |x1 y1 1| = 0
    |x2 y2 1|
    Iese ca (y1-y2)x + (x2-x1)y + y2x1-y1x2 = 0;
    Vom retine fiecare coeficient in variabilele:   a - pt x;
                                                    b - pt y;
                                                    c - pt termenul liber;    */
    d.a = A.gety()-B.gety();
    d.b = B.getx()-A.getx();
    d.c = B.gety()*A.getx() - A.gety()*B.getx();
    return d;
}


segment::segment(const segment &source):dreapta(source){
    p1 = source.p1;
    p2 = source.p2;
}
segment::segment(const punct &_p1, const punct &_p2){
    p1 = _p1;
    p2 = _p2;
    dreapta aux = get_eq(p1, p2);
    a = aux.geta();
    b = aux.getb();
    c = aux.getc();
}
ostream &operator<<(ostream &out, segment &segm){
    //out<<dynamic_cast<dreapta&>(segm);
    out<<segm.p1<<" "<<segm.p2<<"\n";
    return out;
}
segment &segment::operator=(segment& source){
    p1 = source.p1;
    p2 = source.p2;
    a = source.a;
    b = source.b;
    c = source.c;
    return *this;
}
void segment::setpoints(const punct &_p1, const punct &_p2){
    p1 = _p1;
    p2 = _p2;
    dreapta aux = get_eq(p1, p2);
    a = aux.geta();
    b = aux.getb();
    c = aux.getc();
}




triunghi::triunghi() {
    p1.setcoords(0, 0);
    p2.setcoords(0, 0);
    p3.setcoords(0, 0);
}

triunghi::triunghi(const punct &a, const punct &b, const punct &c) {
    p1 = a;
    p2 = b;
    p3 = c;

    p12.setpoints(p1, p2);
    p23.setpoints(p2, p3);
    p31.setpoints(p3, p1);
}

//determinant pentru testul de orientare
double determinant(punct &a,  punct &b,  punct &P) {
    double t1 = (b.getx() - a.getx()) * (P.gety() - a.gety());
    double t2 = (b.gety() - a.gety()) * (P.getx() - a.getx());
    return t1 - t2;
}

//functie pentru daca e in interiorul triunghiului
void triunghi::isInterior(punct &P) {
    hasPoint = false;
    double detp1p2p3 = determinant(p1, p2, p3);
    double prodp1p2 = detp1p2p3 * determinant(p1, p2, P);
    double prodp2p3 = detp1p2p3 * determinant(p2, p3, P);
    double prodp3p1 = detp1p2p3 * determinant(p3, p1, P);

    if(prodp1p2 >= 0 && prodp2p3 >= 0 && prodp3p1 >=0) {
        if(!prodp1p2)
            if(dist(p1, p2) == (dist(p1, P) + dist(p2, P))) {
                //cout << "Punctul se afla pe segmentul " << p12 << endl;
                //fout << p12 << endl;
            }
        if(!prodp2p3)
            if(dist(p2, p3) == (dist(p2, P) + dist(p3, P))) {
               // cout << "Punctul se afla pe segmentul " << p23 << endl;
                //fout << p23 << endl;
            }
        if(!prodp3p1)
            if(dist(p3, p1) == (dist(p3, P) + dist(p1, P))) {
                //cout << "Punctul se afla pe segmentul " << p31 << endl;
                //fout << p31 << endl;
            }
        hasPoint = true;
    }
}

bool triunghi::getHasPoint() {
    return hasPoint;
}

double triunghi::surface(){
    return abs(determinant(p1,p2,p3))/2;
}

ostream &operator<<(ostream &out, triunghi &T){
    punct aux1 = T.getp1(), aux2 = T.getp2(), aux3 = T.getp3();
    out<<"{ "<<aux1<<", "<<aux2<<", "<<aux3<<" }\n";
    return out;
}



void poligon::citire_date(istream &in){
    in>>n;
    punct x,y;
    in>>x;//citim si retinem primul punct din poligon
    varfuri.push_back(x);
    for(int i = 1;i < n; i++){
        in>>y;
        varfuri.push_back(y);
        segment xy(x,y);
        seg.push_back(xy);
        x = y;
    }//Nr. de varfuri din poligon
    segment yx(y,*varfuri.begin());
    seg.push_back(yx);

}
ostream &operator<<(ostream &out, poligon &P){
    out<<P.n<<" Varfuri: ";
    for(punct p:P.varfuri)
        out<<p<<" ";
    out<<"\nSegmente:\n";
    for(segment s:P.seg)
        out<<s;
    return out;
}

bool poligon :: isConvex(punct prevP, punct currP, punct nextP) {
    return (determinant(prevP, currP, nextP) > 0);
}

//partitionam varfurile in convexe si concave
void poligon :: partitieConvex() {
    int vlenght = varfuri.size();

    convexe.resize(varfuri.size());

    //testam separat pentru primul si ultimul varf
    convexe[vlenght - 1] = isConvex(varfuri[vlenght - 2], varfuri[vlenght - 1], varfuri[0]);
    convexe[0] = isConvex(varfuri[vlenght - 1], varfuri[0], varfuri[1]);

    for(int i = 1; i < vlenght - 1; i++)
        convexe[i] = isConvex(varfuri[i - 1], varfuri[i], varfuri[i + 1]);
}

/*
    daca varful convex nu contine niciun alt varf in interiorul triunghiului
    format de el si vecinii sai atunci este EAR

    este suficient sa verificam varfurile concave in interiorul triunghiului
*/
bool poligon :: isEar(punct prevP, punct currP, punct nextP) {
    triunghi T(prevP, currP, nextP);

    for (int i = 0; i < convexe.size(); i++)
        //varful nu este convex deci este concav
        //de asemenea trebuie sa ne asiguram ca varfulpozitii.pop_front();
        //nu se afla printre cele care formeaza triunghiul
        if (!convexe[i] && varfuri[i] != prevP && varfuri[i] != nextP)
        {
            T.isInterior(varfuri[i]);

            if (T.getHasPoint())
                return false;
        }
    return true;
}

void poligon :: triangulare(punct toFind) {
    /*
        TRIANGULAM:
            "La fiecare pas eliminam cate o ureche;"
                                            -Van Gogh
    */
    DList noduri(varfuri);

    //LISTA DE ITERATORI LA LISTA DE NODURI
    //(ca sa am legatura directa intre EARS si noduri)
    list <list<punct> :: iterator> EARS;
    list <int> pozitii;
    vector <bool> already_ear;

    already_ear.resize(varfuri.size());

    partitieConvex();

    noduri.setCurrentPoz(0);

    //PARCURG LISTA DE NODURI SI DACA NODUL CURENT ESTE CONVEX SI URECHE
    //II SALVEZ ITERATORUL, IL SETEZ CA FIIND URECHE IN VECTOR DE APARITII
    //SI II SALVEZ POZITIA IN VECTORUL ORIGINAL IN LISTA DE POZITII
    for (int i = 0; i < convexe.size(); i++, noduri.setNext()) {
        if (convexe[i] && isEar(noduri.getPrev(), noduri.getCurr(), noduri.getNext())){
            EARS.push_back(noduri.getIterator());
            already_ear[i] = true;
            pozitii.push_back(i);
        }
    }

    while(noduri.size() > 3){
        //currentPos = pozitia originala a nodului in vectorul de noduri
        int currentPos = pozitii.front();

        //ma duc in vectorul de noduri pe pozitia pe care se afla prima URECHE gasita
        //mutarea se face in O(1) iteratorul fiind doar o adresa
        noduri.setIterator(EARS.front());

        cout <<"EAR gasit: "<< *EARS.front() << "\n";

        triunghi t(noduri.getPrev(), noduri.getCurr(), noduri.getNext());

        cout << "Formam triunghiul " << t << "\n";

        //VERIFIC DACA PUNCTUL DE CAUTAT SE AFLA IN INTERIORUL TRIUNGHIULUI
        //(functia seteaza hasPoint pe true)
        t.isInterior(toFind);
        if(t.getHasPoint())
            cout << "Punctul " << toFind<<" a fost gasit in triunghiul: "<< t << "\n" ;
        tri.push_back(t);

        noduri.removeCurrent();
        //L-am eliminat pe Vi si verificam daca Vi+1 a devenit si el ureche
        //daca nu e deja ureche verificam daca intre timp a devenit ureche

        //obtin urmatoarea pozitie (originala) a nodului in functie de pozitia lui Vi
        //circular(daca Vi este defapt Vn[ultimul varf] atunci vi+1 este v0)
        int nextPosition = (currentPos == convexe.size() - 1) ? 0 : currentPos + 1;

        if (!already_ear[nextPosition] && isConvex(noduri.getPrev(), noduri.getCurr(), noduri.getNext())){
            convexe[nextPosition] = true;

            //DACA E URECHE CU NOII VECINI(FARA Vi)
            if(isEar(noduri.getPrev(), noduri.getCurr(), noduri.getNext())){
                //cout<<noduri.getCurr();

                EARS.push_back(noduri.getIterator());
                //a devenit ureche
                already_ear[nextPosition] = true;
                pozitii.push_back(nextPosition);
            }
        }
        //Acum verificam daca Vi-1 a devenit si el ureche
        noduri.setPrev();


        //simetric cu nextPosition
        int prevPosition = (currentPos == 0) ? convexe.size() - 1 : currentPos - 1;

        if (!already_ear[prevPosition] && isConvex(noduri.getPrev(), noduri.getCurr(), noduri.getNext())){
           convexe[prevPosition] = true;

            if(isEar(noduri.getPrev(), noduri.getCurr(), noduri.getNext())){
                EARS.push_back(noduri.getIterator());
                already_ear[prevPosition] = true;
                pozitii.push_back(prevPosition);
            }
        }

        pozitii.pop_front();
        EARS.pop_front();

    }

    //FORMEZ TRIUNGHI CU ULTIMELE 3 NODURI RAMASE
    triunghi t(noduri.getPrev(), noduri.getCurr(), noduri.getNext());

    cout << "Formam triunghiul " << t << "\n";

    //VERIFIC DACA PUNCTUL DE CAUTAT SE AFLA IN INTERIORUL TRIUNGHIULUI
    //(functia seteaza hasPoint pe true)
    t.isInterior(toFind);
    if(t.getHasPoint())
        cout << "Punctul " << toFind<<" a fost gasit in triunghiul: "<< t << "\n" ;
    tri.push_back(t);
}

vector<triunghi>& poligon :: getTriangulare() {
    return tri;
}
