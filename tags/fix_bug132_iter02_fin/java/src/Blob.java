/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    Blob.java
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

public class Blob
{
  public int open (String filename, String hostname , int port) 
  {
    this.filename_ = filename;
    this.hostname_ = hostname;
    this.port_ = port;
    return 0;
  }

  public MessageBlock read (int length, int offset) 
  {
    // Check if we have a valid length and a valid offset
    if (length < 0 || offset < 0)
      {
	ACE.ERROR ("Blob::read(): Negative length or offset");
	return null;
      }
    
    // Create a Blob Reader
    BlobReader blobReader = new BlobReader (length, offset, this.filename_, this.hostname_, this.port_);

    // Receive data
    MessageBlock mb = blobReader.receiveData ();
    if (blobReader.bytesRead () != length)
      return null;
    else
      return mb;
  }

  public int write (MessageBlock mb, int length, int offset) 
  {
    // Check if we have a valid length and a valid offset
    if (length < 0 || offset < 0)
      ACE.ERROR ("Blob::write(): Negative length or offset");
    
    // Create a Blob Writer
    BlobWriter blobWriter = new BlobWriter (mb, length, offset, this.filename_);
    
    try
      {
	// Connect to the server
	this.connector_.open (this.hostname_, this.port_);
	this.connector_.connect (blobWriter);
      }
    catch (UnknownHostException e)
      {
	ACE.ERROR (e);
      }
    catch (InstantiationException e)
      {
	ACE.ERROR (e);
      }
    catch (IllegalAccessException e)
      {
	ACE.ERROR (e);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
    
    return blobWriter.bytesWritten ();
  }

  public int close () 
  {
    return 0;
  }

  String filename_;
  String hostname_;
  int port_;
  Connector connector_ = new Connector ();
}
