// $Id$
// 
// = FILENAME
//   DOVEBrowser.java
//
// = AUTHOR
//   Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Entry point for the demo.
//
// ============================================================================


import java.awt.*;

public class DOVEBrowser {

  DemoCore demoCore_;
  
  public DOVEBrowser () {
    super (); 
  }
  
  public void init (String nameServiceIOR, String[] args) {

    demoCore_ = new DemoCore (nameServiceIOR, args, null); 
    demoCore_.show ();
    demoCore_.run ();
  }
  
  public static void main (String[] args) {      
    String nameServiceIOR = null;

    if (args.length == 2) {
      if (args[0] == "-ORBnameserviceior") {
	nameServiceIOR = args[1];
      }
    } 
    
    DOVEBrowser doveBrowser = new DOVEBrowser();
    doveBrowser.init (nameServiceIOR, args);
  }  
}
  
  

