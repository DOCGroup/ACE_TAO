// $Id$
// 
// = FILENAME
//    NewDemo.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Entry point for the demo.
//
// ============================================================================


import java.awt.*;

public class DOVEBrowser  { //extends java.applet.Applet {

  DemoCore demoCore_;
  
  public DOVEBrowser () {
    super ();
    demoCore_ = new DemoCore ();   
  }
  
  public void init () {
    demoCore_.show ();
    demoCore_.run ();
  }

  public void paint (Graphics g) {

    g.drawString ("Michael is here",10,10);
  }
  
  
  public static void main (String[] args)
    {
      DOVEBrowser doveBrowser_ = new DOVEBrowser ();
      doveBrowser_.init ();
    }  
}
  
  
