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

  public void init (String nameServiceIOR, String nameServicePort,
                    String[] args, boolean use_queueing) {

    demoCore_ = new DemoCore (nameServiceIOR,
                              nameServicePort,
                              args,
                              use_queueing,
                              null);
    demoCore_.show ();
    demoCore_.run ();
  }

  public static void main (String[] args) {
    String nameServiceIOR = null;
    String nameServicePort = null;
    boolean use_queueing = false;
    int arg_index = 0;

    // Loop through command line arguments, acting on relevant options
    while (args.length > arg_index)
      {
        System.out.println ("args.length [" + args.length + "] arg_index [" +
                            arg_index + "]");
        // Set the name service IOR
        if (args[arg_index].equals ("-queue"))
          {
            ++arg_index;
            use_queueing = true;
          }
        else if ((args[arg_index].equals ("-nameserviceior")) &&
            (args.length > arg_index + 1))
          {
            System.out.println ("switch [" + args[arg_index] + "]");
            System.out.println ("value [" + args[arg_index + 1] + "]");
	    nameServiceIOR = args[arg_index + 1];
            System.out.println ("nameServiceIOR [" + nameServiceIOR + "]");
            arg_index += 2;
          }
        // Set the name service port
        else if ((args[arg_index].equals ("-nameserviceport")) &&
                 (args.length > arg_index + 1))
          {
            System.out.println ("switch [" + args[arg_index] + "]");
            System.out.println ("value [" + args[arg_index + 1] + "]");
	    nameServicePort = args[arg_index + 1];
            System.out.println ("nameServicePort [" + nameServicePort + "]");
            arg_index += 2;
          }
        // Skip over anything else
        else
          {
           System.out.println ("Skipping [" + args[arg_index] + "]");
            arg_index ++;
          }
      }

    DOVEBrowser doveBrowser = new DOVEBrowser();
    doveBrowser.init (nameServiceIOR, nameServicePort, args, use_queueing);
  }
}



