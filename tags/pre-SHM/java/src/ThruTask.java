/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    ThruTask.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;
    
/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Standard module that acts as a "no op", simply passing on all
 *     data to its adjacent neighbor.
 *</blockquote>
 */
public class ThruTask extends Task
{
  public int open (Object obj)
  {
    return 0;
  }

  public int close (long flags)
  {
    return 0;
  }
  
  public int put (MessageBlock msg, TimeValue tv)
  {
    return this.putNext (msg, tv);
  }

  public int svc ()
  {
    return -1;
  }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }
}
