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
  public static final String DEFAULT_TEST_SERVER = "siesta.cs.wustl.edu";
  public static final int DEFAULT_TEST_SERVER_PORT = 7787;
  public static final String DEFAULT_JAWS_SERVER = "siesta.cs.wustl.edu";
  public static final int DEFAULT_JAWS_SERVER_PORT = 5432;

  public Tester (String testInFile, 
		 ImageApp parent)
  {
    this.testInFile_ = testInFile;
    this.parent_ = parent;
  }

  public void initialize ()
  {
    System.out.println ("Initializing tester...");

    // Get the hostname of the test server
    this.server_ = this.parent_.getParameter ("testServer");
    if (this.server_ == null)
      this.server_ = DEFAULT_TEST_SERVER;

    // Get the port number of the test server
    String testPortString = this.parent_.getParameter ("testPort");
    if (testPortString == null)
      this.port_ = DEFAULT_TEST_SERVER_PORT;
    else
      this.port_ = (new Integer (testPortString)).intValue ();

    // Get the hostname of the JAWS server
    this.JAWSServer_ = this.parent_.getParameter ("JAWSServer");
    if (this.JAWSServer_ == null)
      this.JAWSServer_ = DEFAULT_JAWS_SERVER;

    // Get the port number of the JAWS server
    String JAWSPortString = this.parent_.getParameter ("JAWSPort");
    if (JAWSPortString == null)
      this.JAWSServerPort_ = DEFAULT_JAWS_SERVER_PORT;
    else
      this.JAWSServerPort_ = (new Integer (JAWSPortString)).intValue ();

    System.out.println ("Test Input File: " + this.testInFile_);
    System.out.println ("Test Server: " + this.server_ + "\tPort: " + this.port_);
    System.out.println ("JAWS Server: " + this.JAWSServer_ + "\tPort: " + this.JAWSServerPort_);

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
    TestHandler handler = new TestHandler (imageList, 
					   this.JAWSServer_, this.JAWSServerPort_,
					   this.parent_);
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
	System.out.println ("Test Server not running! Writing to standard out...");
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

  private String JAWSServer_ = "siesta.cs.wustl.edu";
  private int JAWSServerPort_ = 5432;
}

