/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    ServerLoggingHandler.java
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.netsvcs.Logger;

import java.io.*;
import java.util.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import JACE.Connection.*;
import JACE.OS.*;
import JACE.netsvcs.Handler;

/**
 *
 * Created by ServerLoggingAcceptor to handle logging requests.  This
 * simply reads the record and hands it to the registered LoggingStrategy.
 *
 * @see JACE.netsvcs.Logger.ServerLoggingAcceptor
 */
public class ServerLoggingHandler extends Handler
{
  /**
   * Reads in the given LogRecord request and hands it to the
   * LoggingStrategy registered with the ServerLoggingAcceptor parent.
   *
   *@param request LogRecord instance to use
   */
  public void processRequest (Object request) 
    throws SocketException, EOFException, IOException
  {
    LogRecord rec = (LogRecord)request;

    rec.streamInFrom (this.peer ().dataInputStream ());

    ServerLoggingAcceptor parent = (ServerLoggingAcceptor)parent ();

    parent.loggingStrategy ().logRecord (this.hostName (), rec);
  }

  /**
   * Creates a new instance of LogRecord.
   */
  public Object newRequest ()
  {
    return new LogRecord ();
  }
}




