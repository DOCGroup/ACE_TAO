/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceObject.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.ASX.*;
import JACE.Reactor.*;

public class ServiceObject implements EventHandler
{
  /**
   * Initialize object when dynamic loading occurs. Overwrite this
   * method to do anything useful. 
   *@return -1 (default implementation)
   */
  public int init (String [] args)
  {
    return -1;
  }

  /** 
   * Terminate the object. Note that an object can not be explicitly
   * unloaded. Overwrite this method to do anything useful.
   *@return -1 (default implementation)
   */
  public int fini ()
  {
    return -1;
  }

  /**
   * Get information on an active object. Overwrite this method to do
   * anything useful. 
   *@return null (default implementation)
   */
  public String info ()
  {
    return null;
  }

  /** 
   * Called when timer expires. Overwrite this method to do
   * anything useful. 
   *@param tv Time Value for which timer was set
   *@param obj An arbitrary object that was passed to the Timer Queue
   * (Asynchronous Completion Token)
   *@return -1
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    return -1;
  }

  /**
   * Request that this service suspend activity.  Overwrite this
   * method to do anything useful.
   */
  public int suspend () 
  {
    return -1;
  }

  /**
   * Request that this service resume activity.  Overwrite this
   * method to do anything useful.
   */
  public int resume ()
  {
    return -1;
  }
}
