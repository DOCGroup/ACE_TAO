package JACE.tests.netsvcs.Naming;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.SOCK_SAP.*;
import JACE.netsvcs.Naming.*;

/**
 * Simple test program for NameProxy and the naming service.
 *
 * Command line: (hostname) (port)
 */
public class ClientTest
{
  public static void print_usage_and_die ()
  {
    System.out.println ("Usage: ClientTest <hostname> [<port>]");
    System.exit (0);
  }
  public static void main (String [] args)
    throws UnknownHostException, 
	   SocketException,
	   InstantiationException,
	   IllegalAccessException,
	   IOException
  {
    ACE.enableDebugging ();

    int port = ACE.DEFAULT_SERVER_PORT;

    if (args.length == 0 || args.length > 2)
      print_usage_and_die ();

    if (args.length == 2) {
      try
	{
	  port = Integer.parseInt (args[1]);
	}
      catch (NumberFormatException e)
	{
	  print_usage_and_die ();
	}
    }

    System.out.println("Trying to open port " + port + " on " + args[0]);

    NameProxy proxy = new NameProxy ();

    Connector c = new Connector ();
    c.open (args[0], port);
    c.connect (proxy);

    System.out.println("---- Beginning tests ----");

    try {

      System.out.println("Binding (five, six, seven):  " 
			 + proxy.bind("five", "six", "seven"));
      System.out.println("Binding (filth, rat, eats):  " 
			 + proxy.bind("filth", "rat", "eats"));

      Vector res = proxy.resolve("five");

      System.out.println("Resolve (five)            :  " 
			 + (String)res.elementAt(1) + " " 
			 + (String)res.elementAt(2));

      System.out.println("Binding (fish, words, him):  " 
			 + proxy.bind("fish", "words", "him"));

      System.out.println("Unbind  (five, six, seven):  " 
			 + proxy.unbind("five"));

      res = proxy.resolve("five");

      System.out.println("Resolve (five)            :  " 
			 + (String)res.elementAt(1) + " " 
			 + (String)res.elementAt(2));

      System.out.println("Binding (fiction, us, you):  " 
			 + proxy.bind("fiction", "us", "you"));

      System.out.println("\nGetting all records whose names begin with fi:\n");
      Vector res2 = proxy.listNameEntries("fi");

      Enumeration iter = res2.elements();

      while (iter.hasMoreElements()) {
	Vector res3 = (Vector)(iter.nextElement());

	System.out.println((String)res3.elementAt(0) 
			   + "\t" + (String)res3.elementAt(1) 
			   + "\t" + (String)res3.elementAt(2));
      }

    } catch (Exception e) {
      System.err.println ("" + e);
      e.printStackTrace ();
    } finally {
      proxy.close ();
    }
  }
}

