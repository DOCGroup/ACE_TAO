/*************************************************
 *
 * = PACKAGE
 *    JACE.Reactor
 *
 * = FILENAME
 *    TimeValue.java
 *
 *@author Prashant Jain
 *
 *************************************************/
//package JACE.Reactor;
package JACE.ASX;

public class TimeValue
{
  public final static TimeValue zero = new TimeValue (0,0);

  /** 
   * Default constructor.  This creates a TimeValue that is
   * equal to TimeValue.zero.
   */
  public TimeValue ()
  {
    this (0, 0);
  }

  /** 
   * Constructor
   *@param sec seconds
   */
  public TimeValue (long sec)
  {
    this (sec, 0);
  }

  /** 
   * Constructor
   *@param sec seconds
   *@param nanos nanoseconds
   */
  public TimeValue (long sec, int nanos)
  {
    this.set (sec, nanos);
  }

  /** 
   * Sets the seconds and nanoseconds of Time Value
   *@param sec seconds
   *@param nanos nanoseconds
   */
  public void set (long sec, int nanos)
  {
    this.millisec_ = sec * 1000;
    this.nanos_ = nanos;
    this.normalize ();
  }

  /** 
   * Get seconds
   *@return Seconds
   */
  public long sec ()
  {
    return this.millisec_/1000;
  }

  /** 
   * Get nanoseconds
   *@return Nanoseconds
   */
  public int nanos ()
  {
    return this.nanos_;
  }

  /** 
   * Get time in milliseconds.
   *@return time in milliseconds
   */
  public long getMilliTime ()
  {
    return this.millisec_;
  }

  /**
   * Get a String representation of the Time Value.
   *@return String representation of the Time Value
   */
  public String toString ()
  {
    return (new Long (this.millisec_/1000)).toString () + ":" +
      (new Integer (this.nanos_)).toString ();
  }

  /** 
   * Get current time.
   *@return the current system time as a new TimeValue
   */
  public static TimeValue getTimeOfDay ()
  {
    return new TimeValue (System.currentTimeMillis ()/1000);
  }

  /**
   * Return a new TimeValue that represents the current system time 
   * of day offset by the given number of seconds and nanoseconds.  
   *@param sec Number of seconds to offset by
   *@param nanos Number of nanoseconds to offset by
   *@see JACE.ASX.TimeValue
   *@return TimeValue for the system time plus the given offset
   */
  public static TimeValue relativeTimeOfDay(long sec, int nanos) 
  {
    return new TimeValue ((System.currentTimeMillis() / 1000) + sec,
			 nanos);
  }  

  /**
   * Return a new TimeValue that represents the current system time 
   * of day offset by the given TimeValue.
   *@param tv TimeValue to offset by
   *@see JACE.ASX.TimeValue
   *@return TimeValue for the system time plus the given offset
   */
  public static TimeValue relativeTimeOfDay(TimeValue offset)
  {
    return new TimeValue ((System.currentTimeMillis() / 1000) +
			  offset.sec(),
			  offset.nanos());
  }

  /**
   * Compare two Time Values for equality.
   *@param tv Time Value to compare with
   *@return true if the two Time Values are equal, false otherwise
   */
  public boolean equals (TimeValue tv)
  {
    return this.millisec_ == (tv.sec () * 1000) && this.nanos_ == tv.nanos ();
  }

  /**
   * Compare two Time Values for non-equality.
   *@param tv Time Value to compare with
   *@return true if the two Time Values are not equal, false otherwise
   */
  public boolean notEquals (TimeValue tv)
  {
    return !this.equals (tv);
  }

  /**
   * Add two Time Values.
   *@param tv1 The first Time Value
   *@param tv2 The second Time Value
   *@return sum of the two Time Values.
   */
  public static TimeValue plus (TimeValue tv1, TimeValue tv2)
  {
    TimeValue tv = new TimeValue (tv1.sec () + tv2.sec (), 
				  tv1.nanos () + tv2.nanos ());
    tv.normalize ();
    return tv;
  }

  /**
   * Subtract two Time Values.
   *@param tv1 The first Time Value
   *@param tv2 The second Time Value
   *@return difference of the two Time Values.
   */
  public static TimeValue minus (TimeValue tv1, TimeValue tv2)
  {
    TimeValue tv = new TimeValue (tv1.sec () - tv2.sec (), 
				  tv1.nanos () - tv2.nanos ());
    tv.normalize ();
    return tv;
  }

  /**
   * Add Time Value to "this".
   *@param tv The Time Value to add to this.
   */
  public void plusEquals (TimeValue tv)
  {
    this.set (this.sec () + tv.sec (),
	      this.nanos () + tv.nanos ());
    this.normalize ();
  }

  /**
   * Subtract Time Value from "this".
   *@param tv The Time Value to subtract from this.
   */
  public void minusEquals (TimeValue tv)
  {
    this.set (this.sec () - tv.sec (),
	      this.nanos () - tv.nanos ());
    this.normalize ();
  }

  /**
   * Compare two Time Values for less than.
   *@param tv Time Value to compare with
   *@return true if "this" is less than tv, false otherwise
   */
  public boolean lessThan (TimeValue tv)
  {
    return tv.greaterThan (this);
  }

  /**
   * Compare two Time Values for greater than.
   *@param tv Time Value to compare with
   *@return true if "this" is greater than tv, false otherwise
   */
  public boolean greaterThan (TimeValue tv)
  {
    if (this.sec () > tv.sec ())
      return true;
    else if (this.sec () == tv.sec ()
	     && this.nanos () > tv.nanos ()) 
      return true;
    else
      return false;
  }

  /**
   * Compare two Time Values for <=.
   *@param tv Time Value to compare with
   *@return true if "this" <= tv, false otherwise
   */
  public boolean lessThanEqual (TimeValue tv)
  {
    return tv.greaterThanEqual (this);
  }

  /**
   * Compare two Time Values for >=.
   *@param tv Time Value to compare with
   *@return true if "this" >= tv, false otherwise
   */
  public boolean greaterThanEqual (TimeValue tv)
  {
    return this.sec () >= tv.sec () && this.nanos () >= tv.nanos ();
  }

  private void normalize ()
  {
    if (this.nanos_ >= ONE_MILLISECOND)
      {
	do
	  { 
	    this.millisec_++;
	    this.nanos_ -= ONE_MILLISECOND;
	  }
	while (this.nanos_ >= ONE_MILLISECOND);
      }
    else if (this.nanos_ <= -ONE_MILLISECOND)
      {
	do
	  { 
	    this.millisec_--;
	    this.nanos_ += ONE_MILLISECOND;
	  }
	while (this.nanos_ <= -ONE_MILLISECOND);
      }
    
    if (this.millisec_ >= 1 && this.nanos_ < 0)
      {
	this.millisec_--;
	this.nanos_ += ONE_MILLISECOND;
      }
    else if (this.millisec_ < 0 && this.nanos_ > 0)
      {
	this.millisec_++;
	this.nanos_ -= ONE_MILLISECOND;
      }
  }

  private long millisec_;
  private int nanos_;
  private final static int ONE_MILLISECOND = 1000000;
}
