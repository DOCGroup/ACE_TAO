package imaging.framework;

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.image.*;
import java.util.*;
import JACE.Timers.*;

public class Tester implements Runnable
{
  public Tester (String testInFile, String testOutFile, ImageApp parent)
  {
    this.testInFile_ = testInFile;
    this.testOutFile_ = testOutFile;
    this.parent_ = parent;
    this.filterTable_ = this.parent_.filterTable();

    // Run in your own thread of control
    (new Thread (this)).start ();
  }

  public void run ()
  {
    URL inputURL;
    URL outputURL;
    String imageList= null;
    try
      {
	System.out.println ("Test Input File: " + this.testInFile_);
	System.out.println ("Test Output File: " + this.testOutFile_);

	// Create URLs
	inputURL = new URL (this.testInFile_);
	outputURL = new URL (this.testOutFile_);
	URLConnection connection = outputURL.openConnection ();

	// Get the input stream and pipe it to a DataInputStream
	DataInputStream iStream = new DataInputStream (inputURL.openStream ());

	// Get the output stream and pipe it to a PrintStream
	oStream_ = new PrintStream (connection.getOutputStream ());

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
	System.err.println (e);
      }
    catch (IOException e)
      {
	System.err.println (e);
      }

    if (imageList != null)
      {
	StringTokenizer tokens = new StringTokenizer (imageList);
	String image = null;
	
	// Now parse the string, picking up image names. 
	while (tokens.hasMoreTokens ())
	  {
	    // Get the next token
	    image = tokens.nextToken ();
	    this.doTesting (image);
	  }
      }
  }

  private void doTesting (String image)
  {
    if (oStream_ == null)
      System.out.println ("Image: " + image);
    else
      oStream_.println ("Image: " + image);

    this.loadImage (image);
    this.processImage (image);
    this.uploadImage (image);
  }

  private void loadImage (String image)
  {
    if (oStream_ == null)
      System.out.print ("\tLoading...");
    else
      oStream_.print ("\tLoading...");

    // Start the timer
    timer_.start ();

    // Load the image
    parent_.openURL (image);

    // Stop the timer
    timer_.stop ();
    long time = timer_.elapsedTime ();
    if (oStream_ == null)
      System.out.println ("done (" + ((double) time)/1000 + " seconds).");
    else
      oStream_.println ("done (" + ((double) time)/1000 + " seconds).");
  }

  private void processImage (String image)
  {
    if (oStream_ == null)
      System.out.println ("\tProcessing...");
    else
      oStream_.println ("\tProcessing...");

    for (Enumeration e = filterTable_.keys (); e.hasMoreElements (); )
      {
	String filterName = (String) e.nextElement ();
	if (oStream_ == null)
	  System.out.print ("\t\t" + filterName + "...");
	else
	  oStream_.print ("\t\t" + filterName + "...");

	ImageFilter filter = (ImageFilter) filterTable_.get (filterName);

	// Start the timer
	timer_.start ();

	this.parent_.apply (filter);

	// Stop the timer
	timer_.stop ();
	long time = timer_.elapsedTime ();

	if (oStream_ == null)
	  System.out.println ("done (" + ((double) time)/1000 + " seconds).");
	else
	  oStream_.println ("done (" + ((double) time)/1000 + " seconds).");
	this.parent_.resetImage ();
      }
  }
   
  private void uploadImage (String image)
  {

  }

  private String testInFile_ = null;
  private String testOutFile_ = null;
  private ImageApp parent_ = null;
  private ProfileTimer timer_ = new ProfileTimer ();
  private Hashtable filterTable_ = null;
  PrintStream oStream_ = null;
}
