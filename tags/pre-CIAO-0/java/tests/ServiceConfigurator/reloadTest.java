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

public class reloadTest
{

  public static void main (String args [])
  {
    ServiceConfig daemon = new ServiceConfig ();
    try
      {
	daemon.open (args);
	Thread.sleep (10000);

	daemon.remove("TimeService");

	daemon.open (args);
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

    System.err.println("End of reloadTest");
  }

}
