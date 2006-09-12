package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class FP extends ProcessBox
{

   FP (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      name = "Flow Provisioner";
      //setSize (new Dimension (350, 40));
   }

   public String makeScript ()
   {
      String nss = s.get ("namingservice.server");
      String nsp = s.get ("namingservice.port");
      String deployd = s.get ("deploy.dir");
      String drmRoot = s.get ("drm.root");


      String rc = 
         ". ./.devInit; cd " + deployd + "\n " +
         "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	 "export ARMS_ROOT=. \n" +
	 "export DRM_ROOT=" + drmRoot + "\n" +
	 ". ./bin/setup-classpath.sh \n " +
         "java mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.FlowProvisioningServer " + 
	 s.get ( "fp.logprop") + " " + s.get ("fp.routerAuth") + " " + 
	 s.get ("fp.loopback") +  
	 " -ORBInitRef NameService=corbaloc::" + 
	 nss + ":" + nsp + "/NameService " +
	 " < bin/p ";

// Emulab rc

      rc = "cd $DEPLOY\n" +
         "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	 "export ARMS_ROOT=. \n" +
	 "export DRM_ROOT=" + drmRoot + "\n" +
	 ". ./bin/setup-classpath.sh \n " +
         "java mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.FlowProvisioningServer " + 
	 s.get ( "fp.logprop") + " " + s.get ("fp.routerAuth") + " " + 
	 s.get ("fp.loopback") +  
	 " -ORBInitRef NameService=corbaloc::" + 
	 nss + ":" + nsp + "/NameService " +
	 " < bin/p ";

      System.out.println (rc);
      return rc;
   }

}
