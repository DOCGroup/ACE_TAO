// ============================================================================
//
// = PACKAGE
//    tests.SOCK_SAP
// 
// = FILENAME
//    SOCKAcceptorTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.SOCK_SAP;

import java.io.*;
import java.net.*;
import ACE.OS.*;
import ACE.SOCK_SAP.*;

class TestHandler extends Thread
{
  public TestHandler (SOCKStream stream)
    {
      this.stream_ = stream;
      this.start ();
    }

  public void run ()
    {
      int msgLen;
      System.out.println ("Waiting for messages...");
      try
	{
	  while (true)
	    {
	      StringBuffer msg = new StringBuffer ();
	      msgLen = this.stream_.recv (msg);
	      if (msgLen == 0)
		break;
	      ACE.DEBUG ("Received: " + msg);
	      this.stream_.send ("Got it!");
	    }
	}
      catch (NullPointerException e)
	{
	  ACE.ERROR ("connection reset by peer");
	}
      catch (IOException e)
	{
	  ACE.ERROR (e);
	}
      finally
	{
	  try 
	    {
	      this.stream_.close ();
	    }
	  catch (IOException e)
	    {
	    }
	}
    }
  SOCKStream stream_;
}

public class SOCKAcceptorTest
{
  void print_usage_and_die ()
    {
      ACE.DEBUG ("Usage: SOCKAcceptorTest [<port>]");
      System.exit (0);
    }

  public void init (int port)
    {
      try
	{
	  SOCKAcceptor acceptor = new SOCKAcceptor (port);
	  while (true)
	    {
	      SOCKStream stream = new SOCKStream ();
	      acceptor.accept (stream);
	      TestHandler handler = new TestHandler (stream);
	    }
	}
      catch (IOException e)
	{
	}
    }

  public static void main (String [] args)
    {
      int port = ACE.DEFAULT_SERVER_PORT;
      SOCKAcceptorTest server = new SOCKAcceptorTest ();
      if (args.length == 1)
	{
	  try
	    {
	      port = Integer.parseInt (args[0]);
	    }
	  catch (NumberFormatException e)
	    {
	      server.print_usage_and_die ();
	    }
	}
      server.init (port);
    }
}
