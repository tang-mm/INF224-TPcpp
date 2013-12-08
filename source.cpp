#include "source.h"


using namespace std;

// Constructeur
Source::Source(){
    objName = "";
    date = 0L;
    fileName = "";
}

Source::Source(string _objName) {
    objName = _objName;
    date = 0L;
    fileName = "";
}

Source::Source(string _objName, string pathPrefix) {
    objName = _objName;
    date = 0L;
    fileName = pathPrefix + _objName;
}

Source::Source(string _objName, long _date, string pathPrefix) {
    objName = _objName;
    date = _date;
    fileName = pathPrefix + _objName;
}

Source::~Source() {
    cout << ">> smptr: " << objName << " deleted"<< endl;
}

// accensseur et modifieurs
string Source::getObjName() const {
    return objName;
}

long  Source::getDate() const {
    return date;
}

string Source::getFileName() const {
    return fileName;
}

void Source::setObjName(string _objName) {
    objName = _objName;
}

void Source::setDate(long  _date){
    date = _date;
}

void Source::setFileName(string _fileName){
    fileName = _fileName;
}

// affichage
void Source::print() const {
    cout << "------------" << endl
         << "objName = " << getObjName() << endl
         << "date = " << getDate() << endl
         << "fileName = " << getFileName() << endl;
}

// ecrit la sortie dans un stringstream
std::stringstream& Source::printToStream(std::stringstream& stream) const {
/*   stream << "-- objName = " << getObjName()
         << "| date = " << getDate()
         << "| fileName = " << getFileName() << endl;
         */
    stream << getObjName() <<" "<< getDate()<< " " << getFileName();
   return stream;
}
