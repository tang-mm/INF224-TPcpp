#include "film.h"

Film::Film() : Video() {
    tabDur = NULL;
    sizeTab = 0;
}

Film::Film(string _objName) : Video(_objName) {
    tabDur = NULL;
    sizeTab = 0;
}

Film::Film(string _objName, string pathPrefix)
    : Video(_objName, pathPrefix) {
    duration = 0;
    tabDur = NULL;
    sizeTab = 0;
}

Film::Film(string _objName, string pathPrefix, int _dur)
    : Video(_objName, pathPrefix) {
    duration = _dur;
    tabDur = NULL;
    sizeTab = 0;
}

/// constructeur a partir d'une autre instance
Film::Film(const Film & from) :Video(from){
    sizeTab = from.sizeTab;
    tabDur = new int[sizeTab];
    for (int i = 0; i < sizeTab; i++) {
            tabDur[i] = from.tabDur[i];
    }
}

/// redefinition de l'operateur =, copie profonde
Film& Film::operator =(const Film& from) {
    Video::operator=(from);
    sizeTab = from.sizeTab;
    tabDur = new int[sizeTab];
    for (int i = 0; i < sizeTab; i++) {
            tabDur[i] = from.tabDur[i];
    }
    return *this;
}

/// destructeur
Film::~Film() {}

/// setter, copie d'un tableau de duree
void Film::setTabDur(const int* _tabDur, int size) {
    if (_tabDur != NULL) {
        cout << "copie du tableau" << endl;
        tabDur = new int[size];
        sizeTab = size;

        for (int i = 0; i < size; i++) {
            tabDur[i] = _tabDur[i];
        }
    }else {
        cout << "tableau vide" << endl;
        tabDur = NULL;
        sizeTab = 0;
    }
}

/// getter
const int* Film::getTabDur() const {
    return tabDur;
}

/// get nombre de chapitres
int Film::getNbChap() const {
    return sizeTab;
}

/// affichage
void Film::print() const {
    cout << "------------" << endl
         << "objName = " << getObjName() << endl
         << "date = " << getDate() << endl
         << "fileName = " << getFileName() << endl
         << "duration = " << getDuration() << endl
         << "sizeTab = " << getNbChap() << endl;

    // boucle pour afficher le tableau de duree
    for (int i = 0; i < sizeTab; i++) {
        cout << "Chap " << i << " duree: " << tabDur[i] << endl;
    }

}

std::stringstream& Film::printToStream(stringstream& stream) const {
    stream << "--objName=" << getObjName()
         << "|--date=" << getDate()
         << "|--fileName=" << getFileName()
         << "|--duration=" << getDuration()
         << "|--sizeTab=" << getNbChap() ;

    // boucle pour afficher le tableau de duree
    for (int i = 0; i < sizeTab; i++) {
        stream << "|--Chap-" << i << ":duree=" << tabDur[i] ;
    }
    stream << endl;

    return stream;
}


void Film::play() const {
    string* cmd = new string("xdg-open " + getFileName() + " &");
    system(cmd->c_str());
}
