//package ACE.SimpleLogger;

import ACE.SOCK_SAP.*;
import java.io.IOException;

public class LoggingAcceptor extends Thread
{
  private SOCKAcceptor peerAcceptor_;

  public LoggingAcceptor(int port) throws IOException
  {
    peerAcceptor_ = new SOCKAcceptor(port);
    this.setName("LoggingAcceptor");
    System.err.println("Waiting for connection on port " +
		       port);
  }

  public void run()
  {
    try
      {
	while (true)
	  {
	    LoggingHandler handler = new LoggingHandler();
	    peerAcceptor_.accept(handler.stream());
	    handler.open();
	  }
      }
    catch (IOException e)
      {
      }
  }
};
