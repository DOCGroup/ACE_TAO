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

public class DOVEBrowser  extends java.applet.Applet {

  DemoCore demoCore_;
  static String nameServiceIOR_ = null;
  
  public DOVEBrowser () {
    super (); 
  }
  
  public void init () {
	// in case of an applet use the following line, else not!
	nameServiceIOR_ = this.getParameter ("NSIOR");

	if (nameServiceIOR_ == null) {
		System.out.println ("No NameSerivce IOR: Will use Name Service Lookup Protokol.");
	}

    demoCore_ = new DemoCore (nameServiceIOR_, this);   
    demoCore_.show ();
    demoCore_.run ();
  }

  public void paint (Graphics g) {

    g.drawString ("Michael is here",10,10);
  }
  
  
  public static void main (String[] args)
    {      
      if (args[0] == "-ORBnameserviceior") {
		nameServiceIOR_ = args[1];
      }
      
      DOVEBrowser doveBrowser = new DOVEBrowser();
      doveBrowser.init ();
    }  
}
  
  
