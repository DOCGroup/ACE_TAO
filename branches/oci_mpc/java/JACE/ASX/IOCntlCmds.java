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

/**
 * Constants used for IOCTL messages.
 *
 *@see JACE.ASX.IOCntlMsg
 */
public abstract class IOCntlCmds
{
  /**  Set the low water mark. */
  public static final int SET_LWM    = 1;

  /** Get the low water mark. */
  public static final int GET_LWM    = 2; 

  /**  Set the high water mark. */
  public static final int SET_HWM    = 3;

  /** Get the high water mark. */
  public static final int GET_HWM    = 4;

  /** Link modules */
  public static final int MOD_LINK    = 5;

  /** Unlink modules */
  public static final int MOD_UNLINK    = 6;
}
