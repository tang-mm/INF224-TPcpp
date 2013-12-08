#include "video.h"


Video::Video() :Source(), duration(0) {}

Video::Video(string _objName) : Source(_objName), duration(0) {}

Video::Video(string _objName, string pathPrefix) : Source(_objName, 0, pathPrefix){}

Video::~Video(){}

/// retourne la duree
int Video::getDuration() const {
    return duration;
}

/// modifie la duree
void Video::setDuration(int _dur) {
    duration = _dur;
}

/// affichage
void Video::print() const {
    cout << "------------" << endl
         << "objName = " << getObjName() << endl
         << "date = " << getDate() << endl
         << "fileName = " << getFileName() << endl
         << "duration = " << duration << endl;
}

/// ecrire la descriptio dans un stringstream
std::stringstream& Video::printToStream(stringstream& stream) const {
    stream << "--objName=" << getObjName()
         << "|--date=" << getDate()
         << "|--fileName=" << getFileName()
         << "|--duration=" << duration << endl;
    return stream;
}


void Video::play() const {
    string* cmd = new string("xdg-open " + getFileName() + " &");
    system(cmd->c_str());
}
