package JACE.netsvcs.Time;

import java.io.*;
import java.net.*;

/**
 * Request for a time update (and its reply).  This is compatible with 
 * C++ ACE_Time_Request.  Currently, the Java version always specifies to
 * block forever for requests.
 */
public class TimeRequest
{
  /**
   * Type for requesting updates.
   */
  public static int TIME_UPDATE = 01;

  /**
   * Default constructor, specifies block forever for an update.
   */
  public TimeRequest ()
  {
    messageType_ = TIME_UPDATE;
    blockForever_ = 1;
  }

  /**
   * Constructor specifying the type of request, the current
   * time, and to block forever.
   */
  public TimeRequest (int messageType,
		      int timeSec)
  {
    time_ = timeSec;
    messageType_ = messageType;
    blockForever_ = 1;
  }

  /**
   * Dump all class information to a String.
   */
  public String toString ()
  {
    return "TimeRequest (" + messageType_ +
      ", " + blockForever_ + ", " + secTimeout_ + ", " +
      usecTimeout_ + ", " + time_ + ")";
  }

  /**
   * Read the TimeRequest in from a given InputStream.
   */
  public void streamInFrom (InputStream is) 
    throws IOException, EOFException
  {
    BufferedInputStream bis = new BufferedInputStream (is, 25);
    DataInputStream dis = new DataInputStream (bis);

    streamInFrom (dis);
  }

  /**
   * Read the TimeRequest in from a given DataInputStream.
   */
  public void streamInFrom (DataInputStream dis)
    throws IOException, EOFException
  {
    messageType_ = dis.readInt ();
    blockForever_ = dis.readInt ();
    secTimeout_ = dis.readInt ();
    usecTimeout_ = dis.readInt ();
    time_ = dis.readInt ();
  }

  /**
   * Write this TimeRequest out to a given OutputStream.
   */
  public void streamOutTo (OutputStream os)
    throws IOException
  {
    BufferedOutputStream bos = new BufferedOutputStream (os, 25);
    DataOutputStream dos = new DataOutputStream (bos);

    streamOutTo (dos);
  }

  /** 
   * Write this TimeRequest out to a given DataOutputStream.
   */
  public void streamOutTo (DataOutputStream dos) throws IOException
  {
    dos.writeInt (messageType_);
    dos.writeInt (blockForever_);
    dos.writeInt (secTimeout_);
    dos.writeInt (usecTimeout_);
    dos.writeInt (time_);

    dos.flush ();
  }

  /**
   * Return the time value in seconds.
   */
  public int time ()
  {
    return time_;
  }

  /**
   * Set the time value in seconds.
   */
  public void time (int value)
  {
    time_ = value;
  }

  private int messageType_;
  private int blockForever_;
  private int secTimeout_;
  private int usecTimeout_;
  private int time_;
}
