#include <iostream>
#include <dirent.h>
#include "source.h"
#include "video.h"
#include "photo.h"
#include "film.h"
#include "groupe.h"
#include "intrusive_ptr.h"
#include "factory.h"
#include "TCPServer.h"
#include "Socket.h"

#define MAX_PATH 100

using namespace std;

int main()
{
    // get working directory path
    char currentDir[MAX_PATH];
    getcwd(currentDir, MAX_PATH);

    /// file name should be appended to pathPrefix "./multimedia/"
    static string pathPrefix = currentDir;
    pathPrefix = pathPrefix.append("/multimedia/");
    cout<<pathPrefix<<endl;



    //------- ETAPE 4 -----------

/*    Video* video = new Video("myVideo.mp4", pathPrefix + "myVideo.mp4");
    Photo* photo = new Photo("logoTPT.jpg", pathPrefix + "logoTPT.jpg");

     video->print();
     photo->print();

     photo->play();
     video->play();

     delete video;
     delete photo;
*/

    //--------ETAPE 5----Test tableaux----------
/*  Source** tab = new Source*[10];
    int count = 0;

    // 8 elements
    tab[count++] = new Video("v1");
    tab[count++] = new Photo("p1");
    tab[count++] = new Video("v2");
    tab[count++] = new Photo("p2");
    tab[count++] = new Video("v3");
    tab[count++] = new Photo("p3");

   for (int i = 0; i < count; i++) {
        cout << i;
        tab[i]->print();
    }

    delete[] tab; // !!
    tab = NULL;
*/

    //--------- ETAPE 6 ---------------------
/*      int* tabDur = new int[9];
      for (int i = 0; i < 9; i++) {
          tabDur[i] = i*2;
      }

      for (int i = 0; i < 9; i++) {
          cout << tabDur[i] << " ";
      }
      cout << endl;

      Film* myFilm = new Film("myFilm", "filePath", 10);
      myFilm->setTabDur(tabDur, 9);
      myFilm->print();

      cout << "destruction du tableau en argument..." << endl;
      delete[] tabDur;
      tabDur = NULL;

      myFilm->print();


    //--------- ETAPE 7 -------Test copie des objets---------
      Film myFilm2(*myFilm); // copie d'objet
      cout << "------film 2-------" << endl;
      myFilm2.print();

      Film* myFilm3 = new Film("film3", "path3", 30);
      *myFilm3 = *myFilm;
      cout << "------film 3-------" << endl;
      myFilm3->print();

      cout << "------ destruction de myFilm1 ------" << endl;
      delete myFilm;
      myFilm =  NULL;
      myFilm2.print();
      myFilm3->print();

      delete myFilm2;
      myFilm2 = NULL;
      delete myFilm3;
      myFilm3 = NULL;
*/

    //-------- ETAPE 9-10 ------Test Groupe----------

 /*   intrusive_ptr<Video> mVideo1 = new Video("video1","videoPath");
    intrusive_ptr<Video> mVideo2 = new Video("video2","videoPath");
    intrusive_ptr<Video> mVideo3 = new Video("video3","videoPath");
    intrusive_ptr<Photo> mPhoto1 = new Photo("photo1");
    intrusive_ptr<Photo> mPhoto2 = new Photo("photo2");
    intrusive_ptr<Photo> mPhoto3 = new Photo("photo3");
    intrusive_ptr<Film> mFilm1 = new Film("myFilm1", "filePath", 10);
    intrusive_ptr<Film> mFilm2 = new Film("myFilm2", "filePath", 20);
    intrusive_ptr<Film> mFilm3 = new Film("myFilm3", "filePath", 30);

    intrusive_ptr<Groupe> grp1 = new Groupe("groupe1");
    grp1->push_back(mVideo1);
    grp1->push_back(mVideo2);
    grp1->push_back(mPhoto1);
    grp1->push_back(mFilm1);

    intrusive_ptr<Groupe> grp2 = new Groupe("groupe2");
    grp2->push_back(mVideo2);
    grp2->push_back(mPhoto2);
    grp2->push_back(mPhoto3);
    grp2->push_back(mFilm2);

    intrusive_ptr<Groupe> grp3 = new Groupe("groupe3");
    grp3->push_back(mVideo3);
    grp3->push_back(mPhoto3);
    grp3->push_back(mFilm3);
    grp3->push_back(mFilm1);

    grp1->remove(mVideo2);
    grp2->remove(mVideo2);
    mVideo2 = NULL;


    grp3 = NULL;

    cout<<"END" << endl;
*/

    //------ ETAPE 12 ------ Test Factory -----------
/*  Factory* fact = new Factory();

    intrusive_ptr<Photo> p1 = fact->createPhoto("photo1.jpg", pathPrefix);
    intrusive_ptr<Film> f1 = fact->createFilm("film1.mp4", pathPrefix);

    intrusive_ptr<Video> v1 = fact->createVideo("video1.mp4", pathPrefix);
    intrusive_ptr<Film> f2 = fact->createFilm("film2.mp4", pathPrefix);
    intrusive_ptr<Photo> p2 = fact->createPhoto("photo2.png", pathPrefix);

    intrusive_ptr<Groupe> grp1 = fact->createGroup("groupe1");
    intrusive_ptr<Groupe> grp2 = fact->createGroup("groupe2");

    grp1->push_back(p1);
    grp1->push_back(f1);
    grp1->push_back(v1);

    grp2->push_back(f1);    // 2nd time

    grp2->push_back(v1);    // 2nd time
    grp2->push_back(f2);
    grp2->push_back(p2);

    // test
    fact->print();
    fact->find("video1.mp4");
    fact->find("groupe1");
    fact->find("whatever");

    cout<< "-------Film1-------------"<<endl;
    fact->remove("film1");
    cout<< "-------Photo1-------------"<<endl;
    fact->remove("photo1");
    cout<< "-------Groupe1-------------"<<endl;
    fact->remove("groupe1");
*/


    //------ ETAPE 13 ------ Client / Serveur -----------
    TCPServer* server = new TCPServer(1000);
    server->run(3331);

    return 0;
}


