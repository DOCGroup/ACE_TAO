/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    BlobWriter.java
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

// Writer **************************************************

public class BlobWriter extends BlobHandler
{
  public BlobWriter (MessageBlock mb,
		     int length, 
		     int offset, 
		     String filename)
  {
    super (length, offset, filename);
    this.mb_ = mb;
    this.returnCode_ = -1;

  }

  /*******************************
   *  This constructor should be used when using the basic HTTP 1.1 
   *  authentication scheme
   *******************************/
  public BlobWriter (MessageBlock mb,
		     int length, 
		     int offset, 
		     String filename,
		     String authentication)
  {
    super (length, offset, filename);
    this.mb_ = mb;
    this.returnCode_ = -1;
    this.authentication_ = authentication;
  }


  public int open (Object obj)
  {
    if (this.sendRequest () != 0)
      {
	ACE.ERROR ("BlobWriter::open():sendRequest failed");
	return -1;
      }
    else if (this.receiveReply () != 0)
      {
	ACE.ERROR ("BlobWriter::open():receiveReply failed");
	return -1;
      }
    return 0;
  }

  public int close (long flags)
  {
    return 0;
  }

  public int bytesWritten ()
  {
    return this.bytesWritten_;;
  }

  protected int sendRequest ()
  {
    // Check for sanity -- check if we have any data to send.
    if (this.offset_+ this.length_ > this.mb_.length ())
      {
	ACE.ERROR ("BlobWriter::sendRequest():Invalid offset/length");
	return -1;
      }

    if (this.sendHeader () == -1)
      {
	ACE.ERROR ("BlobWriter::sendHeader failed.");
	return -1;
      }
    else
      if (this.sendData () == -1)
      {
	ACE.ERROR ("BlobWriter::sendData failed.");
	return -1;
      }
    return 0;
  }
  
  // Send the header
  protected int sendHeader ()
  {
    String filename = this.filename_;
    // Check if the filename begins with a "/" and if it doesn't, add it
    if (!this.filename_.startsWith ("/"))
      filename = "/" + this.filename_;

    // Create the header, store the actual length in mesglen 
    String mesg = this.requestPrefix_ + " " + filename + " " + this.requestSuffix_;

    if (this.authentication_ != null) 
      mesg += "Authorization: Basic " + JACE.Connection.HTTPHelper.EncodeBase64(this.authentication_) + '\n';

    mesg += "Content-length: " + this.length_ + "\n";

    try
      {
	if (this.peer ().send (mesg) < 0)
	  {
	    ACE.ERROR ("Error sending request");
	    return -1;
	  }
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
	return -1;
      }
    return 0;
  }

  // Send the data
  protected int sendData ()
  {
    // Get the actual data to send
    String data = this.mb_.base ().substring (this.offset_,
					      this.offset_ + this.length_);

 
    try
      {
	//	System.out.println (data);
	// Now send the data
	if (this.peer ().send (data) != this.length_)
	  {
	    ACE.ERROR ("Error sending file");
	    return -1;
	  }
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
	return -1;
      }
    this.bytesWritten_ = this.length_;
    return 0;
  }


  protected int receiveReply ()
  {
    System.out.println("Waiting for reply");

    // Receive the reply from the server
    StringBuffer reply = new StringBuffer (1024);
    
    try
      {
	if (this.peer ().recv (reply) < 0)
	  {
	    ACE.ERROR ("Error receiving reply from server");
	    return -1;
	  }
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }

    String s = reply.toString ();

    int index = -1;
    // Now parse the reply to see if it was a success or a failure
    if ((index = s.indexOf (replyPrefix_)) == -1)
      {
	ACE.ERROR ("Error receiving reply from server");
	return -1;
      }

    int codeIndex = index + replyPrefix_.length () + 1;

    // Assume code is a 3 digit number
    String codeString = s.substring (codeIndex, codeIndex + 3);

    returnCode_ = (new Integer (codeString)).intValue ();
    //    System.out.println (code);

    if (returnCode_ >= 200 && returnCode_ < 300) {  // Check if everything went smoothly
      System.out.println("We got the goodies!");
      return 0;
    } else 
      return -1;
  }

  public int returnCode ()
  {
    return this.returnCode_;
  }

  protected String authentication_ = null;
  protected String protocol_ = "http://";
  protected int bytesWritten_ = 0;
  protected MessageBlock mb_ = null;
  protected String requestPrefix_ = "PUT";
  protected String requestSuffix_ = "HTTP/1.0\n";
  protected String replyPrefix_ = "HTTP/1.0";
  protected int returnCode_;
}


