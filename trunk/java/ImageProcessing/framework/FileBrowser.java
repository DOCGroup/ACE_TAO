package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;
import gjt.Separator;

class FileNode extends Hierarchy
{
  public FileNode (String name, 
		   Icon icon, 
		   boolean root,
		   ImageApp app)
  {
    super (name, icon, root);
    this.app_ = app;
  }

  public FileNode (String name, 
		   Icon icon, 
		   ListFiles list,
		   ImageApp app)
  {
    super (name, icon);
    this.list_ = list;
    this.app_ = app;
  }

  public ImageApp app ()
  {
    return this.app_;
  }

  public void handleEvent ()
  {
    String s = null;
    String pString = "";
    Hierarchy p = this.getHierarchyParent ();

    while (p != null)
      {
	pString = p.getName () + pString;
	p = p.getHierarchyParent ();
      }

    if (pString.endsWith ("/"))
      s = "http://" +  pString + this.getName ();
    else
      s = "http://" + pString + "/" + this.getName ();

    System.out.println ("Opening: " + s);

    // If list is null, then it is a file, else it is a directory and
    // use list to get the rest of the directory.
    if (this.list_ == null)
      this.app_.openURL (s);            // It is a file
    else
      this.list_.listFiles (s, this);	// It is a directory. 
  }

  private ListFiles list_ = null;
  private ImageApp app_ = null;
}

class BrowserPanel extends Panel
{
  public BrowserPanel (ImageApp parent)
  {
    this.resize (300, 300);
    this.parent_ = parent;
    this.setLayout (new BorderLayout ());
  }

  public int initialize (String url, ListFiles list)
  {
    String directory = null;
    int index = -1;

    String pString = list.stripProtocolHeader (url);
    if (!pString.endsWith ("/"))
      pString = pString + "/";

    try
      {
	Icon dirIcon = new Icon (this.parent_.getCodeBase () + "file03.gif", (Applet) this.parent_);
	System.out.println (this.parent_.getCodeBase () + "file03.gif");
	this.root_ = new FileNode (pString,
				   dirIcon, true, this.parent_);
      }
    catch (MalformedURLException e)
      {
      }

    int count = list.listFiles (url, this.root_);
    //    System.out.println ("Count: " + count);
    if (count > 0)
      this.add ("Center", this.root_);
    return count;
  }

  private FileNode root_ = null;
  private ImageApp parent_;
}

class FileBrowser extends Frame
{
  public FileBrowser (String title, ImageApp parent)
  {
    super (title);
    this.resize (300, 300);
    this.browser_ = new BrowserPanel (parent);
    this.setLayout (new BorderLayout ());

    this.cancelButton_ = new Button (" cancel ");
    Panel buttonPanel = new Panel ();
    buttonPanel.add (this.cancelButton_);
    buttonPanel.resize (100, 100);

    Panel southPanel = new Panel ();
    southPanel.setLayout (new BorderLayout ());
    southPanel.add ("North", new Separator ());
    southPanel.add ("South", buttonPanel);
    this.add ("South", southPanel);
    this.add ("Center", this.browser_);
  }

  public int initialize (String url, ListFiles list)
  {
    return this.browser_.initialize (url, list);
  }

  // Handle window destroy events
  public boolean handleEvent (Event evt)
  {
    if (evt.id == Event.WINDOW_DESTROY)
      {
	this.dispose ();
	return true;
      }
    return super.handleEvent (evt);
  }

  // Handle all action events
  public boolean action (Event e, Object arg)
  {
    if (e.target instanceof Button)
      {
	if (e.target == this.cancelButton_)
	  {
	    this.dispose ();
	  }
	validate ();
	return true;
      }
    else
      return false;
  }

  private Button cancelButton_;;
  private BrowserPanel browser_;
}
