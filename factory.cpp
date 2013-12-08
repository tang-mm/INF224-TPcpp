#include "factory.h"

typedef  map<string, intrusive_ptr<Source> > SourceMap;
typedef  map<string, intrusive_ptr<Groupe> > GroupeMap;
typedef list<intrusive_ptr<Source> > SourceList;

Factory::Factory(){}

Factory::~Factory() { }


intrusive_ptr<Photo> Factory::createPhoto(const string &objName, const string& pathPrefix) {
    intrusive_ptr<Photo> obj = new Photo(objName, pathPrefix);
    tabElem[objName] = obj;

    return obj;
}

intrusive_ptr<Video> Factory::createVideo(const string &objName, const string& pathPrefix) {
    intrusive_ptr<Video> obj = new Video(objName, pathPrefix);
    tabElem[objName] = obj;

    return obj;
}

intrusive_ptr<Film> Factory::createFilm(const string &objName, const string& pathPrefix) {
    intrusive_ptr<Film> obj = new Film(objName, pathPrefix);
    tabElem[objName] = obj;

    return obj;
}

intrusive_ptr<Groupe> Factory::createGroup(const string &grpName) {
    intrusive_ptr<Groupe> grp = new Groupe(grpName);
    tabGrp[grpName] = grp;

    return grp;
}


void Factory::remove(const string &name) {
    /// chercher d'abord dans le map des multimedia
    SourceMap::iterator its = tabElem.find(name);

    if (its != tabElem.end()) {
        tabElem.erase(its);  // l'enlever du map, decremente le compteur de smptr
        cout<< "Factory: [" << name << "] removed !" << endl;

        /// chercher s'il appartient à une table
        GroupeMap::iterator itg;
        for (itg = tabGrp.begin(); itg != tabGrp.end(); itg++ ) { // iteration sur tous les pairs <string, groupe>

            intrusive_ptr<Groupe> grp = itg->second;  // get l'obj Groupe
            cout<< "-- " + grp->getGroupName() <<endl;

            // iterator sur Groupe
            for (SourceList::iterator itList = grp->begin(); itList != grp->end(); ) {
                string nm = itList->get()->getObjName();

                if ( nm == name) {   // si trouvé dans la liste (groupe)
                    SourceList::iterator it2 = itList; // save current position
                    it2++;                           // move to the next position
                    grp->erase(itList);             // enlever de la liste
                    (*itList) = NULL;               // decremente smptr
                    itList = it2;                   // move to the next position
                    cout<< "Factory: ["<< name << "] in the group ["<< grp->getGroupName() <<"] removed !" << endl;
                }else {
                    itList++;
                }
            }
        }
    }else { /// si non trouvé dans tabElem, alors "name" est un groupe, chercher dans le map des groupes
        GroupeMap::iterator itg;
        itg = tabGrp.find(name);
        if (itg != tabGrp.end()) {
            tabGrp.erase(itg); // l'enlever du map et decremente smptr

            cout<< "Factory: [" << name << "] deleted!" << endl;
        }
    }

}

void Factory::find(const string &name) const {
    SourceMap::const_iterator its = tabElem.find(name);

    if (its != tabElem.end()) { // afficher
        (its->second)->print();
    }else {
        GroupeMap::const_iterator itg = tabGrp.find(name);
        if (itg != tabGrp.end()) {
            itg->second->print();
        }else {
            cout<<"Factory: !! [" << name <<"] doesn't exist! " <<endl;
        }
    }

}

stringstream& Factory::findAndSendToStream(const string &name, stringstream& stream) const {
    SourceMap::const_iterator its = tabElem.find(name);

    if (its != tabElem.end()) { // afficher
        (its->second)->printToStream(stream);
    } /*else {
        GroupeMap::const_iterator itg = tabGrp.find(name);
        if (itg != tabGrp.end()) {
            itg->second->printToStream(stream);
        }else {
            cout<<"Factory: !! [" << name <<"] doesn't exist! " <<endl;
        }
    }*/

    return stream;
}

void Factory::play(const string &name) const {
    SourceMap::const_iterator its = tabElem.find(name);

    if (its != tabElem.end()) { // play
        (its->second)->play();
    }else {
            cout<<"Factory: !! [" << name <<"] doesn't exist! " <<endl;
    }

}

void Factory::print() const {
    for (SourceMap::const_iterator its = tabElem.begin(); its != tabElem.end(); its++ ) {
        (its->second)->print();
    }
}

const SourceMap& Factory::getTabElem() const {
    return tabElem;
}

const GroupeMap& Factory::getTabGrp() const {
    return tabGrp;
}
