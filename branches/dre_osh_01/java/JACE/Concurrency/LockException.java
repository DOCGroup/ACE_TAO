package JACE.Concurrency;

/**
 * Base class for possible exceptions thrown from Lock
 * mechanisms.  This can be used by later Lock
 * implementations to signal special types of exceptions, such
 * as a remote failure, etc.
 * <P>
 */
public class LockException extends java.lang.Exception 
{
  /** 
   * Default constructor
   */
  public LockException () { }
  
  /**
   * Constructor with a string message that will be returned
   * via the getMessage() method on Exception.
   * <P>
   *@see java.lang.Exception#getMessage
   */
  public LockException (String message) 
  {
    super(message);
  }
}

