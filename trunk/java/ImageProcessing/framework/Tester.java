package imaging.framework;

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.image.*;
import java.util.*;
import JACE.Connection.*;
import JACE.OS.*;

public class Tester implements Runnable
{
  public Tester (String testInFile, String server, int port, ImageApp parent)
  {
    this.testInFile_ = testInFile;
    this.server_ = server;
    this.port_ = port;
    this.parent_ = parent;

    // Run in your own thread of control
    (new Thread (this)).start ();
  }

  public void run ()
  {
    this.setupConnection (this.getImages ());
  }

  private String getImages ()
  {
    URL inputURL;
    String imageList = null;
    try
      {
	System.out.println ("Test Input File: " + this.testInFile_);
	System.out.println ("Server: " + this.server_ + "\tPort: " + this.port_);

	// Create input URL
	inputURL = new URL (this.testInFile_);

	// Get the input stream and pipe it to a DataInputStream
	DataInputStream iStream = new DataInputStream (inputURL.openStream ());

	// Create a buffer to hold all the data we get
	StringBuffer tempBuf = new StringBuffer ();
	// Keep reading the data until we are done
	String tempString = iStream.readLine ();
	while (tempString != null)
	  {
	    tempBuf.append (tempString);
	    tempBuf.append (" ");
	    tempString = iStream.readLine ();
	  }
	imageList = tempBuf.toString ();
      }
    catch (MalformedURLException e)
      {
	ACE.ERROR (e);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
    return imageList;
  }

  private void setupConnection (String imageList)
  {
    // First try to connect to the server. If the server is not
    // running then we will write to standard output.

    // Create a handler which will handle our connection.
    TestHandler handler = new TestHandler (imageList, this.parent_);

    try
      {
	Connector connector = new Connector ();
	connector.open (server_, port_);
	connector.connect (handler);
      }
    catch (UnknownHostException e)
      {
	ACE.ERROR (e);
      }
    catch (SocketException e)
      {
	// The server is not running so write all the output to screen
	handler.doTesting ();
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
  }

  private String testInFile_ = null;
  private ImageApp parent_ = null;
  private String server_ = "siesta.cs.wustl.edu";
  private int port_ = 7787;
}

