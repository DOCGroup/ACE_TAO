/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    IOCntlMsg.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

import JACE.OS.*;

/**
 * Data format for IOCTL messages.
 */
public class IOCntlMsg
{

  // = Initialization method.

  /*
   * Initialize the control message.
   *@param c IOCntlCmd for the control message. Note that this should
   * be of type IOCntlCmds
   */
  public IOCntlMsg (int c) 
  { 
    this.cmd_ = c; 
  }

  // = Get/set methods

  /*
   * Get the command.
   *@return the command.
   */
  public int cmd () 
  { 
    return this.cmd_; 
  }

  /*
   * Set the command.
   *@param c the command.
   */
  public void cmd (int c) 
  { 
    this.cmd_ = c; 
  }

  /*
   * Get the count.
   *@return the count.
   */
  public int count () 
  { 
    return this.count_; 
  }

  /*
   * Set the count.
   *@param c the count.
   */
  public void count (int c) 
  { 
    this.count_ = c; 
  }

  /*
   * Get the error.
   *@return the error.
   */
  public int error () 
  { 
    return this.error_; 
  }

  /*
   * Set the error.
   *@param e the error.
   */
  public void error (int e) 
  { 
    this.error_ = e; 
  }

  /*
   * Get the return value.
   *@return the return value.
   */
  public int rval () 
  { 
    return this.rval_; 
  }

  /*
   * Set the return value.
   *@param r the return value.
   */
  public void rval (int r) 
  { 
    this.rval_ = r; 
  }

  public String toString ()
  {
    return (new Integer (this.cmd_)).toString ();
  }

  private int cmd_;
  // Command.

   private int count_;
   // Count.

   private int error_;
   // Error.

   private int rval_;
   // Return value
}
