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
    ACE.DEBUG ("In Test_Service::init() with arguments: " +
    	       args[0] + args[1]);

    //ACE.DEBUG ("In new Test_Service::init() with arguments: " +
    //       args[0] + args[1]);
    return 0;
  }

}

