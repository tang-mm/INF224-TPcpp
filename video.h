#ifndef VIDEO_H
#define VIDEO_H

#include "source.h"
#include <cstdlib>
#include <sstream>

/**
 * @brief The Video class
 * heritee de la classe Source
 */
class Video : public Source
{
protected:
    int duration;

public:
    /// constructeur
    Video();
    Video(string _objName);
    Video(string _objName, string pathPrefix);

    /// destructeur
    virtual ~Video();

    /// retourne la duree du video
    virtual int getDuration() const;

    /// modifie la duree
    virtual void setDuration(int _dur);

    /// affichage
    virtual void print() const;
    virtual std::stringstream& printToStream(stringstream& stream) const ;

    virtual void play() const;
};

#endif // VIDEO_H
