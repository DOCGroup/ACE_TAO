/*************************************************
 *
 * = PACKAGE
 *    tests.ServiceConfigurator
 *
 * = FILENAME
 *    TestService.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package tests.ServiceConfigurator;

import JACE.ServiceConfigurator.*;
import JACE.OS.*;

public class TestService extends ServiceObject
{
  public int init (String [] args)
  {
    System.out.println("TestService init with arguments: ");

    if (args != null)
      for (int i = 0; i < args.length; i++)
        ACE.DEBUG (args[i]);

    return 0;
  }

  public int suspend()
  {
    System.out.println("TestService suspend ----");

    return 0;
  }

  public int resume()
  {
    System.out.println("TestService resume ----");

    return 0;
  }

  public int fini()
  {
    System.out.println("TestService remove ----");

    return 0;
  }
    
}

