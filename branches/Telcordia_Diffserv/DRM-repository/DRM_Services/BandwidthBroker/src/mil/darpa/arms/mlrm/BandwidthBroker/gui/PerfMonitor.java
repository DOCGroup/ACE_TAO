package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class PerfMonitor extends ProcessBox
{

   PerfMonitor (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      disabled = true;
      name = "Performance Monitor";
      //setSize (new Dimension (350, 40));
   }

}
