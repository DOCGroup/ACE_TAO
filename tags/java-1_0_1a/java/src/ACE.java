/*************************************************
 *
 * = PACKAGE
 *    ACE.OS
 *
 * = FILENAME
 *    ACE.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.OS;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 * <blockquote>Constants, utility "functions", etc.</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 * Defines default constants for ACE. Many of these are used for the
 * ACE tests and applications. You may want to change some of these to
 * correspond to your environment. Also, routines for error handling,
 * debugging and bit manipulation are included.
 *</blockquote>
 *
 * <h2>NOTES</h2>
 *<blockquote>
 * This class is non-instantiable, and intended only to provide a constrained
 * namespace.
 *</blockquote>
 */
public abstract class ACE
{
  /**
   * Default port on which a server listens for connections.
   */
  public static final int DEFAULT_SERVER_PORT = 10002;

  /** 
   * Default name to use for a thread group.
   */
  public static final String DEFAULT_THREAD_GROUP_NAME = "ace_thread_group";

  /**
   * Disable debugging. Once debugging is disabled, all ACE.DEBUG
   * statements would be ignored. 
   */
  public static final void disableDebugging ()
  {
    ACE.debug_ = false;
  }

  /**
   * Enable debugging. Once debugging is enabled, all ACE.DEBUG
   * statements get printed.
   */
  public static final void enableDebugging ()
  {
    ACE.debug_ = true;
  }

  /** 
   * Print the string representation of Java Exception.
   *@param e Java exception
   */
  public static final void ERROR (Exception e)
    {
      System.err.println (e);
    }

  /** 
   * Print the string being passed in.
   *@param s a Java String
   */
  public static final void ERROR (String s)
    {
      System.err.println (s);
    }

  /** 
   * Print the string being passed in.
   *@param s A Java String
   *@return Error value passed in
   */
  public static final int ERROR_RETURN (String s, int errorVal)
    {
      System.err.println (s);
      return errorVal;
    }

  /** 
   * Print the string being passed in. Note the behavior will vary
   * depending upon whether debugging is enabled or disabled.
   *@param s a Java String
   */
  public static final void DEBUG (String s)
    {
      if (ACE.debug_)
	System.out.println (s);
    }

  /** 
   * Flush out any data that may be buffered.
   */
  public static final void FLUSH ()
    {
      System.out.flush ();
    }

  /** 
   * Set the bits of WORD using BITS as the mask.
   *@param WORD the bits to be set.
   *@param BITS the mask to use.
   *@return The value obtained after setting the bits.
   */
  public static final long SET_BITS (long WORD, long BITS)
    {
      return WORD | BITS;
    }

  /** 
   * Clear the bits of WORD using BITS as the mask.
   *@param WORD the bits to clear.
   *@param BITS the mask to use.
   *@return The value obtained after clearing the bits.
   */
  public static final long CLR_BITS (long WORD, long BITS)
    {
      return WORD & ~BITS;
    }
  
  /** 
   * Check if bits are enabled in WORD.
   *@param WORD the bits to check.
   *@param BIT the bit to check to see if it is enabled or not.
   *@return true if bit is enabled, false otherwise.
   */
  public static final boolean BIT_ENABLED (long WORD, long BIT)
    {
      return (WORD & BIT) != 0;
    }

  /** 
   * Check if bits are disabled in WORD.
   *@param WORD the bits to check.
   *@param BIT the bit to check to see if it is disabled or not.
   *@return true if bit is disabled, false otherwise.
   */
  public static final boolean BIT_DISABLED (long WORD, long BIT)
    {
      return (WORD & BIT) == 0;
    }
  
  // Debug flag (turn debugging on/off)
  private static boolean debug_ = true;

  // Default private constructor to avoid instantiation
  private ACE ()
  {
  }
}


