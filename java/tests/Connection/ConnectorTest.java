// ============================================================================
//
// = PACKAGE
//    tests.Connection
// 
// = FILENAME
//    ConnectorTest.java
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

public class ConnectorTest
{
  void print_usage_and_die ()
    {
      System.out.println ("Usage: test_Connector <hostname> [<port>]");
      System.exit (0);
    }

  public void init (String hostname, int port)
    {
      try
	{
	  Connector connector = new Connector ();
	  connector.open (hostname, port);
	  connector.connect (new ClientHandler ());
	}
      catch (UnknownHostException e)
	{
	  ACE.ERROR (e);
	}
      catch (SocketException e)
	{
	  ACE.ERROR ("Connection refused");
	}
      catch (InstantiationException e)
	{
	  ACE.ERROR (e);
	}
      catch (IllegalAccessException e)
	{
	  ACE.ERROR (e);
	}
      catch (IOException e)
	{
	  ACE.ERROR (e);
	}
    }

  public static void main (String [] args)
    {
      int port = ACE.DEFAULT_SERVER_PORT;
      ConnectorTest connectorTest = new ConnectorTest ();

      if (args.length == 2)
	{
	  try
	    {
	      port = Integer.parseInt (args[1]);
	    }
	  catch (NumberFormatException e)
	    {
	      connectorTest.print_usage_and_die ();
	    }
	}
      connectorTest.init (args[0], port);
    }
}
