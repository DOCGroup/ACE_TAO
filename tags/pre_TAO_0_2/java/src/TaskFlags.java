/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    TaskFlags.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

public abstract class TaskFlags
{
  /** Identifies a Task as being the "reader" in a Module. */
  public static final int ACE_READER    = 01;

  /** Just flush data messages in the queue. */
  public static final int ACE_FLUSHDATA = 02;

  /** Flush all messages in the Queue. */
  public static final int ACE_FLUSHALL  = 04;

  /** Flush read queue */
  public static final int ACE_FLUSHR    = 010;

  /** Flush write queue */
  public static final int ACE_FLUSHW    = 020;

  /** Flush both queues */
  public static final int ACE_FLUSHRW   = 030;

  /** Identifies a thread as suspended */
  public static final int THR_SUSPENDED = 0x00000080;

  /** Identifies a thread as a daemon thread */
  public static final int THR_DAEMON    = 0x00000100;

  // Default private constructor to avoid instantiation
  private TaskFlags ()
  {
  }
}
