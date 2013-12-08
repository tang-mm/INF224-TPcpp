#ifndef PHOTO_H
#define PHOTO_H

#include "source.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/**
 * @brief The Photo class
 * heritee de la classe Source
 */
class Photo : public Source
{
protected:
    string place;

public:
    /// constructeur
    Photo();
    Photo(string _objName);
    Photo(string _objName, string pathPrefix);

    /// destructeur
    virtual ~Photo();

    /// getter
    virtual string getPlace() const;

    /// setter
    virtual void setPlace(string _place);

    /// affichage
    virtual void print() const;
    virtual std::stringstream& printToStream(stringstream& stream) const;

    virtual void play() const;
};


#endif // PHOTO_H
