package tests.netsvcs.Naming;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.SOCK_SAP.*;
import netsvcs.Naming.*;

public class ClientTest
{
  void print_usage_and_die ()
    {
      System.out.println ("Usage: ClientTest <hostname> [<port>]");
      System.exit (0);
    }

  public SOCKStream init (SOCKStream socket, String hostname, int port)
    {
      try
	{
	  SOCKConnector sc = new SOCKConnector(socket, hostname, port);
	}
      catch (UnknownHostException e)
	{
	  ACE.ERROR (e);
	  System.exit(0);
	}
      catch (SocketException e)
	{
	  ACE.ERROR ("Connection refused");
	  System.exit(0);
	}
      catch (IOException e)
	{
	  ACE.ERROR (e);
	  System.exit(0);
	}

      return socket;
    }

  public static void main (String [] args)
    {
      SOCKStream socket;
      int port = ACE.DEFAULT_SERVER_PORT;
      ClientTest clientTest = new ClientTest ();

      if (args.length == 2)
	{
	  try
	    {
	      port = Integer.parseInt (args[1]);
	    }
	  catch (NumberFormatException e)
	    {
	      clientTest.print_usage_and_die ();
	    }
	}
      System.out.println("Trying to open port " + port + " on " + args[0]);

      socket = clientTest.init (new SOCKStream(), args[0], port);

      NameProxy proxy = new NameProxy(socket);

      System.out.println("---- Beginning tests ----");

      try {

	System.out.println("Binding (five, six, seven):  " + proxy.bind("five", "six", "seven"));
	System.out.println("Binding (filth, rat, eats):  " + proxy.bind("filth", "rat", "eats"));

	Vector res = proxy.resolve("five");

	System.out.println("Resolve (five)            :  " + (String)res.elementAt(1) + " " 
			                                   + (String)res.elementAt(2));

	System.out.println("Binding (fish, words, him):  " + proxy.bind("fish", "words", "him"));

	System.out.println("Unbind  (five, six, seven):  " + proxy.unbind("five"));

	res = proxy.resolve("five");

	System.out.println("Resolve (five)            :  " + (String)res.elementAt(1) + " " 
			                                   + (String)res.elementAt(2));
	System.out.println("Binding (fiction, us, you):  " + proxy.bind("fiction", "us", "you"));

	System.out.println("\nGetting all records whose names begin with fi:\n");
	Vector res2 = proxy.listNameEntries("fi");

	Enumeration iter = res2.elements();

	while (iter.hasMoreElements()) {
	  Vector res3 = (Vector)(iter.nextElement());

	  System.out.println((String)res3.elementAt(0) + "\t" + (String)res3.elementAt(1) 
			     + "\t" + (String)res3.elementAt(2));
	}

      } catch (IOException e) 
	{
	ACE.ERROR("" + e);
	} 
      finally 
	{
	  try {
	    socket.close();
	  } catch (IOException e) {
	    ACE.ERROR("" + e);
	  }
	}

    }

};

