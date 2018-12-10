#include "doubly_linked_list.h"

DList :: DList(vector <punct> &puncte) {
    for (punct P : puncte)
        L.push_back(P);
}
//lista goala initial
DList :: DList() {
}

list <punct> :: iterator DList :: getIterator() {
    return it;
}

void DList :: setIterator(list <punct> :: iterator setIt) {
    it = setIt;
}


void DList :: push_back(punct &P) {
    L.push_back(P);
}

void DList :: push_front(punct &P) {
    L.push_front(P);
}

void DList :: removeCurrent() {
    list <punct> :: iterator eraseable = it;

    if (it == --L.end())
        it = L.begin();
    else
        ++it;

    L.erase(eraseable);
}

//caut nodul de pe pozitia poz
punct& DList :: setCurrentPoz(unsigned poz) {
    it = L.begin();

    for (int currPosition = 0; currPosition < poz; currPosition++)
        ++it;

    return *it;
}
//pe ce pozitie sunt
/*
int DList :: whatPosition() {
    return currPosition;
}
*/
//nodul curent devine anteriorul
punct& DList :: setPrev() {
    if (it == L.begin())
        it = --L.end();
    else
        --it;

    return *it;
}
//nodul curent devine urmatorul
punct& DList :: setNext() {
    if (it == --L.end())
        it = L.begin();
    else
        ++it;

    return *it;
}
//primesc nodul dinainte
punct& DList :: getPrev() {
    if (it == L.begin()) {
        return *(--L.end());
    }
    else {
        list <punct> :: iterator prev = --it;
        ++it;
        return *prev;
    }
}

punct& DList :: getCurr() {
    return *it;
}

//primesc nodul dupa
punct& DList :: getNext() {
    if (it == --L.end()) {
        return *(L.begin());
    }
    else {
        list <punct> :: iterator next = ++it;
        --it;
        return *next;
    }

}

unsigned DList :: size() {
    return L.size();
}
