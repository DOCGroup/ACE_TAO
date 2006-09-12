package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;


public class NS extends ProcessBox
{

   NS (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      name = "Name Service";
   }
   String getHost ()
   {
      return s.get ("ns.host");
   }

   public String makeScript ()
   {
      String nss = s.get ("namingservice.server");
      String nsp = s.get ("namingservice.port");
      String rc = 
         ". ./.devInit\n" +
         "uname -a \n" +
         "tnameserv -ORBInitialPort " + nsp  + " \n";
      return rc;
   }


}
