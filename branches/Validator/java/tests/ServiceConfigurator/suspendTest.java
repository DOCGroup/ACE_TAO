/*************************************************
 *
 * = PACKAGE
 *    tests.ServiceConfigurator
 *
 * = FILENAME
 *    mainTest.java
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
package tests.ServiceConfigurator;

import JACE.ServiceConfigurator.*;
import JACE.OS.*;
import java.io.*;

public class suspendTest
{

  public static void main (String args [])
  {
    ServiceConfig daemon = new ServiceConfig ();
    ACE.enableDebugging();

    try
      {
	String args1 = "-f suspendTest1.conf";
	String[] argv1 = OS.createStringArray(args1, " ");

	daemon.open (argv1);

	Thread.sleep (10000);


	System.err.println("Suspending");
	String args2 = "-f suspendTest2.conf";
	String[] argv2 = OS.createStringArray(args2, " ");
	
	daemon.open (argv2);

	Thread.sleep (10000);

	System.err.println("Resuming");
	String args3 = "-f suspendTest3.conf";
	String[] argv3 = OS.createStringArray(args3, " ");

	daemon.open (argv3);

	Thread.sleep(10000);
      }
    catch (InterruptedException e)
      {
	ACE.ERROR (e);
      }
    catch (FileNotFoundException e)
      {
	ACE.ERROR (e);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      } 
    catch (ClassNotFoundException e)
      {
	ACE.ERROR (e + "foo");
      }
    catch (IllegalAccessException e)
      {
	ACE.ERROR (e);	
      }
    catch (InstantiationException e)
      {
	ACE.ERROR (e);
      }

    System.err.println("End of suspendTest");
  }

}
