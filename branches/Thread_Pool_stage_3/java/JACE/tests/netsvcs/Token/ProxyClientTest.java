package JACE.tests.netsvcs.Token;

import java.net.*;
import java.io.*;
import JACE.ASX.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Concurrency.*;
import JACE.netsvcs.Token.*;
 
public class ProxyClientTest
{ 
  public static void report(String s) {
    System.out.println(s);
  }

  static InputStreamReader stdin = new InputStreamReader (System.in);
  static RemoteLock lock;
  static int requeuePosition = -1, timeout = -1;

  public static void showMenu ()
  {
    report ("\n[A]cquire");
    report ("Acquire [w]rite");
    report ("Acquire rea[d]");
    report ("[T]ry Acquire");
    report ("Try acquire wr[i]te");
    report ("Try acquire r[e]ad");
    report ("Re[n]ew");
    report ("[R]elease");

    report ("\n[Q]uit");
    System.out.print ("\nOption (AWDTIENRQ): ");
  }
  
  public static void processCommands () throws IOException
  {
    int ch = 0, result = -1;

    showMenu ();
    do {

      ch = stdin.read();
      
      try {

	switch (ch) 
	  {
	  case 'a':
	  case 'A':
	    if (timeout >= 0) {
	      TimeValue tv = TimeValue.relativeTimeOfDay (timeout, 0);
	      result = lock.acquire (tv);
	    } else
	      result = lock.acquire ();
	    break;
	  case 'w':
	  case 'W':
	    if (timeout >= 0) {
	      TimeValue tv = TimeValue.relativeTimeOfDay (timeout, 0);
	      result = lock.acquireWrite (tv);
	    } else
	      result = lock.acquire ();
	    break;
	  case 'd':
	  case 'D':
	    if (timeout >= 0) {
	      TimeValue tv = TimeValue.relativeTimeOfDay (timeout, 0);
	      result = lock.acquireRead (tv);
	    } else
	      result = lock.acquire ();
	    break;
	  case 't':
	  case 'T':
	    result = lock.tryAcquire ();
	    break;
	  case 'i':
	  case 'I':
	    result = lock.tryAcquireWrite ();
	    break;
	  case 'e':
	  case 'E':
	    result = lock.tryAcquireRead ();
	    break;
	  case 'r':
	  case 'R':
	    result = lock.release ();
	    break;
	  case 'n':
	  case 'N':
	    if (timeout >= 0) {
	      TimeValue tv = TimeValue.relativeTimeOfDay (timeout, 0);
	      result = lock.renew (requeuePosition, tv);
	    } else
	      result = lock.renew (requeuePosition);
	    break;
	  case 'q':
	  case 'Q':
	    continue;
	  case -1:
	  case 10:
	  case 13:
	    // Ignore line feeds and carriage returns
	    continue;
	  default:
	    report ("Unknown command: " + (char)ch + " (" + ch + ")\n");
	    showMenu ();
	    continue;
	  }

	report ("\nResult: " + (result == lock.SUCCESS ? "Success" : 
				"Failure"));

      } catch (TimeoutException e) {
	report ("\nRequest timed out");
	/*
      } catch (InterruptedException e) {
	report ("\nInterrupted while attemping operation");
	*/
      } catch (LockException e) {
	report ("\nLock failure: " + e.getMessage ());
      }

      showMenu ();

    } while (ch != 'Q' && ch != 'q');
  }

  public static void printUsage ()
  {
    report ("Valid options:\n");
    report ("-h <host name>      Specify token server host");
    report ("-p <port number>    Port to connect to");
    report ("-t <time in sec>    Relative timeout to use");
    report ("-l <lock type>      0 Mutex, 1 RWMutex");
    report ("-c <client ID>      Specify ID (default is this machine name)");
    report ("-n <token name>     Specify token name");
    report ("-q <requeue pos>    Use this requeue position in renew");
    report ("-d                  Enable debugging");
  }

  public static void main (String args[]) throws IOException 
  { 
    GetOpt opt = new GetOpt (args, "h:p:t:l:c:n:q:d", true);

    String host = "localhost", tokenName = "token1";
    int port = ACE.DEFAULT_SERVER_PORT;
    int type = LockTypes.MUTEX, c;
    String clientID = InetAddress.getLocalHost().getHostName ();

    try {
      while ((c = opt.next ()) != -1) {
	switch (c) 
	  {
	  case 'h':
	    host = opt.optarg ();
	    break;
	  case 'p':
	    port = Integer.parseInt (opt.optarg ());
	    break;
	  case 't':
	    timeout = Integer.parseInt (opt.optarg ());
	    break;
	  case 'l':
	    type = Integer.parseInt (opt.optarg ());
	    break;
	  case 'c':
	    clientID = opt.optarg ();
	    break;
	  case 'n':
	    tokenName = opt.optarg ();
	    break;
	  case 'q':
	    requeuePosition = Integer.parseInt (opt.optarg ());
	    break;
	  case 'd':
	    ACE.enableDebugging ();
	    ACE.DEBUG ("Debugging is enabled");
	    break;
	  default:
	    printUsage ();
	    return;
	  }
      }
    } catch (ArrayIndexOutOfBoundsException e) {
      printUsage ();
      return;
    }

    report ("Using    : " + host + " on port " + port);
    report ("Token    : " + tokenName);
    report ("Client ID: " + clientID);

    lock = new RemoteLock (type, 0, tokenName, clientID, host, port);

    processCommands ();

    lock.close ();
  }
}
