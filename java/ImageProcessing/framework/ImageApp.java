package imaging.framework;

import java.util.*;
import java.awt.*;
import java.net.*;
import java.io.*;
import java.awt.image.*;
import java.applet.*;
import gjt.Separator;
import gjt.Util;

public class ImageApp extends Applet
{
  public void init ()
  {
    // Use BorderLayout for our applet frame
    this.setLayout (new BorderLayout ());

    // Now load all the filters specified in the config file
    //    this.loadFilters ();
    this.setupButtonPanel ();
    this.add ("Center", this.imageCanvas_);
    // Check if we are running in test mode
    String testFile = getParameter ("testFile");
    if (testFile != null)
      {
	this.tester_ = new Tester (testFile, this);
	this.tester_.initialize ();
      }    
  }

  private void setupButtonPanel ()
  {
    Panel southPanel = new Panel ();
    southPanel.setLayout (new BorderLayout ());
    
    Panel buttonPanel = new Panel ();
    buttonPanel.setLayout (new GridLayout (1, 5));

    this.statusDisplay_ = new StatusDisplay ();
    // Create a panel for all the buttons
    this.filePanel_ = new FilePanel (this);
    this.resetPanel_ = new ResetPanel (this);
    this.zoomPanel_ = new ZoomPanel (this);
    this.filterPanel_ = new FilterPanel (this);
    this.helpPanel_ = new HelpPanel (this);

    buttonPanel.add (this.filePanel_);
    buttonPanel.add (this.resetPanel_);
    buttonPanel.add (this.zoomPanel_);
    buttonPanel.add (this.filterPanel_);
    buttonPanel.add (this.helpPanel_);

    southPanel.add ("North", new Separator ());
    southPanel.add ("Center", buttonPanel);
    southPanel.add ("South", this.statusDisplay_);

    southPanel.resize (400, 400);
  
    // Now add all these components to the main frame
    this.add ("South", southPanel);
    this.add ("North", new Panel ()); // Empty panel (for aesthetics)
    //    this.add ("East", new Panel ()); // Empty panel (for aesthetics)
    //    this.add ("West", new Panel ()); // Empty panel (for aesthetics)
  }

  public void displayStatus (String s)
  {
    this.statusDisplay_.setText (s);
  }

  // Handle all action events
  public void zoomFactor (double zoomFactor)
  {
    this.imageCanvas_.zoomFactor (zoomFactor);
  }

  public void reloadFilters ()
  {
    this.filterPanel_.loadFilters ();
    repaint ();
  }
  
  public Hashtable filterTable ()
  {
    return this.filterTable_;
  }

  public ImageFilter getFilter (String s)
  {
    return (ImageFilter) this.filterTable_.get (s);
  }

  public void apply ()
  {
    ImageFilter filter = this.getFilter (this.filterPanel_.choice ().getSelectedItem ());
    this.apply (filter);
  }

  public void apply (ImageFilter filter)
  {
    if (filter != null)
      {
	Util.getFrame (this).setCursor (Frame.WAIT_CURSOR);
	this.imageCanvas_.applyFilter (filter);
	Util.getFrame (this).setCursor (Frame.DEFAULT_CURSOR);
      }
  }

  public void resetImage ()
  {
    this.imageCanvas_.applyFilter (null);
  }

  public int openURL (String url)
  {
    if (url == null)
      return -1;

    Image image = null;
    try
      {
	image = getImage (new URL (url));
      }
    catch (MalformedURLException e)
      {
	return -1;
      }

    if (image != null)
      {
	// Check if the image was actually loaded. Note that we have
	// to wait for the potential image to finish loading before we
	// know if it is a valid image.
	if (this.imageCanvas_.setImage (image) == -1)
	  return -1;
	else
	  this.filePanel_.enableSaveButton ();
      }
    else
      return -1;
    return 0;
  }

  public void saveFile (String url)
  {
    ImageSender imageSender = new ImageSender (this);
    imageSender.open (this.imageCanvas_.getImage (), url);
    int bytesSent = imageSender.send ();
    if (bytesSent == -1)
      DialogManager.popDialog (DialogType.NOT_SUPPORTED,
			       "Server does not support uploading or URL not found");
  }

  public Choice getFilters () 
  {
    Choice choice = new Choice ();

    // Add the default choice first
    choice.addItem ("Filters:");

    // Now do the file processing -- to determine which filters need
    // to be loaded.

    // Check if the filename has been previously specified and
    // if not then check if the user has specified the name of the
    // config file
    if (this.configFile_ == null)
      this.configFile_ = getParameter ("configFile");
    if (this.configFile_ == null)
      this.configFile_ = "http://www.cs.wustl.edu/~pjain/java/ACE_wrappers/java/ImageProcessing/framework/filter.conf";

    URL url;
    String configInfo= null;
    try
      {
	System.out.println ("Configuration File: " + this.configFile_);
	// Create a new URL
	url = new URL (this.configFile_);

	// Get the input stream and pipe it to a DataInputStream
	DataInputStream iStream = new DataInputStream (url.openStream ());

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
	configInfo = tempBuf.toString ();
      }
    catch (MalformedURLException e)
      {
	System.err.println (e);
      }
    catch (IOException e)
      {
	System.err.println (e);
      }

    if (configInfo != null)
      {
	try
	  {
	    StringTokenizer tokens = new StringTokenizer (configInfo);
	    String fullFilterName = null;
	    String filterName = null;
	    // Now parse the string, picking up filter names. Use these
	    // names to load the actual filters as well add new choices to
	    // the filter choices.
	    while (tokens.hasMoreTokens ())
	      {
		// Get the next token
		fullFilterName = tokens.nextToken ();
		filterName = this.extractFilterName (fullFilterName);
		
		System.out.println ("Loading: " + fullFilterName);
		// Load the filter class
		Class c = Class.forName (fullFilterName);
		  //		Class c = this.filterRepository_.load (filter);
		
		// Add the filter to the Filter Repository
		this.filterTable_.put (filterName, 
				       (ImageFilter) c.newInstance ());
	    
		// Add filter name to the list of filter choices
		choice.addItem (filterName);
	      }
	  }
	catch (ClassNotFoundException e)
	  {
	    System.err.println ("Filter not found: " + e);
	    return null;
	  }
	catch (IllegalAccessException e)
	  {
	    System.err.println ("Filter not found: " + e);
	    return null;
	  }
	catch (InstantiationException e)
	  {
	    System.err.println ("Filter not found: " + e);
	    return null;
	  }
      }
    return choice;
  }

  // Extract the short filter name from the full filter name. For
  // example, this method returns "EmbossFilter" if it is given the
  // string "imaging/filters/EmbossFilter" 
  private String extractFilterName (String s)
  {
    String filterName = null;
    StringTokenizer tokens = new StringTokenizer (s, ".");
    while (tokens.hasMoreTokens ())
      filterName = tokens.nextToken ();
    return filterName;
  }

  private Panel centerPanel_ = new Panel ();
  private String configFile_ = null;
  private Choice choice_ = null;
  private ImageCanvas imageCanvas_ = new ImageCanvas ();
  private FilePanel filePanel_;
  private ResetPanel resetPanel_;
  private ZoomPanel zoomPanel_;
  private FilterPanel filterPanel_;
  private HelpPanel helpPanel_;

  private StatusDisplay statusDisplay_;
  private Hashtable filterTable_ = new Hashtable ();
  private Tester tester_;
  
}
