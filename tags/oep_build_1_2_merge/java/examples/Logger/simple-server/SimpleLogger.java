//package ACE.SimpleLogger;

import java.io.IOException;

class SimpleLogger implements Runnable
{
  private LoggingAcceptor la;
  private int port;

  public static void main(String[] args)
  {
    SimpleLogger sl = new SimpleLogger();
    sl.run();
  }

  public SimpleLogger()
  {
    this(LoggerConstants.DEFAULT_SERVER_PORT);
  }

  public SimpleLogger(int port)
  {
    try
      {
	la = new LoggingAcceptor(port);
      }
    catch (IOException ioe)
      {
	System.err.println("SimpleLogger: unable to create LoggingAcceptor ("
			   + ioe.getMessage() + ")");
      }
  }

  public void run()
  {
    la.run();
    try
      {
	la.join();
      }
    catch (InterruptedException ie)
      { }
  }
};
