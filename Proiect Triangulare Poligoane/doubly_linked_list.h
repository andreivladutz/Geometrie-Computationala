#ifndef DOUBLY_LINKED_LIST_H_INCLUDED
#define DOUBLY_LINKED_LIST_H_INCLUDED

#include "js_output.h"

using namespace std;

class punct;

class DList {
    list <punct> L;
    list <punct> :: iterator it;
    //unsigned currPosition;
public :
    //initializare dintr-un vector(pentru poligon)
    DList(vector <punct> &P);
    //lista goala initial
    DList();

    void push_back(punct &P);
    void push_front(punct &P);
    unsigned size();

    list <punct> :: iterator getIterator();
    void setIterator(list <punct> :: iterator setIt);

    //sterg varful curent iar iteratorul se va afla
    //pe pozitia urmatoare dupa stergere (daca sterg ultima pozitie
    //iteratorul se muta pe prima pozitie din lista
    void removeCurrent();

    //caut nodul de pe pozitia poz
    punct& setCurrentPoz(unsigned poz);
    //returnez nodul de pe pozitia curenta
    punct& getCurr();

    //pe ce pozitie sunt
    //int whatPosition();

    //nodul curent devine anteriorul
    punct& setPrev();
    //nodul curent devine urmatorul
    punct& setNext();
    //primesc nodul dinainte
    punct& getPrev();
    //primesc nodul dupa
    punct& getNext();
};

#endif // DOUBLY_LINKED_LIST_H_INCLUDED
