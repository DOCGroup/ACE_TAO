package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.io.*;
import gjt.ComponentScroller;

class URLFrame extends Frame
{
  public URLFrame (String title, ImageApp parent)
  {
    super (title);

    this.parent_ = parent;
    this.resize (500,130);
    this.setLayout (new BorderLayout ());

    Panel textPanel = new Panel ();
    textPanel.setLayout (new BorderLayout ());
    textPanel.add ("North", new Label ("Image Location:"));
    textPanel.add ("Center", this.openURLText_);

    Panel buttonPanel = new Panel ();
    buttonPanel.setLayout (new FlowLayout (FlowLayout.CENTER));
    buttonPanel.add (this.browseButton_);
    buttonPanel.add (this.loadButton_);
    buttonPanel.add (this.clearButton_);
    buttonPanel.add (this.cancelButton_);

    this.add ("North", textPanel);
    this.add ("South", buttonPanel);
    this.loadButton_.nextFocus ();
  }

  private int browseFiles ()
  {
    String pString = this.openURLText_.getText ();
    if (pString.compareTo ("ru") == 0)
      pString = "http://www.cs/~pjain/java";

    fileBrowser_ = new FileBrowser ("Browse", this.parent_);
    //    this.scroller_ = new ComponentScroller (fileBrowser_);
    ListFiles list = new ListFiles (this.fileBrowser_, this.parent_);
    return this.fileBrowser_.initialize (pString, list);
  }

  // Handle all action events
  public boolean action (Event e, Object arg)
  {
    if (e.target instanceof Button)
      {
	if (e.target == this.loadButton_)
	  {
	    this.parent_.openURL (this.openURLText_.getText ());
	    this.dispose ();
	  }
	else if (e.target == this.clearButton_)
	  {
	    this.openURLText_.setText (new String ());
	    this.openURLText_.requestFocus ();
	  }
	else if (e.target == this.cancelButton_)
	  this.dispose ();
	else if (e.target == this.browseButton_)
	  {
	    this.dispose ();
	    ListFiles list = new ListFiles ();
	    switch (this.browseFiles ())
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
					 "Error: Malformed URL");
		break;
	      }
	  }

	validate ();
	return true;
      }
    else
      return false;
  }

  // Create the Open URL Frame and also the buttons which appear in
  // it
  private Button browseButton_ = new Button ("Browse");
  private Button loadButton_ = new Button ("Load");
  private Button clearButton_ = new Button ("Clear");
  private Button cancelButton_ = new Button ("Cancel");

  private TextField openURLText_ = new TextField (40);

  private FileBrowser fileBrowser_ = null;
  private ImageApp parent_;
  private ComponentScroller scroller_;

}

