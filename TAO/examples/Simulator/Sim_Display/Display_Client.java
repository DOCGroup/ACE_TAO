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
import java.net.*;
import java.io.*;
import java.awt.*;

public class Display_Client
{
  public final static int DEFAULT_PORT = 5555;
  
  public final static String CPU_USAGE = "CPU Usage (%)",
    LATENCY = "Latency (ms)",
    JITTER = "Jitter (ms)",
    OVERHEAD = "Overhead (%)",
    DEADLINES = "Missed Deadlines (#)";
  
  //private Display display_;


  public Display_Client (Display display)
    {
      this (DEFAULT_PORT, display);
    }
  
  public Display_Client (int port, Display display)
    {
      String art_hor_name =
	Display_Object_Factory.resolve_name
	(Display_Object_Factory.ART_HORIZON_ENUM);
      String weapons_name =
	Display_Object_Factory.resolve_name
	(Display_Object_Factory.WEAPONS_ENUM);
      
      display.add_metric (CPU_USAGE, 100);
      display.add_metric (OVERHEAD, 100);
      display.add_metric (JITTER, 10);
      display.add_metric (LATENCY, 10);
      display.add_metric (DEADLINES, 1);
      display.add_simulator (weapons_name);
      display.add_simulator (art_hor_name);
    }


  public static void main (String[] args)
    {
      Frame sim_frame = new Frame ();
      Display display = new Display ();
      Display_Client disp_client = new Display_Client (display);      
      Display_Consumer display_consumer = new Display_Consumer (display);
      Dimension ss = Toolkit.getDefaultToolkit ().getScreenSize ();

      sim_frame.setResizable (false);
      sim_frame.setTitle ("Scheduler Demo");
      sim_frame.setBounds (ss.width/2 - 275, ss.height/2 - 275, 550, 550);
      sim_frame.setLayout (new BorderLayout ());
      sim_frame.add ("Center", display);

      sim_frame.show ();
      
      display_consumer.run ();
    }
}

