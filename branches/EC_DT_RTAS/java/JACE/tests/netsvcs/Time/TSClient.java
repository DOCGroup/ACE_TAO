package JACE.tests.netsvcs.Time;

import JACE.netsvcs.Time.*;
import JACE.OS.*;
import JACE.Reactor.*;

public class TSClient
{

  // Will run until killed since the default is to run the timer
  // queue in its own thread. 
  public static void main (String args[])
  {
    ACE.enableDebugging ();

    TSClerkProcessor cp = new TSClerkProcessor ();

    if (cp.init (args) != 0)
      return;

    ACE.DEBUG ("***************************");
    ACE.DEBUG ("Press enter to end the test");
    ACE.DEBUG ("***************************");

    try {
      System.in.read();

      cp.close ();

    } catch (Exception e) {
      ACE.ERROR (e);
    }
  }
}
 


