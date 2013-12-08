#ifndef GROUPE_H
#define GROUPE_H

#include <list>
#include "source.h"
#include "intrusive_ptr.h"

using namespace std;

/**
 * @brief The Groupe class
 * class heritee de std::list
 */
class Groupe : public list< intrusive_ptr<Source> >, public Pointable
{
private:
    string name;

public:
    /// constructeur, donner le nom du groupe
    Groupe(string _name);

    /// destructeur
    virtual ~Groupe();

    /// getter du nom du groupe
    virtual string getGroupName() const;

    /// affichage des elements dans le groupe
    virtual void print() const;

};

#endif // GROUPE_H
