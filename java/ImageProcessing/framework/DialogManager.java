package imaging.framework;

import java.awt.*;
import java.awt.image.*;

class DialogManager
{
  public static void popDialog (int type, String message)
  {
    Frame frame = null;
    
    switch (type)
      {
      case DialogType.ABOUT:
	frame = new AboutFrame ();
	break;
      case DialogType.MALFORMED_URL:
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
    /*    frame.reshape ((d.width - frame.size ().width)/2,
		   (d.height - frame.size ().height)/2,
		   frame.size ().width,
		   frame.size ());
		   */
    frame.show ();
  }
}

class MessageFrame extends Frame
{
  public MessageFrame (String title, String message)
  {
    super (title);

    this.resize (250,100);
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
    this.setText ();
    
    this.resize (500,700);
    this.setLayout (new BorderLayout ());

    Panel okButtonPanel = new Panel ();
    okButtonPanel.add (this.okButton_);
    
    this.add ("South", okButtonPanel);
  }

  public void paint (Graphics g)
  {
    g.clearRect (0, 0, this.size ().width, this.size ().height);
    g.setFont (new Font ("TimesRoman", Font.PLAIN, 18));
    this.setBackground (Color.white);
    int x = 20;
    int y = 100;

    for (int i = 0; i < AboutFrame.MAXROWS; i++)
      {
	g.drawString(this.text_[i], x, y);
	y += g.getFont ().getSize () + 5;
      }
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

  private void setText ()
  {
    text_[0] = "This is a prototype of a large scale distributed medical";
    text_[1] = "imaging system.  It has been developed using Java, in";
    text_[2] = "particular, several components of Java ACE.";
    text_[3] = "";
    text_[4] = "The prototype allows images to be downloaded across the";
    text_[5] = "network.  It then provides several filters that can be";
    text_[6] = "used to do image processing.  The image filters are";
    text_[7] = "configured into the system dynamically using the Service";
    text_[8] = "Configurator pattern.  In the current implementation, the";
    text_[9] = "filters are specified via a configuration file located at";
    text_[10] = "server.  The file can be modified at runtime to add";
    text_[11] = "additional filters or to remove some filters.  This allows";
    text_[12] = "filters to be configured and reconfigured dynamically.";
    text_[13] = "";
    text_[14] = "Currently, images can not be uploaded.  This is mainly due";
    text_[15] = "to security restrictions imposed by current servers. Our";
    text_[16] = "goal is to use the prototpe in conjunction with JAWS, an";
    text_[17] = "adaptive web server we are currently developing in which we";
    text_[18] = "plan to provide support for image uploading.";
    text_[19] = "";
    text_[20] = "For more information about this prototype, please contact";
    text_[21] = "Prashant Jain (pjain@cs.wustl.edu).";
  }

  private final static int MAXROWS = 22;
  private Button okButton_ = new Button (" ok ");
  private TextArea textArea_ = null;
  private String [] text_ = new String [AboutFrame.MAXROWS];
}

