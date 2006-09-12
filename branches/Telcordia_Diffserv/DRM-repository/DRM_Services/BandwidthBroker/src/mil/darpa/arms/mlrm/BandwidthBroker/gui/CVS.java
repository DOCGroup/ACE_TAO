package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class CVS extends ProcessBox
{

   CVS (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      name = "CVS Download";
      setSize (new Dimension (350, 40));
   }

   public String makeScript ()
   {
      String rc = 
         ". ./.devInit; . ./CVSInit; cd $BB; \n" +
         "cvs -d $CVSROOT update -Pd \n" +
         "echo CVS update completed. /n";
      return rc;
   }

   public String getPassword ()
   {
      return "sgadgil123";
   }

}
