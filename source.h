#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <sstream>
#include"intrusive_ptr.h"

using namespace std;

/** classe de base de multimedia
 * abstraite
 */
class Source : public Pointable
{
protected:
    string objName;
    long int date;      // seconds
    string fileName;    // path

public:
    /// Constructeur
    Source();
    Source(string _objName);
    Source(string _objName, string pathPrefix);
    Source(string _objName, long _date, string pathPrefix );

    /// destructeur
    virtual ~Source();

    /// retourne le nom de l'objet
    virtual string getObjName() const;
    /// retourne la date d'acquisition de l'objet
    virtual long getDate() const;
    /// retourne le chemin complet du fichier
    virtual string getFileName() const;

    /// modifie le nom de l'objet
    virtual void setObjName(string _objName);
    /// modifie la date d'acquisition de l'objet
    virtual void setDate(long date);
    /// modifie le chemin complet du fichier
    virtual void setFileName(string _fileName);

    /// affichage
    virtual void print() const;
    /// ecrit la sortie dans un stringstream
    virtual std::stringstream& printToStream(stringstream& stream) const;

    /// play
    virtual void play() const = 0;
};

#endif // SOURCE_H
