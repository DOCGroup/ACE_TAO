/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceObject.java
 *
 *@author Prashant Jain
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.ASX.*;
import JACE.Reactor.*;

/**
 * Provides a default implementation of the Service interface, and can also
 * be registered with the Reactor.
 *
 *@see JACE.Reactor
 *@see Service
 */
public class ServiceObject implements EventHandler, Service
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
   *@param tv Time Value for when timer expired
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
   * method to do anything useful.  Currently, this sets an internal
   * state variable to true.
   */
  public int suspend () 
  {
    this.suspended_ = true;

    return 0;
  }

  /**
   * Request that this service resume activity.  Currently, this sets
   * an internal state variable to false.
   */
  public int resume ()
  {
    this.suspended_ = false;

    return 0;
  }

  /**
   * Is this service suspended? 
   */
  public boolean suspended ()
  {
    return this.suspended_;
  }

  /**
   * Return the name of the Service.  Implementation provided.
   */
  public String name ()
  {
    return this.name_;
  }

  /**
   * Set the name of the Service.  Should be called when a Service is
   * created -- this is done automatically by ServiceConfig when loading
   * from a file.  Implementation provided.
   */
  public void name (String name)
  {
    this.name_ = name;
  }

  /**
   * Name of this ServiceObject.
   */
  protected String name_ = null;

  /**
   * Status of whether this ServiceObject is suspended or not.
   * (Initially false)
   */
  protected boolean suspended_ = false;
}
