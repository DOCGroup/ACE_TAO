package JACE.netsvcs.Time;

/**
 * Wrapper for use with the clerk, containing a sequence number and
 * time offset pair.
 */
public class TimeInfo
{
  /**
   * Default constructor.
   */
  public TimeInfo ()
  {
    this (0, 0);
  }

  /** 
   * Constructor.
   *
   *@param seqNum sequence number
   *@param delta time offset in seconds
   */
  public TimeInfo (int seqNum, long delta)
  {
    sequenceNumber_ = seqNum;
    delta_ = delta;
  }

  /** 
   * Returns this TimeInfo's sequence number.
   */
  public int sequenceNumber ()
  {
    return sequenceNumber_;
  }

  /** 
   * Sets this TimeInfo's sequence number.
   */
  public void sequenceNumber (int num)
  {
    sequenceNumber_ = num;
  }

  /**
   * Returns the time offset represented by this TimeInfo instance. 
   * (in sec)
   */
  public long delta ()
  {
    return delta_;
  }

  /**
   * Sets the time offset (in sec).
   */
  public void delta (long num)
  {
    delta_ = num;
  }

  /**
   * Returns an informative String about the time difference represented
   * by this TimeInfo instance.  The sequence number is included in
   * brackets.
   * <P>
   * Example:
   * <PRE>
   * Local time is 3 sec slower [57]
   * </PRE>
   */
  public String toString ()
  {
    String result = "Local time is ";
    if (delta_ > 0) {
      result += (delta_ + " sec slower");
    } else
    if (delta_ < 0) {
      result += (delta_ + " sec faster");
    } else
      result += "the same as the average";

    result += " [" + sequenceNumber_ + "]";

    return result;
  }

  private long delta_;
  private int sequenceNumber_;
}
