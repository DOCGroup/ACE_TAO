/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    LogRecord.java
 *
 *@author Chris Cleeland, Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;

import java.util.Date;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.PrintStream;
import java.io.IOException;
import JACE.OS.*;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * Communicates logging information.  Compatible with the C++ ACE
 * ACE_Log_Record class.
 *
 */
public class LogRecord
{
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
   * @param pid the process ID; not currently used
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
   * Conversion to string.  Only includes the <pre>msgData_</pre> member.
   */
  public String toString()
  {
    return new String(msgData_);
  }

  /**
   * Place a textual representation of the record on a PrintStream.
   * @param hostname name of the host generating this record
   * @param verbose if <b>true</b>, print information in the form, (give example)
   * @param ps A PrintStream instance to which the output should go.
   * @see PrintStream,String
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
   * Streaming methods
   */
  public void streamInFrom(DataInputStream dis) throws IOException
  {
    // Order here must match layout order in the C++ class.
    // This, of course, is VERY fragile, and ought not be used as
    // a model for anything except how NOT to do anything.
    length(dis.readInt());
    type(dis.readInt());
    this.timeStamp((long)dis.readInt() * 1000);

    // Skip smaller time resolution info since we're lucky if Java's
    // timer can handle more than millisecond precision, anyway
    dis.skipBytes(4); 

    pid(dis.readInt());

    // Does readFully() allocate space for the buffer?  Either
    // way, we won't have memory leaks :-)
    int dataLength = (int) (length_ - numIntMembers * sizeofIntInBytes);

    msgData_ = new byte[dataLength];
    dis.readFully(msgData_, 0, dataLength);
  }

  public void streamOutTo(DataOutputStream dos) throws IOException
  {
    dos.writeInt(length());
    dos.writeInt(type());
    dos.writeInt((int)(this.msec_ / 1000));
    dos.writeInt(0);  
    dos.writeInt(pid());

    dos.write(msgData_);
  }

  /**
   * Accessor methods
   */
  public int type()            { return type_; }
  public void type(int t)      { type_ = t; }

  public int length()          { return length_;  }
  public void length(int l)    { length_ = l; }
  private void setLen(int msgLen)
       { length(msgLen + numIntMembers * sizeofIntInBytes); }

  public long timeStamp()      { return this.msec_; }
  public void timeStamp(long msec){ this.msec_ = msec; }

  public int pid()             { return pid_; }
  public void pid(int p)       { pid_ = p; }

  public byte[] msgData()      { return msgData_; }
  public void msgData(byte[] m)
    { 
      int size = m.length;

      if (size > MAXLOGMSGLEN) 
	size = MAXLOGMSGLEN;

      this.msgData_ = new byte[size];
 
      System.arraycopy(m, 0, msgData_, 0, size);

      setLen(size);
    }

  public void msgData(String m)
  {
    byte temp[] = m.getBytes();
    if (temp.length > MAXLOGMSGLEN) {
      this.msgData_ = new byte[MAXLOGMSGLEN];
      
      System.arraycopy(temp, 0, msgData_, 0, MAXLOGMSGLEN);
    } else
      this.msgData_ = temp;
    
    setLen(msgData_.length);
  }
};

