/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameReply.java
 *
 *************************************************/
package JACE.netsvcs.Naming;

import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.*;
import JACE.SOCK_SAP.*;

/**
 * Used by the naming server to give quick status messages
 * to the client.  This is only used to signal the success or
 * failure of bind and unbind requests.  The error number seems
 * to be unused in the C++ version.
 *
 *@see NameHandler
 *@author Everett Anderson
 *
 */
public class NameReply
{
  /** Successful operation indicator */
  public final static int SUCCESS = 0;

  /** Failed operation indicator */
  public final static int FAILURE = -1;

  /**
   * Default Constructor (success, errno 0)
   */
  public NameReply () 
  { 
    this.type_ = this.SUCCESS;
    this.errno_ = 0;
  }

  /**
   * Constructor
   *
   *@param type    Success or failure
   *@param err     Error number (unused)
   */
  public NameReply (int type, int err)
  {
    this.type_ = type;
    this.errno_ = err;
  }

  /**
   * Length accessor
   */
  int length() 
  { return this.length_; }

  /**
   * Type accessor -- success or failure
   */
  int type()
  { return this.type_; }

  /**
   * Error number accessor 
   */
  int errno()
  { return this.errno_; }

  /** 
   * Set type
   * @param type    New type
   */
  void type(int type)
  { this.type_ = type; }

  /**
   * Set error number
   * @param errno     New error number
   */
  void errno(int errno)
  { this.errno_ = errno; }

  /**
   * Send this data to the given SOCKStream.
   *
   *@param sock     SOCKStream to send to
   */
  public void streamOutTo (JACE.SOCK_SAP.SOCKStream sock) throws IOException
  {
    streamOutTo (sock.dataOutputStream ());
  }

  /**
   * Send this instance to the given DataOutputStream.
   */
  public void streamOutTo (DataOutputStream dos) throws IOException
  {
    dos.writeInt(this.length_);
    dos.writeInt(this.type_);
    dos.writeInt(this.errno_);
    
    dos.flush();
  }

  /**
   * Send this instance to the given OutputStream.
   */
  public void streamOutTo (OutputStream os) throws IOException
  {
    BufferedOutputStream bos = new BufferedOutputStream (os);
    DataOutputStream dos = new DataOutputStream (bos);

    streamOutTo (dos);
  }

  /**
   * Fill the fields of this instance from data in the socket
   *
   *@param sock     SOCKStream to read from
   */
  public void streamInFrom (JACE.SOCK_SAP.SOCKStream sock) throws IOException
  {
    this.streamInFrom(sock.dataInputStream ());
  }

  /**
   * Fill this instance from the DataInputStream (which should be buffered).
   *
   *@param dis      DataInputStream to use
   */
  public void streamInFrom (DataInputStream dis) throws IOException
  {
    int length = dis.readInt();
      
    if (length != this.length_) 
      throw new IOException("Incorrect NameReply length");
      
    type_ = dis.readInt();
    errno_ = dis.readInt();
  }

  /**
   * Fill this instance from the given InputStream.
   */
  public void streamInFrom (InputStream is) throws IOException
  {
    BufferedInputStream bis = new BufferedInputStream (is);
    DataInputStream dis = new DataInputStream (bis);

    streamInFrom (dis);
  }

  final static int length_ = 12;

  int type_;
  int errno_;
}

  
  
  
