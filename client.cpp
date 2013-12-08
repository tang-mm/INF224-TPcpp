//
// Client C++ pour communiquer avec le Serveur C++
// eric lecolinet - telecom paristech - 2013
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Socket.h"
using namespace std;


static const char* DEFAULT_HOST = "localhost";
static const int DEFAULT_PORT = 3331;
static const int BUFFER_SIZE = 1000;


int main(int argc, char* argv[]) {
  const char* host = DEFAULT_HOST;
  int port = DEFAULT_PORT;
  
  if (argc >=2) host = argv[1];
  if (argc >=3) port = atoi(argv[2]);
  
  Socket sock;
  
  int status = sock.connect(host, port);
  if (status < 0) {
    cerr << "Client couldn't connect: "<< host<<":"<<port << endl;
    return 1;
  }
  else cerr << "Client connected to "<< host<<":"<<port << endl;

  SocketBuffer sockbuf(&sock);
  
  while (cin.good()) {
    cout << "Message: ";
    string message;
    getline(cin, message);

    ssize_t sent = sockbuf.writeLine(message);
    if (sent < 0) {
      cerr << "Client could not send message to "<<host<<":"<<port<< endl;
      return 2;
    }
    
    string response;
    ssize_t received = sockbuf.readLine(response);
    if (received < 0) {
      cerr << "Client could not receive message from "<<host<<":"<<port<< endl;
      return 3;
    }

    cout << "Response: '" << response << "'" << endl;
  }
  
  return 0;
}


