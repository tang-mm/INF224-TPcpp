#ifndef FILM_H
#define FILM_H

#include <iostream>
#include "video.h"

using namespace std;

/**
 * @brief The Film class
 * heritee de la classe Video
 */
class Film : public Video
{
private:
    int* tabDur;
    int sizeTab;

public:
    /// constructeur
    Film();
    Film(string _objName);
    Film(string _objName, string pathPrefix);
    Film(string _objName, string pathPrefix, int _dur);
    Film(const Film&);

    /// redefinition de l'operateur =
    Film& operator=(const Film&);

    /// destructeur
    virtual ~Film();

    /// setter
    virtual void setTabDur(const int* _tabDur, int size);

    /// getter du tableau de durees
    virtual const int* getTabDur() const;

    /// getter du nombre de chapitres
    virtual int getNbChap() const;

    /// affichage
    virtual void print() const;    
    virtual std::stringstream& printToStream(stringstream& stream) const ;

    virtual void play() const;

};


#endif // FILM_H
