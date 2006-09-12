package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class Build extends ProcessBox
{

   Build (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      name = "Build";
      setSize (new Dimension (350, 40));
   }

   public String makeScript ()
   {
      String rc = 
         ". ./.devInit; cd $BB; \n" +
         "ant dist\n";
      return rc;
   }

}
