#include "groupe.h"

Groupe::Groupe(string _name) {
    name = _name;
}

Groupe::~Groupe() {
    cout << ">> smptr: Group "<< name << " deleted "<< endl;
}

string Groupe::getGroupName() const {
    return name;
}

void Groupe::print() const {

    cout << "< " << getGroupName() << " >" << endl;
    for (list< intrusive_ptr<Source> >::const_iterator it = this->begin(); it != this->end(); ++it) {
        (*it)->print();
    }

}
