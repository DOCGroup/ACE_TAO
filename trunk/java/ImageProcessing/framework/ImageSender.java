package imaging.framework;

import java.awt.*;
import java.net.*;
import java.io.*;
import JACE.OS.*;
import JACE.Connection.*;

public class ImageSender 
{
  public void open (Image image, String url)
  {
    this.image_ = image;

    this.hostname_ = url;
    if (this.hostname_.startsWith ("http://"))
      this.hostname_ = this.hostname_.substring (7);

    int index = -1;
    if ((index = this.hostname_.indexOf (":")) != -1)
      {
	String temp = this.hostname_.substring (index + 1);
	int i = -1;
	if ((i = temp.indexOf ("/")) != -1)
	  {
	    this.port_ = (new Integer (temp.substring (0, i))).intValue ();
	    this.filename_ = temp.substring (i);
	  }
	this.hostname_ = this.hostname_.substring (0, index);
      }
    else
      {
	int i = -1;
	if ((i = this.hostname_.indexOf ("/")) != -1)
	  {
	    this.filename_ = hostname_.substring (i);
	  }
	this.hostname_ = this.hostname_.substring (0, i);
	this.port_ = 80;
      }
  }

  public int send ()
  {
    int length = 0;
    try
      {
	GIFOutputStream ostream = new GIFOutputStream ();
	GifEncoder encoder = new GifEncoder (this.image_, ostream);
	encoder.encode ();
	//	this.encoder_.Write (ostream);

	//	this.encoder_ = new GIFEncoder (this.image_);
	//	this.encoder_.Write (ostream);
	//	StringBuffer buf = ostream.data ();
	length = ostream.count ();
	System.out.println ("send: " + length);
      }
    catch (IOException e)
      {
	ACE.ERROR ("Exception generating gif");
      }

    GIFHandler gifHandler = new GIFHandler (this.filename_, this.image_, length);
    try
      {
	// Connect to the server and send the image
	this.connector_.open (this.hostname_, this.port_);
	this.connector_.connect (gifHandler);
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

    if (gifHandler.returnCode () != -1)
      return gifHandler.bytesWritten ();
    else
      return -1;
  }

  private Image image_ = null;
  String filename_;
  String hostname_;
  int port_;
  Connector connector_ = new Connector ();
}

