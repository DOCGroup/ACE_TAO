// $Id$ 

/**
  * This class pulls the structured simulation data from a socket and
  * funnels it into the display.
  *
  * @author Seth Widoff
  * @version $Id$
  * modified by Michael Kircher on 1998/02/19
  */

import java.util.*;
import java.awt.*;

class Display_Frame extends Frame
{
  MenuBar menubar_;
  Menu menu_quit_;
  MenuItem menu_quit_item_;

  public Display_Frame ()
    {
      super ();
      menubar_ = new MenuBar ();
      menu_quit_ = new Menu ("Quit");
      menu_quit_item_ = new MenuItem ("Quit");
      menu_quit_item_.enable ();
      menu_quit_.add (menu_quit_item_);
      menubar_.add (menu_quit_);

      setMenuBar (menubar_);
    }


 
  public boolean action (Event e, Object arg)
    {
      System.out.println ("action occured");
      if (e.target == menu_quit_item_)
	{
	  System.exit (0);
	  return true;
	}
      return false;
    }

}


public class Display_Client // extends java.applet.Applet
{
  public final static int DEFAULT_PORT = 5555;
  
  public final static String CPU_USAGE = "CPU Usage (%)",
    LATENCY = "Latency (ms)",
    JITTER = "Jitter (ms)",
    OVERHEAD = "Overhead (%)",
    DEADLINES = "Missed Deadlines (#)";
  
  
public synchronized void paint (Graphics g) {}

  public void init () 
    {

      String art_hor_name =
	Display_Object_Factory.resolve_name
	(Display_Object_Factory.ART_HORIZON_ENUM);
      String weapons_name =
	Display_Object_Factory.resolve_name
	(Display_Object_Factory.WEAPONS_ENUM);
      
      display.add_metric (CPU_USAGE, 1);
      display.add_metric (OVERHEAD, 1);
      display.add_metric (JITTER, 10);
      display.add_metric (LATENCY, 10);
      display.add_metric (DEADLINES, 2);
      display.add_simulator (weapons_name);
      display.add_simulator (art_hor_name);

      display_consumer = new Display_Consumer (display); //, this.getParameter("NS_IOR"), this);
      Dimension ss = Toolkit.getDefaultToolkit ().getScreenSize ();

      sim_frame.setResizable (false);
      sim_frame.setTitle ("Scheduler Demo");
      sim_frame.setBounds (ss.width/2 - 275, ss.height/2 - 275, 700, 700);
      sim_frame.setLayout (new BorderLayout ());
      sim_frame.add ("Center", display);

      sim_frame.show ();
      
      display_consumer.run ();
    }
 


  public Display_Client ()
    {
      sim_frame = new Display_Frame ();
      display = new Display ();

    }
  
  private Frame sim_frame;
  private Display display;
  private Display_Consumer display_consumer;

  public static void main (String[] args)
    {
      
      Display_Client display_client_ = new Display_Client ();
      display_client_.init ();
    }
}


