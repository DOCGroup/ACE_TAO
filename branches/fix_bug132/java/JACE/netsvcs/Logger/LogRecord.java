/*************************************************
 *
 * = PACKAGE
 *    JACE.netsvcs.Logger
 *
 * = FILENAME
 *    LogRecord.java
 *
 *@author Chris Cleeland, Everett Anderson
 *
 *************************************************/
package JACE.netsvcs.Logger;

import java.util.*;
import java.io.*;
import JACE.OS.*;

/**
 * Communicates logging information.  Compatible with the C++ ACE
 * ACE_Log_Record class.
 */
public class LogRecord
{
  /**
   * Maximum size of a LogRecord
   */
  final public int MAXLOGMSGLEN = 4 * 1024;

  private int type_;
  private int length_;
  private long msec_;
  private int pid_;
  private byte[] msgData_;
  private final static int numIntMembers = 5;
  private final static int sizeofIntInBytes = 4;

  /**
   * Create a default instance.
   */
  public LogRecord()
  {
    type(0);
    timeStamp((int)new Date().getTime());
    length(0);
    pid(0);
  }

  /**
   * Create a LogRecord.  This is the designated initializer.
   * @param priority a numeric specification of the priority (ascending)
   * @param milliseconds time attached to the log entry in Unix <pre>time_t</pre> format 
   * @param pid the process ID 
   */
  public LogRecord(int priority,
		   long milliseconds,
		   int pid)
  {
    type(priority);
    timeStamp(milliseconds);
    length(0);
    pid(pid);
  }

  /**
   * Create a LogRecord with the current time and the given message.
   *
   *@param message message to log
   */
  public LogRecord (String message)
  {
    this ();

    msgData (message);
  }

  /**
   * Conversion to string.  Only includes the <pre>msgData_</pre> member.
   */
  public String toString()
  {
    String result = null;
    try {
      result = new String (msgData_,
			   "US-ASCII");
    } catch (UnsupportedEncodingException e) {
      result = new String (msgData_);
    }

    return result;
  }

  /**
   * Place a textual representation of the record on a PrintStream.
   * When verbose is specified to be true, the output takes the form
   * <PRE>(Date)@(host name)@(PID)@(type)@(message)</PRE>
   * Otherwise it just prints the message.
   * @param hostname name of the host generating this record
   * @param verbose specify how much information to print (see above)
   * @param ps A PrintStream instance to which the output should go.
   */
  public void print(String hostname,
		    boolean verbose,
		    PrintStream ps)
  {
    String toprint;
    if (verbose)
      {
	Date now = new Date(this.timeStamp());

	/* 01234567890123456789012345 */
	/* Wed Oct 18 14:25:36 1989n0 */
	toprint = now.toString().substring(4) + "@"
                + hostname + "@" + pid_ + "@" + type_ + "@"
                + this.toString();
      }
    else
      {
	toprint = this.toString();
      }
    ps.println(toprint);
  }

  /**
   * Read in the data for this LogRecord from the given InputStream.
   *
   *@param is InputStream to read from
   *@exception IOException error during transmission
   */
  public void streamInFrom (InputStream is) throws IOException
  {
    BufferedInputStream bis = new BufferedInputStream (is);

    DataInputStream dis = new DataInputStream (bis);

    streamInFrom (dis);
  }

  /**
   * Read in the data for this LogRecord from the given DataInputStream.
   *
   *@param dis DataInputStream to read from
   *@exception IOException error during transmission
   */
  public void streamInFrom(DataInputStream dis) throws IOException
  {
    // Order here must match layout order in the C++ class.
    length(dis.readInt());
    type(dis.readInt());
    this.timeStamp((long)dis.readInt() * 1000);

    // Skip smaller time resolution info since we're lucky if Java's
    // timer can handle more than millisecond precision, anyway
    dis.skipBytes(4); 

    pid(dis.readInt());

    int dataLength = (int) (length_ - numIntMembers * sizeofIntInBytes);

    msgData_ = new byte[dataLength];

    dis.readFully(msgData_, 0, dataLength);
  }

  /** 
   * Write this LogRecord out to the given OutputStream.
   *
   *@param os OutputStream to write to
   *@exception IOException error during transmission
   */
  public void streamOutTo (OutputStream os) throws IOException
  {
    BufferedOutputStream bos = new BufferedOutputStream (os);

    DataOutputStream dos = new DataOutputStream (bos);

    streamOutTo (dos);
  }

  /**
   * Write this LogRecord out to the given DataOutputStream.
   *
   *@param dos OutputStream to write to
   *@exception IOException error during transmission
   */
  public void streamOutTo(DataOutputStream dos) throws IOException
  {
    dos.writeInt(length());
    dos.writeInt(type());
    dos.writeInt((int)(this.msec_ / 1000));
    dos.writeInt(0);  
    dos.writeInt(pid());

    dos.write(msgData_);

    dos.flush ();
  }

  /**
   * Return the LogRecord type.
   */
  public int type()            { return type_; }

  /**
   * Set the LogRecord type.
   */
  public void type(int t)      { type_ = t; }

  /**
   * Return the length of this LogRecord.
   */
  public int length()          { return length_;  }

  /**
   * Set the length of this LogRecord.
   */
  public void length(int l)    { length_ = l; }

  /**
   * Calculate the length of this LogRecord from the size of
   * the message and the header.
   */
  private void setLen(int msgLen)
       { length(msgLen + numIntMembers * sizeofIntInBytes); }

  /**
   * Return the millisec time stamp of this LogRecord.
   */
  public long timeStamp()      { return this.msec_; }

  /**
   * Set the millisec time stamp of this LogRecord.
   */
  public void timeStamp(long msec){ this.msec_ = msec; }

  /**
   * Return the PID of this LogRecord.
   */
  public int pid()             { return pid_; }

  /**
   * Set the PID of this LogRecord.
   */
  public void pid(int p)       { pid_ = p; }

  /**
   * Return the message of this LogRecord as a byte array.
   */
  public byte[] msgData()      { return msgData_; }

  /**
   * Set the message of this LogRecord to a given byte array.
   */
  public void msgData(byte[] m)
    { 
      int size = m.length;

      if (size > MAXLOGMSGLEN) 
	size = MAXLOGMSGLEN;

      this.msgData_ = new byte[size];
 
      System.arraycopy(m, 0, msgData_, 0, size);

      setLen(size);
    }

  /**
   * Set the message of this LogRecord to a given byte array.  First
   * tries to use US-ASCII encoding, then uses the default encoding
   * if that fails.  The toString method is essentially the opposite
   * version.
   */
  public void msgData(String m)
  {
    byte temp[] = null;
    try {
      temp = m.getBytes("US-ASCII");
    } catch (UnsupportedEncodingException e) {
      temp = m.getBytes ();
    }
    if (temp.length > MAXLOGMSGLEN) {
      this.msgData_ = new byte[MAXLOGMSGLEN];
      
      System.arraycopy(temp, 0, msgData_, 0, MAXLOGMSGLEN);
    } else
      this.msgData_ = temp;
    
    setLen(msgData_.length);
  }
}
