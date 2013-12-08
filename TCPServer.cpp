//
//  TCPServer.cpp: TCP/IP command server for remote clients.
//  Eric Lecolinet - Telecom ParisTech - 2013.
//

#include "TCPServer.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <pthread.h>
#include <dirent.h>
using namespace std;

#define MAX_PATH 100

TCPServer::TCPServer(int buffer_size) :
servsock(SOCK_STREAM)  // mode connecté (TCP/IP)
{ }


TCPServer::~TCPServer() { }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int TCPServer::run(int port) {
  int stat = servsock.bind(port);
  if (stat < 0) {
    cerr << "Server: cannot bind, status=" << stat << endl;
    return stat;
  }

  // ignorer le signal SIGPIPE (qui avorterait le serveur en cas d'erreur d'écriture)
  servsock.ignoreSigPipe();
  
  while (true) {
    Socket* sock = servsock.accept();

    if (sock == NULL) {
      cerr << "Server: accept() failed" << endl;
    }
    else {
      // lancer la lecture des messages dans un thread
      Hook hook(this, sock);
      pthread_t tid;
      pthread_create(&tid, NULL, startReadMessageThread, &hook);
      threads.push_back(tid);
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// the prototype is imposed by 
// NOTE: pthread_create() imposes a must be a static method with prototype: void* func(void*);

void* TCPServer::startReadMessageThread(void* _hook) {
  TCPServer::Hook* hook = static_cast<Hook*>(_hook);
  hook->server->readMessages(hook->sock);
  return NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TCPServer::readMessages(Socket* sock) {
  SocketBuffer sockbuf(sock);
  
  while (true) {
    string message, response;
    
    // lire la ligne de données envoyée par le client 
    ssize_t received = sockbuf.readLine(message);
    //cout << "TCPServer: received: " << received << endl;

    if (received < 0) {
      cerr << "TCPServer: read error on socket: " << sock << endl;
      goto CLOSE;
    }
    else if (received == 0) {
      cout << "TCPServer: connection closed by client on socket: " << sock << endl;
      goto CLOSE;
    }
    
    // traiter le message. Fermer la connection si la valeur de retour == false.
    if (! processMessage(message, response)) {
      cout << "TCPServer: closing connection with client on socket" << sock << endl;
      goto CLOSE;
    }
    
    // toujours envoyer une reponse (sinon le client va se bloquer)
    ssize_t sent = sockbuf.writeLine(response);
    //cout << "TCPServer: sent: " << sent << endl;

    if (sent < 0) {         // erreur d'ecriture
      cerr << "TCPServer: write error on socket: " << sock << endl;
      goto CLOSE;
    }
    else if (sent == 0) {
      cout << "TCPServer: connection closed by client on socket: " << sock << endl;
      goto CLOSE;
    }
  }
  
  // terminaison normale : ne pas effectuer ce qui suit
  return;
  
CLOSE:
  delete sock;           // detruire le socket (NB: le destr fait sock.close())
  pthread_exit(NULL);    // finir le thread
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/**
  * process message, react to telecommand from Client
  */
bool TCPServer::processMessage(const string& message, string& response) {
  //cout << "TCPServer: message: '" << message << "'" << endl;
  
  response = "Recieve: ";
  response += message;
  //cout << "TCPServer: response: '" << response << "'" << endl;

  // get command by splitting message
  char* msg = new char[256];
  strcpy(msg, message.c_str());
  char* cmd = strsep(&msg, " ");  // split the message by space


  if (strcmp(cmd, "list") == 0) {
      /// lister tous les objets au cote serveur
      Factory* fact = initFactory();
      cout<<"factory::print()"<<endl;
      fact->print();

  }else if (strcmp(cmd, "find") == 0) {
      /// ou recherche un objet et l'afficher au cote client
      if (msg == NULL) {
          cout << cmd << " >> Invalid parameter!" << endl;
          response += " >> Invalid parameter!";
          return true;
      }

      stringstream stream;

      // initiate a multimedia Factory
      Factory* fact = initFactory();
      fact->findAndSendToStream(msg, stream);

      string str;
      while (stream >> str) response += " | "+str;

  } else if (strcmp(cmd, "play") == 0) {
      /// ou trouver et jouer un objet au cote serveur
      if (msg == NULL) {
          cout << cmd << " >> Invalid parameter!" << endl;
          response += " >> Invalid parameter!";
          return true;
      }

      // initiate a multimedia Factory
      Factory* fact = initFactory();
      fact->play(msg);

  } else {
      response += " >> Command not found! Usage: \"find [objectName]\" or \"play [objectName]\" or \"list\"";
  }


  //the connection will be closed if false is returned.
  return true;
}

Factory* TCPServer::initFactory() {

    // get working directory path
    char currentDir[MAX_PATH];
    getcwd(currentDir, MAX_PATH);

    /// file name should be appended to pathPrefix "./multimedia/"
    string pathPrefix = currentDir;
    pathPrefix = pathPrefix.append("/multimedia/");
    cout<<"PATH: " << pathPrefix <<endl;

    Factory* fact = new Factory();

   intrusive_ptr<Photo> p1 = fact->createPhoto("photo1.jpg", pathPrefix);
   intrusive_ptr<Film> f1 = fact->createFilm("film1.rmvb", pathPrefix);

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

   return fact;
}

