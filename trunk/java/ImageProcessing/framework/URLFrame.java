package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.io.*;

class URLFrame extends Frame
{
  public URLFrame (String title, ImageApp parent, boolean open)
  {
    super (title);
    // Cache information -- whether we are a load window or a save window
    this.open_ = open;

    this.parent_ = parent;
    this.resize (500,130);
    this.setLayout (new BorderLayout ());

    Panel textPanel = new Panel ();
    textPanel.setLayout (new BorderLayout ());
    textPanel.add ("North", new Label ("Image Location:"));
    textPanel.add ("Center", this.openURLText_);

    Panel buttonPanel = new Panel ();
    buttonPanel.setLayout (new FlowLayout (FlowLayout.CENTER));

    if (this.open_)
      buttonPanel.add (this.openButton_);
    else
      buttonPanel.add (this.saveButton_);

    buttonPanel.add (this.clearButton_);
    buttonPanel.add (this.cancelButton_);

    this.add ("North", textPanel);
    this.add ("South", buttonPanel);
  }

  private int browseFiles (String url)
  {
    fileBrowser_ = new FileBrowser ("Browse", this.parent_);
    ListFiles list = new ListFiles (this.fileBrowser_, this.parent_);
    return this.fileBrowser_.initialize (url, list);
  }

  // Handle all action events
  public boolean action (Event e, Object arg)
  {
    if (e.target instanceof Button)
      {
	if (e.target == this.openButton_)
	  {
	    this.getURL ();
	  }
	if (e.target == this.saveButton_)
	  {
	    this.saveFile ();
	  }
	else if (e.target == this.clearButton_)
	  {
	    this.openURLText_.setText (new String ());
	    this.openURLText_.requestFocus ();
	  }
	else if (e.target == this.cancelButton_)
	  this.dispose ();
	validate ();
	return true;
      }
    else
      return false;
  }

  public boolean keyDown (Event e, int key)
  {
    if (key == 10)
      {
	if (this.open_)
	  this.getURL ();
	else
	  this.saveFile ();
	return true;
      }
    else
      return false;
  }

  private void getURL ()
  {
    this.hide ();
    String url = this.openURLText_.getText ();
    this.dispose ();

    // The following is only for debugging
    if (url.compareTo ("ru") == 0)
      url = "http://www.cs/~pjain/gifs/";
    else if (url.compareTo ("pj") == 0)
      url = "http://www.cs/~pjain/myphoto.gif";

    if (!url.endsWith ("/") &&
	(this.parent_.openURL (url) != -1))  // Try to open it as an image
      return;
    else
      {
	ListFiles list = new ListFiles ();
	switch (this.browseFiles (url))
	  {
	  case 1:
	    this.fileBrowser_.show ();
	    break;
	  case 0:
	    DialogManager.popDialog (DialogType.MALFORMED_URL,
				     "Error: Directory contains index.html");
	    break;
	  default:
	    DialogManager.popDialog (DialogType.MALFORMED_URL,
				     "Error: Not a valid image or URL not found");
	    break;
	  }		
      }
  }

  private void saveFile ()
  {
    String url = this.openURLText_.getText ();
    this.hide ();
    this.dispose ();
    this.parent_.saveFile (url);
  }


  // Create the Open URL Frame and also the buttons which appear in
  // it
  private Button openButton_ = new Button ("Open");
  private Button saveButton_ = new Button ("Save");
  private Button clearButton_ = new Button ("Clear");
  private Button cancelButton_ = new Button ("Cancel");

  private TextField openURLText_ = new TextField (40);

  private FileBrowser fileBrowser_ = null;
  private ImageApp parent_;

  // Flag indicating if this is a load window or a save window
  private boolean open_ = true;

}

