package clientServer;
//
// Client Java pour communiquer avec le Serveur C++ 
// eric lecolinet - telecom paristech - 2013
//

import java.io.*;
import java.net.*;


public class Client {
  private static final long serialVersionUID = 1L;
  static final String DEFAULT_HOST = "localhost";
  static final int DEFAULT_PORT = 3331;
  
  private Socket sock;
  private BufferedReader input;
  private BufferedWriter output;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  public static void main(String argv[]) {
    String host = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    
    if (argv.length >=2) host = argv[0];
    if (argv.length >=3) port = Integer.parseInt(argv[1]);
    
    Client client = null;
    
    try {
      client = new Client(host, port);
    }
    catch (Exception e) {
      System.out.println("Client: couldn't connect: "+host+":"+port);
      System.exit(1);
    }
    
    System.out.println("Client connected to "+host+":"+port);

    // pour lire depuis la console
    BufferedReader cin = new BufferedReader(new InputStreamReader(System.in));
    
    while (true) {
      System.out.print("Message: ");
      try {
        String message = cin.readLine();
        String response = client.send(message);
        System.out.println("Response: " + response);
      }
      catch (java.io.IOException e) {
        System.out.println("Client: IO error");
        return;
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public Client(String host, int port) throws UnknownHostException, IOException {
    try {
      sock = new java.net.Socket(host, port);
    }
    catch (java.net.UnknownHostException e) {
      System.err.println("Client: couldn't find host: "+host+":"+port);
      throw e;
    }
    catch (java.io.IOException e) {
      System.err.println("Client: couldn't reach host: "+host+":"+port);
      throw e;
    }
    
    try {
      input = new BufferedReader(new InputStreamReader(sock.getInputStream()));
      output = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
    }
    catch (java.io.IOException e) {
      System.err.println("Client: couldn't open input or output streams");
      throw e;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /** envoie une commande au server et retourne sa reponse.
   * noter que la fonction bloque si le serveur ne repond pas.
   */
  public String send(String command) {
    
    // Envoyer la commande au serveur
    try {
      command += '\n';  // ajouter le delimiteur qui separe les messages
      output.write(command, 0, command.length());
      output.flush();
    }
    catch (java.io.IOException e) {
      System.err.println("Client couldn't write: " + e);
      return null;
    }
    
    // Recuperer le resultat retourne par le serveur
    try {
      return input.readLine();
    }
    catch (java.io.IOException e) {
      System.err.println("Client couldn't read: " + e);
      return null;
    }
  }
}


