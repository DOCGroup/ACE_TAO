// ============================================================================
//
// = PACKAGE
//    tests.Connection
// 
// = FILENAME
//    AcceptorTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.Connection;

import java.io.*;
import java.net.*;
import ACE.OS.*;
import ACE.Connection.*;

public class AcceptorTest
{
  void print_usage_and_die ()
    {
      System.out.println ("Usage: test_server [<port>]");
      System.exit (0);
    }

  public void init (int port)
    {
      try
	{
	  Acceptor acceptor = new Acceptor (Class.forName ("tests.Connection.ServerHandler"));
	  acceptor.open (port);
	  while (true)
	    {
	      acceptor.accept ();
	    }
	}
      catch (ClassNotFoundException e)
	{
	  ACE.ERROR (e);
	}
      catch (SocketException e)
	{
	  ACE.ERROR ("Socket Exception: " + e);
	}
      catch (InstantiationException e)
	{
	  ACE.ERROR (e);
	}
      catch (IllegalAccessException e)
	{
	  ACE.ERROR ("Dang!" + e);
	}
      catch (IOException e)
	{
	  ACE.ERROR (e);
	}
    }

  public static void main (String [] args)
    {
      int port = ACE.DEFAULT_SERVER_PORT;
      AcceptorTest acceptorTest = new AcceptorTest ();

      if (args.length == 1)
	{
	  try
	    {
	      port = Integer.parseInt (args[0]);
	    }
	  catch (NumberFormatException e)
	    {
	      acceptorTest.print_usage_and_die ();
	    }
	}
      acceptorTest.init (port);
    }
}
