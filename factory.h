#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include "photo.h"
#include "video.h"
#include "film.h"
#include "groupe.h"


class Factory
{
private:
    map<string, intrusive_ptr<Source> > tabElem;
    map<string, intrusive_ptr<Groupe> > tabGrp;

public:
    Factory();

    virtual ~Factory();

    /**
     * @brief create an instance Photo and add into tabElem
     * @param objName
     * @param pathPrefix
     * @return the created object
     */
    virtual intrusive_ptr<Photo> createPhoto(const string& objName, const string& pathPrefix);

    /**
     * @brief  create an instance Video and add into tabElem
     * @param objName
     * @return renvoie
     * @param pathPrefixnt l'objet créé afin de pouvoir le manipuler ultérieurement.
     */
    virtual intrusive_ptr<Video> createVideo(const string& objName, const string& pathPrefix);

    /**
     * @brief create an instance Film and add into tabElem
     * @param objName
     * @param pathPrefix
     * @return renvoient l'objet créé afin de pouvoir le manipuler ultérieurement.
     */
    virtual intrusive_ptr<Film> createFilm(const string& objName, const string& pathPrefix);

    /**
     * @brief create an instance Groupe and add into tabGrp
     * @param grpName
     * @return renvoient l'objet créé afin de pouvoir le manipuler ultérieurement.
     */
    virtual intrusive_ptr<Groupe> createGroup(const string& grpName);

    /**
     * Supprimer un objet multimédia ou un groupe à partir de son nom, donné en argument.
     * L'élément doit être enlevé de la table adéquate et détruit s'il n'appartient plus à aucune table.
     * De plus, lorsqu'on supprime un objet multimédia il faut d'abord l'enlever de tous les groupes dans lesquels il figure.
     * @brief remove object from the map
     * @param name
     */
    virtual void remove(const string& name);

    /**
     * Rechercher un objet multimédia ou un groupe à partir de son nom, donné en argument.
     *Cette fonction doit permettre d'afficher les attributs d'un objet ans le Terminal.
     *Dans le cas d'un groupe, afficher son nom puis les attributs des objets qu'il contient.
     * @brief search for the object with the given name, print the attibutes on screen
     * @param name
     */
    virtual void find(const string& name) const;

    /**
      * rechercher un objet et ecrire sa description dans un stringstream
      * @param name
      * @param stream stringstream dans lequel le resultat va etre ecrit
      */
    virtual stringstream& findAndSendToStream(const string &name, stringstream& stream) const;

    /**
     * Jouer un objet multimédia (à partir de son nom, donné en argument).
     * Même chose que précédemment sauf que l'on appelle la méthode play() au lieu d'afficher les attributs.
     * @brief find the object with the given name and play
     * @param name
     */
    virtual void play(const string& name) const;

    /**
      * show all available objects
      */
    virtual void print() const;

    virtual const map<string, intrusive_ptr<Source> >&  getTabElem() const;

    virtual const map<string, intrusive_ptr<Groupe> >& getTabGrp() const;


};

#endif // FACTORY_H
