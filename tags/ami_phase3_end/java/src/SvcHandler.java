/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    SvcHandler.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import JACE.ASX.*;
import JACE.Reactor.*;

public abstract class SvcHandler extends Task
{

  /**
   * Do nothing constructor. 
   */
  public SvcHandler ()
  {
  }
  
  /**
   * Set the stream using the SOCKStream passed in. This sets the
   * underlying peer
   *@param s SOCK Stream to use for the connection
   */
  public void setHandle (SOCKStream s) throws IOException
  {
    this.stream_ = s;
  }

  /**
   * Get the underlying peer
   *@return the underlying peer
   */
  public SOCKStream peer ()
  {
    return this.stream_;
  }

  /**
   * Abstract method that subclasses must define to allow
   * initialization to take place.
   */
  public abstract int open (Object obj);

  /**
   * Provide a default implementation to simplify ancestors.
   *@return 0
   */
  public int close (long flags)
  {
    return 0;
  }

  /**
   * Provide a default implementation to simplify ancestors.
   *@return -1
   */
  public int put (MessageBlock mb, TimeValue tv)
  {
    return -1;
  }

  /**
   * Provide a default implementation to simplify ancestors.
   *@param tv Time Value for which timer was set
   *@param obj An arbitrary object that was passed to the Timer Queue
   * (Asynchronous Completion Token)
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    return -1;
  }
  
  protected SOCKStream stream_;
}
