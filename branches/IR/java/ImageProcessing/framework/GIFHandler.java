package imaging.framework;

import java.awt.*;
import java.io.*;
import JACE.OS.*;
import JACE.Connection.*;


public class GIFHandler extends BlobWriter
{
  public GIFHandler (String filename, Image image, int length)
  {
    super (null, length, 0, filename);
    this.image_ = image;
  }

  /******************************
   * Used for HTTP 1.1 PUT authorization
   *****************************/
  public GIFHandler (String filename, Image image, int length, String authentication)
    {
      super (null, length, 0, filename, authentication);
      this.image_ = image;
    }
  
  protected int sendRequest ()
  {
    if (this.sendHeader () == -1)
      {
	ACE.ERROR ("GIFHandler::sendHeader failed.");
	return -1;
      }
    else
      if (this.sendData () == -1)
	{
	  ACE.ERROR ("GIFHandler::sendData failed.");
	  return -1;
	}
    return 0;
  }

  // Send the data
  protected int sendData ()
  {


    try
      {
	if (this.length_ > 0)
	  {
	    OutputStream ostream = this.peer ().outputStream ();

	    this.encoder_ = new GifEncoder (this.image_, ostream);

	    this.encoder_.encode ();

	    ostream.flush();

	    //	this.encoder_ = new GIFEncoder (this.image_);
	    //	this.encoder_.Write (ostream);
	  }
	this.bytesWritten_ = this.length_;
	  
      }
    catch (IOException e)
      {
	ACE.ERROR ("Error writing to server");
      }
    return 0;
  }

  GifEncoder encoder_ = null;
  //  GIFEncoder encoder_ = null;
  Image image_ = null;
}
