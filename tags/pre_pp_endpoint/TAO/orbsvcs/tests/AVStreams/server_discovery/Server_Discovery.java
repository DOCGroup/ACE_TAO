// ========================================================================
// $Id$
// 
// = FILENAME
//    Server_Discovery.java
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import com.sun.java.swing.*;
import Server_Discovery_Util;
import Server_Discovery_Selection;


public class Server_Discovery extends JFrame
{
  Thread thread;
  private static final Integer DOCLAYER = new Integer(5);
  private static final Integer TOOLLAYER = new Integer(6);
  private static final Integer HELPLAYER = new Integer(7);

  private JMenuBar menu_bar_;
  private JDesktopPane desktop_;
  private Server_Discovery_Selection movies_selector_=  new Server_Discovery_Selection (this);
  // private User_Interface user_interface = new User_Interface();
  
  public Server_Discovery ()
    {
      super ("Server Discovery");
      
      final int inset = 50;
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      this.setBounds (inset, inset, screenSize.width - inset*2, screenSize.height - inset*2);     

      // Ok, as grotesque as these anonymous classes are, they do have
      // their uses.
      WindowAdapter window_adapter = new WindowAdapter ()
        {
          
          public void windowClosing (WindowEvent e)
            {
              Server_Discovery_Perf doc = (Server_Discovery_Perf)e.getSource();
              Server_Discovery sd = (Server_Discovery)doc.getParent();
              //sd.thread.stop();
              doc.continue_ = false;
              System.out.println("Within Window Close");
              System.exit (0);
            }
        };

      System.out.println("It reachead here");
      this.addWindowListener (window_adapter);
      this.desktop_ = new JDesktopPane ();
      this.getContentPane ().add (this.desktop_);

      //this.desktop_.add (this.user_interface  , DOCLAYER);
      this.desktop_.add (this.movies_selector_, DOCLAYER);
      try
        {
          System.out.println ("Compiling movies...");
          this.compile_movies ();
          this.movies_selector_.setSelected (true); 
        }
      catch (java.beans.PropertyVetoException e2) {}
    }

  // Mediator callback methods. 
  public void display_performance (String movie_name, String host_name)
    {
      Server_Discovery_Perf doc = new Server_Discovery_Perf (host_name);

      doc.add_dynamic_property ("CPU");
      doc.add_dynamic_property ("Disk");
      doc.add_dynamic_property ("Pages");
      doc.add_dynamic_property ("Swaps");
      doc.add_dynamic_property ("Packets");
      doc.add_dynamic_property ("errors");
      doc.add_dynamic_property ("switches");
      doc.add_dynamic_property ("collisions");
      doc.add_dynamic_property ("interrupts");
      doc.add_dynamic_property ("load");

      this.desktop_.add (doc, DOCLAYER);      

      try
        { 
          doc.setSelected (true); 
          thread = new Thread(doc);
          thread.start();
        }
      catch (java.beans.PropertyVetoException e2) {}
    }

  public void load_page (String url)
    {
      Server_Discovery_Browser doc = new Server_Discovery_Browser (url);
      this.desktop_.add (doc, DOCLAYER);      

      try
        { 
          doc.setSelected (true); 
        }
      catch (java.beans.PropertyVetoException e2) {}

    }

  private void compile_movies ()
    {
      // Add the movies to the list.
      System.out.println ("Getting a list of servers.");
      String[] host_names = Server_Discovery_Util.get_servers ();

      System.out.println ("Number of server names returned: " + host_names.length);
      for (int i = 0; i < host_names.length; i++)
        {
          System.out.println ("Getting movies for server " + host_names[i]);
          TAO_VR.Movie[] movies = Server_Discovery_Util.get_movie_info (host_names[i]);

          System.out.println ("Adding movies for server " + host_names[i]);
          for (int j = 0; j < movies.length; j++)
            {
              System.out.println ("Adding movie: " + movies[j].name_);
              this.movies_selector_.add_movie (host_names[i], movies[j]);
            }
        }
    }

  public static void main (String[] args)
    {
      System.loadLibrary ("avdemo");
      
      System.out.println ("Launching the Server Discovery frame.");
      JFrame frame = new Server_Discovery ();
      frame.setVisible (true);
    }
}

