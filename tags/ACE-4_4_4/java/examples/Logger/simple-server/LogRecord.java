/**
 * Class used to communicate logging information; compatible with
 * the C++ ACE ACE_Log_Record class.
 *
 * @author Chris Cleeland
 */

//package ACE.SimpleLogger;

import java.util.Date;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.PrintStream;
import java.io.IOException;

public class LogRecord
{
  final public int MAXLOGMSGLEN = 4 * 1024;

  private int type_;
  private int length_;
  private int timeStamp_;
  private int pid_;
  private byte[] msgData_ = new byte[MAXLOGMSGLEN];
  private final static int numIntMembers = 4;
  private final static int sizeofIntInBytes = 4;

  /**
   * Create a default instance.
   */
  public LogRecord()
  {
    this(0, (int) ((new Date()).getTime()/1000), 0);
  }

  /**
   * Create a LogRecord.  This is the designated initializer.
   * @param priority a numeric specification of the priority (ascending)
   * @param time_stamp time attached to the log entry in Unix <pre>time_t</pre> format
   * @param pid the process ID; not currently used
   */
  public LogRecord(int priority,
		   int timeStamp,
		   int pid)
  {
    type(priority);
    timeStamp(timeStamp);
    length(0);
    pid(pid);
  }

  /**
   * Conversion to string.  Only includes the <pre>msgData_</pre> member.
   */
  public String toString()
  {
    return new String(msgData_, 0);
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
	long cur = (long)timeStamp() * (long)1000;
	Date now = new Date(cur);

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
    type(dis.readInt());
    length(dis.readInt());
    timeStamp(dis.readInt());
    pid(dis.readInt());

    // Does readFully() allocate space for the buffer?  Either
    // way, we won't have memory leaks :-)
    int dataLength = (int) (length_ - numIntMembers * sizeofIntInBytes);
    msgData_ = new byte[dataLength];
    dis.readFully(msgData_, 0, dataLength);
  }

  public void streamOutTo(DataOutputStream dos) throws IOException
  {
    dos.writeInt(type());
    dos.writeInt(length());
    dos.writeInt(timeStamp());
    dos.writeInt(pid());
    int dataLength = (int) (length_ - numIntMembers * sizeofIntInBytes);
    dos.write(msgData_, 0, dataLength);
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

  public int timeStamp()      { return timeStamp_; }
  public void timeStamp(int t){ timeStamp_ = t; }

  public int pid()             { return pid_; }
  public void pid(int p)       { pid_ = p; }

  public byte[] msgData()      { return msgData_; }
  public void msgData(byte[] m){ msgData_ = m; setLen(m.length); }
  public void msgData(String m)
  {
    m.getBytes(0, m.length(), msgData_, 0);
    setLen(m.length());
  }
};

