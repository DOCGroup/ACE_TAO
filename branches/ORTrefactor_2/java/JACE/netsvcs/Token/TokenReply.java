package JACE.netsvcs.Token;

import java.io.*;
import JACE.ASX.*;
import JACE.OS.*;

/**
 * Reply from a lock operation, and constants involved in it.  
 * This is compatible with the C++ ACE version.  The user probably
 * never deals directly with the constant errno values in Java ACE since
 * the proxy (RemoteLock) should hide those details.
 */
public class TokenReply
{

  /** indicates success */
  public static final int NO_ERRORS = 0;

  /** indicates a timeout */
  public static final int ETIME = 62;

  /** indicates the operation was interrupted */
  public static final int EINTR = 4;

  /** deadlock indication errno (JACE currently doesn't implement a
   * deadlock detection system, but C++ ACE does and JACE proxies
   * act appropriately).
   */
  public static final int EDEADLK = 45;

  /** indicates the operation would block, used in tryAcquire */
  public static final int EWOULDBLOCK = 11;

  /** indicates a token name or client ID was too long */
  public static final int ENAMETOOLONG = 78;

  /** indicates an operation type was not supported */
  public static final int ENOTSUP = 48;

  /** indicates that this client was not the owner of the lock,
   * so couldn't perform the desired operation */
  public static final int EACCES = 13;

  /** indicates an IO error occured during transmission of the request */
  public static final int EIO = 5;

  /** indicates a generic failure to complete the request */
  public static final int EFAULT = 14;

  /** indicates an operation was requested on an unknown type of token */
  public static final int EINVAL = 22;

  /** constant length of a valid token reply */
  private final static int LENGTH = 12;

  /** error code */
  private int errno_;

  /** argument (unused in JACE) */
  private int arg_;
  
  /** Dump the state of this TokenReply to a String */
  public String toString ()
  {
    return "TokenReply(" + this.length() + ", " + this.errno_
      + ", " + this.arg_ + ")";
  }

  /** Default constructor (NO_ERRORS) */
  public TokenReply ()
  {
    errno_ = NO_ERRORS;
    arg_ = 0;
  }
  
  /** Constructor which takes the error code and argument */
  public TokenReply (int errno, int arg)
  {
    errno_ = errno;
    arg_ = arg;
  }

  /**
   * Accessor for the length of this TokenReply.
   */
  public int length ()
  {
    return LENGTH;
  }

  /** Accessor for the error code of this TokenReply. */
  public int errno ()
  {
    return errno_;
  }

  /**
   * Set the error code of this TokenReply.
   */
  public void errno (int value)
  {
    errno_ = value;
  }

  /**
   * Accessor of the argument of this TokenReply.  (Unused in JACE)
   */
  public int arg ()
  {
    return arg_;
  }

  /**
   * Set the argument of this TokenReply.  (Unused in JACE)
   */
  public void arg (int value)
  {
    arg_ = value;
  }

  /**
   * Read this TokenReply in from the given InputStream.
   */
  public void streamInFrom (InputStream is) 
    throws IOException, EOFException
  {
    BufferedInputStream bis = new BufferedInputStream (is, LENGTH);
    DataInputStream dis = new DataInputStream (bis);

    streamInFrom (dis);
  }

  /** 
   * Read this TokenReply in from the given DataInputStream.
   */
  public void streamInFrom (DataInputStream dis) 
    throws IOException, EOFException
  {
    int length = dis.readInt ();
    if (length != LENGTH)
      throw new IOException ("Invalid TokenReply length " + length);

    this.errno_ = dis.readInt ();
    this.arg_ = dis.readInt ();
  }

  /**
   * Write this TokenReply out to the given OutputStream.
   */
  public void streamOutTo (OutputStream os)
    throws IOException
  {
    BufferedOutputStream bos = new BufferedOutputStream (os, LENGTH);
    DataOutputStream dos = new DataOutputStream (bos);

    streamOutTo (dos);
  }

  /**
   * Write this TokenReply out to the given DataOutputStream.
   */
  public void streamOutTo (DataOutputStream dos)
    throws IOException
  {
    dos.writeInt (LENGTH);
    dos.writeInt (this.errno_);
    dos.writeInt (this.arg_);

    dos.flush ();
  }
}
