/*************************************************
 *
 * = PACKAGE
 *    JACE.SOCK_SAP
 *
 * = FILENAME
 *    SOCKStream.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.SOCK_SAP;

import java.io.*;
import java.net.*;
import JACE.OS.*;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *     Defines the methods in the ACE.SOCKStream abstraction.
 *     
 * <p><b>DESCRIPTION</b><br>
 *     This adds additional wrapper methods atop the java Socket class.
 */
public class SOCKStream
{
  /**
   * Create a default SOCK Stream. Do nothing constructor.
   */
  public SOCKStream ()
    {
    }

  /**
   * Create a SOCK Stream.
   *@param s Socket to initialize SOCK Stream with.
   */
  public SOCKStream (Socket s) throws IOException
    {
      this.socket (s);
    }

  /**
   * Set the socket and the underlying streams.
   *@param s Socket associated with the SOCK Stream.
   */
  public void socket (Socket s) throws IOException
  {
    this.socket_ = s;
    // Note that if s is not a valid socket or is null, the
    // following calls will throw exceptions
    this.iStream_ = new DataInputStream (s.getInputStream ());
    this.oStream_ = new PrintStream (s.getOutputStream ());
  }

  /* Get the underlying Socket.
   *@return the underlying socket
   */
  public Socket socket ()
    {
      return this.socket_;
    }

  /**
   * Close the streams and the underlying socket.
   */
  public void close () throws IOException
    {
      if (this.socket_ != null)
	this.socket_.close ();
      this.socket_ = null;
    }
  
  // = The following send and recv methods are overloaded to provide a
  // flexible interface

  /**
   * Send a StringBuffer. Note that the method blocks.
   *@param s the StringBuffer to send
   *@return the length of the StringBuffer
   */
  public int send (StringBuffer s) throws IOException
    {
      // Get the data out
      String buf = s.toString ();
      this.oStream_.println (buf);
      this.oStream_.flush ();
      return buf.length ();
    }

  /**
   * Send a String. Note that the method blocks.
   *@param s the String to send
   *@return the length of the String
   */
  public int send (String s) throws IOException
    {
      this.oStream_.println (s);
      this.oStream_.flush ();
      return s.length ();
    }

  /** 
   * Send an array of bytes. Note that the method blocks.
   *@param b array of bytes to send
   *@param offset offset into the byte array to start sending from
   *@param length number of bytes to send
   *@return number of bytes sent
   */
  public int sendN (byte[] b, int offset, int length) throws IOException
    {
      this.oStream_.write (b, offset, length);
      this.oStream_.flush ();
      return length;
    }

  /**
   * Receive data and append it to the StringBuffer that was passed
   * in. Note that the method blocks.
   *@param s the StringBuffer to append the result of the recv to
   *@return the length of the String received
   */
  public int recv (StringBuffer s) throws IOException
    {
      String temp = this.iStream_.readLine ();
      s.append (temp);
      return temp.length ();
    }

  /**
   * Receive an array of characters. This method blocks until either
   * all the bytes are read, the end of the stream is detected, or
   * an exception is thrown.
   *@param b byte array to receive the data in
   *@param offset the start offset of the data in the byte array.
   *@param n number of bytes to receive
   *@return n
   */
  public int recvN (byte[] b, int offset, int n) throws IOException
    {
      this.iStream_.readFully (b, offset, n);
      return n;
    }

  /**
   * Set the underlying input stream.
   *@param iStream the input stream
   */
  public void inputStream (InputStream iStream)
    {
      this.iStream_ = new DataInputStream (iStream);
    }

  /**
   * Get the underlying input stream.
   *@return the underlying input stream
   */
  public InputStream inputStream ()
    {
      return this.iStream_;
    }

  /**
   * Set the underlying output stream.
   *@param iStream the output stream
   */
  public void outputStream (OutputStream oStream)
    {
      this.oStream_ = new PrintStream (oStream);
    }

  /**
   * Get the underlying output stream.
   *@return the underlying output stream
   */
  public OutputStream outputStream ()
    {
      return this.oStream_;
    }

  /**
   * Cleanup when the SOCK Stream is garbage collected.
   */
  protected void finalize () throws Throwable
    {
      super.finalize ();
      this.close ();
    }
  
  private Socket socket_;

  // = The input and output streams (by default null)
  private DataInputStream iStream_;
  private PrintStream oStream_;
}
