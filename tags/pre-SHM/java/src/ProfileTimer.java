/*************************************************
 *
 * = PACKAGE
 *    JACE.Timers
 *
 * = FILENAME
 *    ProfileTimer.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Timers;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *     A Java wrapper for interval timers.
 */
public class ProfileTimer
{
  /**
   * Start the timer.
   */
  public void start ()
    {
      this.startTime_ = java.lang.System.currentTimeMillis ();
    }

  /**
   * Stop the timer.
   */
  public void stop ()
    {
      this.stopTime_ = java.lang.System.currentTimeMillis ();
    }

  /**
   * Determine elapsed time between start and stop.
   *@return Total elapsed time (stop - start).
   */
  public long elapsedTime ()
    {
      return this.stopTime_ - this.startTime_;
    }

  private long startTime_;
  private long stopTime_;
}
