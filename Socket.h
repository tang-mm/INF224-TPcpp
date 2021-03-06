//
//  Socket.h: TCP/IP or datagram INET sockets.
//  Eric Lecolinet - Telecom ParisTech - 2013.
//

#ifndef __Socket__
#define __Socket__
#include <string>
#include <sys/socket.h>

// socklen_t required on Linux but doesn't exist on BSD
#ifdef HAVE_NO_SOCKLEN_T
#  define socklen_t int
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/** TCP/IP or Datagram socket.
 * This class supports TCP/IP or Datagram AF_INET connections (IPv4 Internet protocol).
 * Note: other families, such as AF_INET6 or AF_UNIX not supported.
 */
class Socket {
public:
  /** Socket errors.
   * - Socket::FAILED (-1): could not connect
   * - Socket::UNKNOWN_HOST (-2): could not reach host
   * - Socket::INVALID_SOCKET (-3): wrong socket type.
   */
  enum Errors {FAILED = -1, UNKNOWN_HOST = -2, INVALID_SOCKET = -3};

  /** Creates a new socket.
   * Creates a AF_INET socket using the IPv4 Internet protocol. 
   * Type can be:
   * - SOCK_STREAM (the default) for TCP/IP connected stream sockets
   * - SOCK_DGRAM for UDP datagram sockets
   */
  Socket(int type = SOCK_STREAM);
  
  /** Destructor.
   * Closes the socket.
   */
  virtual ~Socket();
  
  /** Assigns this address to the socket.
   * A SOCK_STREAM ServerSocket must typically call this function once created.
   * @return 0 on success or a negative value on error which is one of Socket::Errors
   */
  virtual int bind(const std::string& host, int port);
  
  /** Connects this socket to the server.
   * A SOCK_STREAM Socket must typically call this function once created.
   * @return 0 on success or a negative value on error which is one of Socket::Errors
   */
  virtual int connect(const std::string& host, int port);
  
  /** Closes the socket.
   * @return 0 on success and -1 on error.
   */
  virtual int close();
  
  /// Returns the file descriptor of the socket.
  int getFileDescriptor() const {return sockfd;}
    
  /** Sends data to a connected socket.
   * Sends _len_ bytes to a connected (i.e. SOCK_STREAM) socket.
   * @note that that connected sockets do not preserve record boundaries (@see SocketBuffer).
   * @return the number of bytes sent or -1 in case of an error.
   * @see the send() system call for more details.
   */
  ssize_t send(const void* buf, size_t len, int flags = 0) {
    return ::send(sockfd, buf, len, flags);
  }
  
  /** Receives data from a connected socket.
   * Reads at most _len_ bytes from a connected (i.e. SOCK_STREAM) socket.   
   * By default, this function blocks the caller until data is present (@see recv()).
   * @note that that connected sockets do not preserve record boundaries (@see SocketBuffer).
   * @return the number of bytes received, -1 in case of an error, 0 at end-of-stream 
   * (e.g. if shutdownOutput() was called on the other side).
   * @see the recv() system call for more details.
   */
  ssize_t receive(void* buf, size_t len, int flags = 0) {
    return ::recv(sockfd, buf, len, flags);
  }
  
  /** Sends data to a datagram socket.
   * Sends _len_ bytes to a datagram (i.e. SOCK_DGRAM) socket.
   * @return the number of bytes sent or -1 in case of an error.
   * @see the sendto() system call for more details.
   */
  ssize_t sendTo(const void* buf, size_t len, int flags,
                 const struct sockaddr* dest_addr, socklen_t addrlen) {
    return ::sendto(sockfd, buf, len, flags, dest_addr, addrlen);
  }

  /** Receives data from datagram socket.
   * Reads at most _len_ bytes from a datagram (i.e. SOCK_DGRAM) socket.
   * By default, this function blocks the caller until data is present (@see recv()).
   * @return the number of bytes received, -1 in case of an error, 0 at end-of-stream
   * (e.g. if shutdownOutput() was called on the other side).
   * @see the recvfrom() system call for more details.
   */
  ssize_t receiveFrom(void* buf, size_t len, int flags,
                      struct sockaddr* src_addr, socklen_t* addrlen) {
    return ::recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
  }

  /// Disables further receive operations.
  void shutdownInput();
  
  /// Disables further send operations.
  void shutdownOutput();
  
  /** Gets socket options.
   * @see the getsockopt() system call.
   * @return 0 on success and -1 on error.
   */
  int getOption(int level, int optname, void* optval, socklen_t* optlen);
  
  /** Sets socket options.
   * @see the setsockopt() system call.
   * @seealso helper functions setReuseAddress(), setTcpNoDelay(), etc.
   * @return 0 on success and -1 on error.
   */
  int setOption(int level, int optname, const void* optval, socklen_t optlen);
  
  /// sets the SO_RCVBUF option to the specified value.
  int setReceiveBufferSize(int size);
    
  /// enables/disables the SO_REUSEADDR socket option.
  int setReuseAddress(bool);
  
  /// sets the SO_SNDBUF option to the specified value.
  int setSendBufferSize(int size);

  /// enables/disables SO_LINGER with the specified linger time in seconds.
  int setSoLinger(bool, int linger);
  
  /// enables/disables SO_TIMEOUT with the specified timeout (in milliseconds).
  int setSoTimeout(int timeout);
  
  /// turns on/off TCP coalescence (useful in some cases to avoid delays).
  int setTcpNoDelay(bool);

protected:
  /// initializes a INET4 address.
  virtual struct sockaddr* initIN4Address(struct sockaddr_in& addr,
                                          const std::string& host, int port);
private:
  friend class ServerSocket;
  int sockfd;

  Socket(const Socket&);  // no copy constr
  Socket& operator=(const Socket&);  // no copy oper
  Socket(class ServerSocket*, int sockfd);  // private constructor for ServerSocket.
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/** Class for exchanging data blocks or text lines between SOCK_STREAM sockets.
 * SOCK_STREAM (i.e. TCP/IP) sockets do not preserve record boundaries.
 * Messages can thus be split or merged so that one call to Socket::send() on the
 * sending side does not necessarily correspond to one call to Socket::receive() 
 * on the receiving side.
 * This class makes it easier to solve this problem by providing functions that
 * call send() or receive() as many times as needed.
 */
class SocketBuffer {
public:
  /** constructor.
   * Argument must be a valid SOCK_STREAM socket.
   */
  SocketBuffer(Socket*);
  
  virtual ~SocketBuffer();
  
  /** sends a fixed number of bytes to a connected socket.
   * Calls Socket::send() as many times as needed to send 'len' bytes.
   * @return the number of bytes sent or -1 in case of an error
   * @see Socket::send() for more details.
   */
  virtual ssize_t write(const void* buf, size_t len);
  
  /** Receives a fixed number of bytes from a connected socket.
   * Calls Socket::receive() as many times as needed to get 'len' bytes.
   * @return the number of bytes received, -1 in case of an error, 0 at end-of-stream
   * (e.g. if shutdownOutput() was called on the other side).
   * @see Socket::receive() for more details.
   */
  virtual ssize_t read(void* buf, size_t len);
  
  /** sends a line of text to a connected socket.
   * Same effect as write() except that a newline (character '\n' ) is added to
   * the end of the string.
   * @return the number of bytes sent (including the newline) or -1 in case of an error.
   * @see write() for more details.
   */
  virtual ssize_t writeLine(const std::string&);
  
  /** reads a line of text from a connected socket.
   * Reads characters from the socket and stores them into str until a newline 
   * (character '\n' or '\r') have been read or the end-of-stream is reached.
   * @return the number of bytes received including the newline), -1 in case of an error,
   * 0 at end-of-stream (e.g. if shutdownOutput() was called on the other side).   
   * @see read() for more detail.
   */
  virtual ssize_t readLine(std::string&);
  
  /** sends a line of text to a connected socket.
   * Same effect as write() except that a newline (character '\n' ) is added to
   * the end of the string.
   * @return the number of bytes sent (including the newline) or -1 in case of an error.
   * @see write() for more details.
   */
  virtual ssize_t writeLine(const char* str, size_t len);

  /** reads a line of text from a connected socket.
   * Reads characters from the socket and stores them into str until (num-1) 
   * characters have been read or either a newline (character '\n' or '\r')
   * or the end-of-stream is reached. A terminating null character is always appended.
   * @return the number of bytes received including the newline), -1 in case of an error,
   * 0 at end-of-stream (e.g. if shutdownOutput() was called on the other side).
   * @see read() for more details.
   */
  virtual ssize_t readLine(char* str, size_t len, bool& truncated);
  
private:
  Socket* sock;
  char* remaining_buffer;
  size_t remaining_buffer_size;
  void saveRemainings(const char* buf1, size_t size1, const char* buf2, size_t size2);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 /** TCP/IP or Datagram INET socket server.
 */
class ServerSocket {  
public:
  /** creates a new ServerSocket.
   * Creates a listening AF_INET socket (using the IPv4 Internet protocol) that 
   * waits for connection requests by client sockets. 'type' can be:
   * - SOCK_STREAM (the default) for TCP/IP connected sockets
   * - SOCK_DGRAM for UDP datagram sockets
   */
  ServerSocket(int type = SOCK_STREAM);
  
  virtual ~ServerSocket();
  
  /** accepts a new connection request and returns the corresponding socket.
   * By default, this function blocks the caller until a connection is present
   * @return Returns the new Socket or NULL on error
   */
  virtual Socket* accept();

  /** assigns this port to the socket on localhost.
   * @return returns 0 on success or a negative value on error which is one of:
   * - Socket::FAILED (-1): could not connect
   * - Socket::INVALID_SOCKET: wrong socket type.
   */
  virtual int bind(int port, int backlog = 50);
  
  /// closes the socket.
  virtual int close();
  
  /// returns the file descriptor of the socket.
  int getFileDescriptor() const {return sockfd;}
  
  /** handle SIGPIPE signals.
   * Sockets may abort programs by throwing a SIGPIPE signal. The function given
   * as an argument will be called instead of aborting the program.
   * @seealso ignoreSigPipe()
   */
  void handleSigPipe( void(*function)(int signal) );
  
  /// ignore SIGPIPE signals (@see handleSigPipe()).
  void ignoreSigPipe();
  
  /** gets socket options.
   * Same arguments and effect as the getsockopt() system call.
   * @return On success, zero is returned. On error, -1 is returned.
   */
  int getOption(int level, int optname, void* optval, socklen_t* optlen);
  
  /** sets socket options.
   * Same arguments and effect as the setsockopt() system call.
   * @return On success, zero is returned. On error, -1 is returned.
   * @seealso helper functions setReuseAddress(), setTcpNoDelay(), etc.
   */
  int setOption(int level, int optname, const void* optval, socklen_t optlen);
  
  /// sets the SO_RCVBUF option to the specified value.
  int setReceiveBufferSize(int size);
    
  /// enables/disables the SO_REUSEADDR socket option.
  int setReuseAddress(bool);
  
  /// enables/disables SO_TIMEOUT with the specified timeout (in milliseconds).
  int	setSoTimeout(int timeout);
  
  /// turns on/off TCP coalescence (useful in some cases to avoid delays).
  int setTcpNoDelay(bool);
    
private:
  int sockfd;  // listening socket
  ServerSocket(const ServerSocket&);  // no copy constr
  ServerSocket& operator=(const ServerSocket&);  // no copy oper.
};


#endif
