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
  public TestHandler (String imageList, ImageApp parent)
  {
    this.imageList_ = imageList;
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

  private void process (String image)
  {
    try
      {
	if (stdOut)
	  System.out.println ("Image: " + image);
	else
	  this.peer ().send ("Image: " + image + "\n");
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }

    this.loadImage (image);
    this.processImage (image);
    this.uploadImage (image);
  }

  private void loadImage (String image)
  {
    try
      {
	if (stdOut)
	  System.out.print ("\tLoading...");
	else
	  this.peer ().send ("\tLoading...");

	// Start the timer
	timer_.start ();

	// Load the image
	parent_.openURL (image);

	// Stop the timer
	timer_.stop ();
	long time = timer_.elapsedTime ();
	if (stdOut)
	  System.out.println ("done (" + ((double) time)/1000 + " seconds).");
	else
	  this.peer ().send ("done (" + ((double) time)/1000 + " seconds).\n");
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }

  }

  private void processImage (String image)
  {
    try
      {
	if (stdOut)
	  System.out.println ("\tProcessing...");
	else
	  this.peer ().send ("\tProcessing...\n");

	for (Enumeration e = filterTable_.keys (); e.hasMoreElements (); )
	  {
	    String filterName = (String) e.nextElement ();
	    if (stdOut)
	      System.out.print ("\t\t" + filterName + "...");
	    else
	      this.peer ().send ("\t\t" + filterName + "...");

	    ImageFilter filter = (ImageFilter) filterTable_.get (filterName);

	    // Start the timer
	    timer_.start ();

	    this.parent_.apply (filter);

	    // Stop the timer
	    timer_.stop ();
	    long time = timer_.elapsedTime ();

	    if (stdOut)
	      System.out.println ("done (" + ((double) time)/1000 + " seconds).");
	    else
	      this.peer ().send ("done (" + ((double) time)/1000 + " seconds).\n");
	    this.parent_.resetImage ();
	  }
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
  }
   
  private void uploadImage (String image)
  {

  }

  private ImageApp parent_ = null;
  private ProfileTimer timer_ = new ProfileTimer ();
  private String imageList_ = null;
  private boolean stdOut = true;
  private Hashtable filterTable_ = null;

}
