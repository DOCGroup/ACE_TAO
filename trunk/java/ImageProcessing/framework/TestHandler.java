package imaging.framework;

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.image.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Timers.*;

public class TestHandler extends SvcHandler
{
  public TestHandler (String imageList,
		      String JAWSServer, int JAWSPort,
		      ImageApp parent)
  {
    this.imageList_ = imageList;
    this.JAWSServer_ = JAWSServer;
    this.JAWSPort_ = JAWSPort;
    this.parent_ = parent;
    this.filterTable_ = this.parent_.filterTable();
  }

  public int open (Object obj)
  {
    // We got called by the Connector so assume connection was set up
    // fine and therfore do not use standard output
    stdOut = false;

    doTesting ();
    return 0;
  }

  public void doTesting ()
  {
    if (imageList_ != null)
      {
	StringTokenizer tokens = new StringTokenizer (imageList_);
	String image = null;
	
	// Now parse the string, picking up image names. 
	while (tokens.hasMoreTokens ())
	  {
	    // Get the next token
	    image = tokens.nextToken ();
	    this.process (image);
	  }
      }
  }

  private void write (String data)
  {
    try
      {
	// If we are connected to the server then send the data to the
	// server, otherwise write it to standard out.
	if (stdOut)
	  System.out.print (data);
	else
	  this.peer ().send (data);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
  }

  private void process (String image)
  {
    this.write ("Image: " + image + "\n");

    this.loadImage (image);
    this.processImage (image);
    this.uploadImage (image);
  }

  private void loadImage (String image)
  {
    this.write ("\tLoading...");
    
    // Start the timer
    timer_.start ();
    
    // Load the image
    parent_.openURL (image);
    
    // Stop the timer
    timer_.stop ();
    long time = timer_.elapsedTime ();

    this.write ("done (" + ((double) time)/1000 + " seconds).\n");
  }

  private void processImage (String image)
  {
    this.write ("\tProcessing...\n");

    for (Enumeration e = filterTable_.keys (); e.hasMoreElements (); )
      {
	String filterName = (String) e.nextElement ();

	this.write ("\t\t" + filterName + "...");
	
	ImageFilter filter = (ImageFilter) filterTable_.get (filterName);
	    
	// Start the timer
	timer_.start ();
	
	this.parent_.apply (filter);
	
	// Stop the timer
	timer_.stop ();
	long time = timer_.elapsedTime ();
	
	this.write ("done (" + ((double) time)/1000 + " seconds).\n");
	
	this.parent_.resetImage ();
      }
  }
   
  private void uploadImage (String image)
  {
    int index = image.lastIndexOf ("/");
    String imageName = image.substring (index+1);
    String url = "http://" + this.JAWSServer_ + ":" + this.JAWSPort_ + "/" + imageName;
    this.write ("\tUploading " + url + "...");

    // Start the timer
    timer_.start ();

    this.parent_.saveFile (url);

    // Stop the timer
    timer_.stop ();
    long time = timer_.elapsedTime ();

    this.write ("done (" + ((double) time)/1000 + " seconds).\n");		       
  }

  private ImageApp parent_ = null;
  private ProfileTimer timer_ = new ProfileTimer ();
  private String imageList_ = null;
  private boolean stdOut = true;
  private Hashtable filterTable_ = null;
  private String JAWSServer_ = null;
  private int JAWSPort_ = 5432;
}
