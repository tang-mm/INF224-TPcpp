//
//  TCPServer.h: TCP/IP server for remote clients.
//  Eric Lecolinet - Telecom ParisTech - 2013.
//

#ifndef __TCPServer__
#define __TCPServer__
#include "Socket.h"
#include <vector>
#include "factory.h"


/** TCP/IP command server for remote clients.
 * Commands are executed sequentially (no threads)
 */
class TCPServer {
public:
  TCPServer(int buffer_size = 1000);
  virtual ~TCPServer();
  
  /** starts the main loop of the server on this port.
   * returns 0 for normal termination, a value < 0 for an error: 
   * FAILED = -1, UNKNOWN_HOST = -2, INVALID_SOCKET = -3
   */
  virtual int run(int port);

protected:
  /// processes a message; the connection will be closed if false is returned.
  virtual bool processMessage(const std::string& message, std::string& response);

  /// reads messages from a given socket.
  virtual void readMessages(Socket*);
  
  /// starts a thread for reading messages; the argument mus be a Hook.
  static void* startReadMessageThread(void* hook);
  
  struct Hook {
    Hook(TCPServer* _server, Socket* _sock) : server(_server), sock(_sock) {}
    TCPServer* server;
    Socket* sock;
  };

  ServerSocket servsock;
  std::vector<pthread_t> threads;

private: // disables copy.
  TCPServer(const TCPServer&);
  TCPServer& operator=(const TCPServer&);

  /// create and initialize a Multimadia Factory
  virtual Factory* initFactory() ;

};

#endif
