#include "photo.h"

Photo::Photo() : Source() {
    place = "not defined";
}

Photo::Photo(string _objName) : Source(_objName) {
    place = "not defined";
}

Photo::Photo(string _objName, string pathPrefix) : Source(_objName, pathPrefix) {
    place = "not defined";
}

Photo::~Photo() {}

string Photo::getPlace() const {
    return place;
}

void Photo::setPlace(string _place) {
    place = _place;
}

/// affichage
void Photo::print() const {
    cout << "------------" << endl
         << "objName = " << getObjName() << endl
         << "date = " << getDate() << endl
         << "fileName = " << getFileName() << endl
         << "place = " << place << endl;
}

/// ecrire dans un stringstream
std::stringstream& Photo::printToStream(stringstream& stream) const {
    stream << "--objName=" << getObjName()
         << "|--date=" << getDate()
         << "|--fileName=" << getFileName()
         << "|--place=" << place << endl;
    return stream;
}

void Photo::play() const {
    string *cmd = new string("xdg-open " + getFileName() + " &");
    system(cmd->c_str());
}
