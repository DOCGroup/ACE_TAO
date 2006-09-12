package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class TestPanel extends JPanel
{
   public TestPanel ()
   {
      setLayout (new GridLayout (7, 1));
      add (new AddFlowTest (TestBB.sts));
      add (new DelFlowTest (TestBB.sts));
      add (new ModFlowTest (TestBB.sts));
      add (new StandbyTest (TestBB.sts));
      add (new ResumeTest (TestBB.sts));
      add (new ModeChange (TestBB.sts));
      add (new EventTest (TestBB.sts));
   }

   abstract class TestBox extends ProcessBox
   {
      TestBox (Settings s)
      {
         super (s);
      }
      String getHost ()
      {
         return s.get ("bbtest.host");
      }
   }
   class AddFlowTest extends TestBox
   {
      AddFlowTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Add Flow Test"));
	 add (jt = new JTextField (s.get ("test.AddFlowTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Add Flow Test";
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
	    " uname -a \n" +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
//Emulab
         rc = 
            "cd $DEPLOY \n" +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "bin/bigtest.sh \n" +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);
         return rc;
      }
      
   }

   class DelFlowTest extends TestBox
   {
      DelFlowTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Delete Flow Test"));
	 add (jt = new JTextField (s.get ("test.DelFlowTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Delete Flow Test";
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
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
// Emulab
            rc = "cd $DEPLOY\n" +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
// Emulab
         System.out.println (rc);
         return rc;
      }
      
   }

   class ModFlowTest extends TestBox
   {
      ModFlowTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Modify Flow Test"));
	 add (jt = new JTextField (s.get ("test.ModFlowTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Modify Flow Test";
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
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
// Emulab
            rc = "cd $DEPLOY\n" + 
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);
         return rc;
      }
      
   }

   class StandbyTest extends TestBox
   {
      StandbyTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Standby Test"));
	 add (jt = new JTextField (s.get ("test.StandbyTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Standby Test";
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
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
//Emulab
         rc = "cd $DEPLOY \n " +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);
         return rc;
      }
      
   }

   class ResumeTest extends TestBox
   {
      ResumeTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Resume Test"));
	 add (jt = new JTextField (s.get ("test.ResumeTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Resume Test";
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
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
	// Emulab

         rc = 
            "cd $DEPLOY \n " +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);
         return rc;
      }
      
   }

   class ModeChange extends TestBox
   {
      ModeChange (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Mode Change Test"));
	 add (jt = new JTextField (s.get ("test.ModeChangeTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Mode Change Test";
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
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
//Emulab
         rc = 
            "cd $DEPLOY\n " +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);
         return rc;
      }
   }
      
   class EventTest extends TestBox
   {
      EventTest (Settings s)
      {
         super (s);
      }

      JTextField jt;
      public void fillBox ()
      {
         add (new JLabel ("Network Event Test"));
	 add (jt = new JTextField (s.get ("test.NetworkEventTest.args"), 50));
         add (l = new JLabel ("Start/Stop"));
         l.setBackground (Color.white);
         l.setOpaque (true);
         l.setHorizontalAlignment (JLabel.CENTER);
         l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
      }

      public void init ()
      {
         name = "Network Event Test";
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
	    "mysql -u arms -parms < $BB/src/db/fault.sql \n" +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         System.out.println (rc);

//Emulab
         rc = 
            "cd $DEPLOY \n" +
            "export ONS_PORT=" + nsp + "; export ONS_HOST=" + nss + "\n" +
	    "export ARMS_ROOT=. \n" +
	    "export DRM_ROOT=" + drmRoot + "\n" +
	    ". ./bin/setup-classpath.sh \n " +
	    "mysql -u arms -parms < $BB/src/db/fault.sql \n" +
	    "java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite " +
	    "-ORBInitRef NameService=corbaloc::" + nss + ":" + nsp + 
	    "/NameService " + jt.getText () + "\n ";
         return rc;
      }
      
   }
}
