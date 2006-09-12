package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class FaultMonitor extends ProcessBox
{

   FaultMonitor (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      disabled = true;
      name = "Fault Monitor";
      //setSize (new Dimension (350, 40));
   }

}
