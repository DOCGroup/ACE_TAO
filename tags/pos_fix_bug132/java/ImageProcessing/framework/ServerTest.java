package imaging.framework;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.Connection.*;

public class ServerTest
{
  void print_usage_and_die ()
    {
      System.out.println ("Usage: ServerTest [<port>]");
      System.exit (0);
    }

  public void init (int port)
    {
      try
	{
	  Acceptor acceptor = new Acceptor (Class.forName ("imaging.framework.ServerHandler"));
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
      ServerTest server = new ServerTest ();

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
