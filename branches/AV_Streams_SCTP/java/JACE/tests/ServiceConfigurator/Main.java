package JACE.tests.ServiceConfigurator;

import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.ServiceConfigurator.*;

/**
 * Runs the service configurator, allowing the user to interact with it.
 */
public class Main
{
  public static void main (String args[])
    throws FileNotFoundException, IOException, ClassNotFoundException, 
	   IllegalAccessException, InstantiationException
  {
    ACE.enableDebugging ();

    if (ServiceConfig.open (args) < 0) {
      System.err.println ("Error opening ServiceConfig");
      return;
    }

    processCommands (args);

    // close everything down
    ServiceConfig.close ();
  }

  public static void report(String s) {
    System.out.println(s);
  }

  static InputStreamReader stdin = new InputStreamReader (System.in);

  public static void showMenu ()
  {
    report ("\n[C]lose all");
    report ("[S]uspend all");
    report ("[R]esume all");
    report ("[Re[l]oad all");
    report ("List [n]ames");

    report ("\n[Q]uit");
    System.out.print ("\nOption (CSRLNQ): ");
  }

  public static void processCommands (String [] args)
    throws FileNotFoundException, IOException, ClassNotFoundException, 
	   IllegalAccessException, InstantiationException
  {
    int ch = 0;
    Enumeration svcs;

    showMenu ();

    do {

      ch = stdin.read ();

      switch (ch)
	{
	case 'c':
	case 'C':
	  ServiceConfig.close ();
	  break;
	case 's':
	case 'S':
	  svcs = ServiceConfig.serviceNames ();
	  while (svcs.hasMoreElements ()) 
	    ServiceConfig.suspend ((String)svcs.nextElement ());
	  break;
	case 'r':
	case 'R':
	  svcs = ServiceConfig.serviceNames ();
	  while (svcs.hasMoreElements ())
	    ServiceConfig.resume ((String)svcs.nextElement ());
	  break;
	case 'l':
	case 'L':
	  ServiceConfig.close ();
	  svcs = ServiceConfig.serviceNames ();
	  while (svcs.hasMoreElements ()) 
	    ServiceConfig.remove ((String)svcs.nextElement ());
	  ServiceConfig.open (args);
	  break;
	case 'n':
	case 'N':
	  report ("\nService names:\n");
	  svcs = ServiceConfig.services ();
	  while (svcs.hasMoreElements ()) {
	    Service sv = (Service)svcs.nextElement ();
	    report (sv.name () + " : " + sv.info ());
	  }
	  break;
	case 'q':
	case 'Q':
	  report ("\nExiting...");
	  return;
	case -1:
	case 10:
	case 13:
	  continue;
	default:
	  break;
	}

      showMenu ();

    } while (true);
  }

}
