package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.applet.*;
import java.util.*;
import java.io.*;
import java.net.*;

class DialogManager
{
  public static void popDialog (int type, String message, Applet parent)
  {
    codeBase_ = parent.getCodeBase ().toString ();
    popDialog (type, message);
  }

  public static void popDialog (int type, String message)
  {
    Frame frame = null;
    
    switch (type)
      {
      case DialogType.ABOUT:
	frame = new AboutFrame ();
	break;
      case DialogType.HELP:
	frame = new HelpFrame (codeBase_);
	break;
      case DialogType.MALFORMED_URL:
      case DialogType.NOT_SUPPORTED:
      case DialogType.URL_NOT_FOUND:
	frame = new MessageFrame ("Error", message);
	break;
      case DialogType.NOT_YET_IMPLEMENTED:
	frame = new MessageFrame ("", message);
	break;
      }
    Dimension d = Toolkit.getDefaultToolkit ().getScreenSize ();
    frame.move ((d.width - frame.size ().width)/2,
		(d.height - frame.size ().height)/2);
    frame.show ();
  }
  private static String codeBase_ = "";
}

class MessageFrame extends Frame
{
  public MessageFrame (String title, String message)
  {
    super (title);

    this.resize (message.length () * 8, 100);
    this.setLayout (new BorderLayout ());

    this.text_ = new TextField (message);
    this.text_.setEditable (false);

    Panel okButtonPanel = new Panel ();
    okButtonPanel.add (this.okButton_);
    okButtonPanel.resize (100, 100);
    
    this.add ("Center", this.text_);
    this.add ("South", okButtonPanel);
  }

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
	if (e.target == this.okButton_)
	  {
	    this.dispose ();
	  }
	return true;
      }
    else
      return false;
  }

  private Button okButton_ = new Button (" ok ");
  private TextField text_ = null;
}

class AboutFrame extends Frame
{
  public AboutFrame ()
  {
    super ("About");
    this.resize (300,300);
    this.setLayout (new BorderLayout ());

    Panel okButtonPanel = new Panel ();
    okButtonPanel.add (this.okButton_);
    
    this.add ("South", okButtonPanel);
  }

  public void paint (Graphics g)
  {
    int width = this.size ().width;
    int height = this.size ().height;

    g.clearRect (0, 0, width, height);
    this.setBackground (Color.cyan);

    // First draw the title
    g.setFont (new Font ("TimesRoman", Font.BOLD | Font.ITALIC, 48));
    FontMetrics fontMetrics = g.getFontMetrics ();
    int x = (width - fontMetrics.stringWidth (title_))/2;
    int y = 100;

    Color color = g.getColor ();
    g.setColor (Color.orange);
    g.drawString(title_, x+2, y+2);
    g.setColor (color);
    g.drawString(title_, x, y);

    // Then draw author's name
    g.setFont (new Font ("TimesRoman", Font.ITALIC, 24));
    fontMetrics = g.getFontMetrics ();
    x = (width - fontMetrics.stringWidth (by_))/2;
    y += 50;
    g.drawString(by_, x, y);

    x = (width - fontMetrics.stringWidth (author_))/2;
    y += 50;
    g.drawString(author_, x, y);

    // Finally draw other information -- version number etc.
    g.setFont (new Font ("TimesRoman", Font.ITALIC, 18));
    fontMetrics = g.getFontMetrics ();
    x = (width - fontMetrics.stringWidth (info_))/2;
    y += 50;
    g.drawString(info_, x, y);

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
	if (e.target == this.okButton_)
	  {
	    this.dispose ();
	  }
	return true;
      }
    else
      return false;
  }

  private String title_ = "MedJava";
  private String by_ = "by";
  private String author_ = "Prashant Jain";
  private String info_ = "Version 1.0";
  private Button okButton_ = new Button (" ok ");
}

class HelpFrame extends Frame
{
  public HelpFrame (String codeBase)
  {
    super ("Help");
    this.setBackground (Color.cyan);
    this.text_.setEditable (false);
    Font defaultFont = new Font ("TimesRoman", Font.PLAIN, 14);
    this.text_.setFont (defaultFont);

    try
      {
	URL url = new URL (codeBase + "../ImageProcessing/framework/help.conf");
	String delim = "\n";
	
	// Get the input stream and pipe it to a DataInputStream
	DataInputStream iStream = new DataInputStream (url.openStream ());
	
	// Keep reading the data until we are done
	String tempString = iStream.readLine ();
	while (tempString != null)
	  {
	    if (tempString.startsWith ("<START>"))
	      delim = "";
	    else if (tempString.startsWith ("<END>"))
	      delim = "\n";
	    else if (tempString.startsWith ("<TAB>"))
	      this.text_.appendText ("\t");
	    else if (tempString.startsWith ("<P>"))
	      this.text_.appendText ("\n");
	    else
	      {
		this.text_.appendText (tempString);
		this.text_.appendText (delim);
	      }
	    tempString = iStream.readLine ();		
	  }
      }
    catch (MalformedURLException e)
      {
	System.err.println (e);
      }
    catch (IOException e)
      {
	System.err.println (e);
      }
    
    this.resize (600,700);
    this.setLayout (new BorderLayout ());
    
    Panel okButtonPanel = new Panel ();
    okButtonPanel.add (this.okButton_);
    this.add ("South", okButtonPanel);
    this.add ("Center", this.text_);
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
	if (e.target == this.okButton_)
	  {
	    this.dispose ();
	  }
	return true;
      }
    else
      return false;
  }

  private Vector helpInfo_ = new Vector ();
  private Button okButton_ = new Button (" ok ");
  private TextArea text_ = new TextArea ();
}

