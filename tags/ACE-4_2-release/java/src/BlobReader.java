/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    BlobReader.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import JACE.ASX.*;
import JACE.OS.*;

// Reader **************************************************

public class BlobReader
{
  public BlobReader (int length, 
		     int offset, 
		     String filename,
		     String hostname,
		     int port)
  {
    this.length_ = length;
    this.offset_= offset;
    this.filename_ = filename;    
    this.hostname_ = hostname;
    this.port_ = port;
  }


  public MessageBlock receiveData ()
  {
    String hostname = this.hostname_;
    String filename = this.filename_;

    // Check if the filename begins with a "/" and if so, remove it
    // since we are concatenating a "/" to the hostname.
    if (this.filename_.startsWith ("/"))
      filename = this.filename_.substring (1);
    
    hostname = hostname + ":" + this.port_ + "/";
    //    System.out.println (hostname + filename);
    
    // Allocate a buffer to hold the offset worth of data
    byte tempBuf [] = new byte [this.offset_];
    // Allocate a buffer to hold the actual data
    byte dataBuf [] = new byte [this.length_];

    try
      {
	// Create a URL to fetch the file
	URL url = new URL (this.protocol_ + hostname + filename);
	
	// Get the input stream and pipe it to a DataInputStream
	DataInputStream iStream = new DataInputStream (url.openStream ());

	// Read the offset worth of bytes
	iStream.readFully (tempBuf, 0, this.offset_);

	// Read length worth of bytes
	iStream.readFully (dataBuf, 0, this.length_);
      }
    catch (MalformedURLException e)
      {
	ACE.ERROR (e);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
    // Cache number of bytes read
    this.bytesRead_ = this.length_;
    return new MessageBlock (new String (dataBuf, 0));
  }

  public int close (long flags)
  {
    return 0;
  }

  public int bytesRead ()
  {
    return this.bytesRead_;
  }

  private String protocol_ = "http://";

  int length_ = 0;
  int offset_= 0;
  String filename_ = null;
  String hostname_ = "localhost";
  int port_ = 80;

  int bytesRead_ = 0;
}


