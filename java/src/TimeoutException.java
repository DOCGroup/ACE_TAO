/*************************************************
 *
 * = PACKAGE
 *    ACE.ASX
 *
 * = FILENAME
 *    TimeoutException.java
 *
 *@author Prashant Jain and Doug Schmidt
 *
 *************************************************/
package ACE.ASX;

import ACE.Reactor.*;

public class TimeoutException extends Exception
{ 
  /**
   * Default Constructor.
   */
  public TimeoutException ()
  {
    super ("Timed Out");
  }

  /**
   * Constructor.
   *@param timeout The timeout value which expired.
   *@param desc Textual description of the exception
   */
  public TimeoutException (TimeValue timeout, String desc)
  {
    super ("Timed Out in " + timeout + ": " + desc);
  }

}
