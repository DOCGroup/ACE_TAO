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
  
  public DOVEBrowser (String nameServiceIOR) {
    super ();
    demoCore_ = new DemoCore (nameServiceIOR);   
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
      String nameServiceIOR = null;
      if (args[0] == "-ORBnameserviceior") {
	nameServiceIOR = args[1];
      }
      
      DOVEBrowser doveBrowser = new DOVEBrowser (nameServiceIOR);
      doveBrowser.init ();
    }  
}
  
  
