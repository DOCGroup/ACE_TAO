/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    LogClient.java
 *
 *@author Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;
 
import JACE.SOCK_SAP.*;
import JACE.OS.*;
import java.net.*;
import java.io.*;
import java.util.*;
import java.lang.*;

/**
 * Simple direct logging client to test the logging service.  This is similar to
 * C++ ACE's direct_logging example.  Command line parameters are the port
 * and host to log to.
 */
public class LogClient
{
  public static void main(String [] args)
  {
    
    if (args.length != 2) {
      ACE.ERROR("Command line arguments: <port> <hostname>");
      System.exit(1);
    }

    Socket sock = null;
    try {

      ACE.DEBUG("Trying: " + args[1] + " port " + args[0]);
      
      Integer port = new Integer(args[0]);

      sock = new Socket(args[1], port.intValue());

    } catch (UnknownHostException e) {
      ACE.ERROR("" + e);
      System.exit(1);
    } catch (IOException e) {
      ACE.ERROR("Couldn't get socket");
      System.exit(1);
    }

    try {

      DataOutputStream dos = new DataOutputStream(sock.getOutputStream());

      LogRecord log = new LogRecord();
      log.msgData("hello world.");

      log.streamOutTo(dos);

      sock.close();

    } catch (IOException e) {
      ACE.ERROR("Error sending message");
      System.exit(1);
    }
  }
}

    
