package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.io.*;

/******************************************************
 * Future idea:
 *
 * Create a smart button class with an invoke method that's
 * to be called when the button is pressed.
 *
 * Subclasses override the invoke method to define 
 * specific behavior.
 *
 * That would allow things like load and save frames
 * to be almost exactly the same -- one could inherit
 * from the other and override addButtons, etc
 *****************************************************/


class SaveURLFrame extends Frame
{
  public SaveURLFrame (String title, ImageApp parent)
  {
    super (title);

    this.parent_ = parent;
    this.resize (500,200);
 
    this.addButtons();
  }

  private void addButtons()
  {
    this.setLayout (new BorderLayout ());

    Panel textPanels = new Panel();
    textPanels.setLayout(new BorderLayout());

    Panel URLPanel = new Panel ();
    URLPanel.setLayout (new BorderLayout ());
    URLPanel.add ("North", new Label ("Image Location:"));
    URLPanel.add ("Center", this.openURLText_);

    // Password panel
    Panel pwPanel = new Panel();
    pwPanel.setLayout (new BorderLayout ());
    pwPanel.add ("North", new Label ("Authentication:"));
    pwPanel.add ("Center", this.passwordText_);

    textPanels.add("North", URLPanel);
    textPanels.add("South", pwPanel);

    Panel buttonPanel = new Panel ();
    buttonPanel.setLayout (new FlowLayout (FlowLayout.CENTER));

    buttonPanel.add (this.saveButton_);
    buttonPanel.add (this.clearButton_);
    buttonPanel.add (this.cancelButton_);

    this.add ("North", textPanels);
    this.add ("South", buttonPanel);
  }

  // Handle all action events
  public boolean action (Event e, Object arg)
  {
    if (e.target instanceof Button)
      {
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
	this.saveFile ();
	return true;
      }
    else
      return false;
  }

  private void saveFile ()
  {
    String url = this.openURLText_.getText ();
    String password = this.passwordText_.getText();
    this.hide ();
    this.dispose ();

    // The following is only for debugging
    if (url.startsWith ("#"))
      url = "http://merengue.cs:5432/" + url.substring (1);

    if ((password != null) && (password.length() > 0))
      this.parent_.saveFile (url, password);
    else
      this.parent_.saveFile (url);
  }


  // Create the Open URL Frame and also the buttons which appear in
  // it
  private Button saveButton_ = new Button ("Save");
  private Button clearButton_ = new Button ("Clear");
  private Button cancelButton_ = new Button ("Cancel");

  private TextField openURLText_ = new TextField (40);
  private TextField passwordText_ = new TextField (40);

  private ImageApp parent_;

  // Flag indicating if this is a load window or a save window
  private boolean open_ = true;

}
