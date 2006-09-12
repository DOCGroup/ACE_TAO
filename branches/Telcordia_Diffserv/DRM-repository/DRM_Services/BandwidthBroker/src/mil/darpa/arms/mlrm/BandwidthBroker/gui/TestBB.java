package mil.darpa.arms.mlrm.BandwidthBroker.gui;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class TestBB
{
   public static Settings sts  = new Settings ("TestBB.set");

   private static class MainWindow extends JFrame implements ActionListener
   {
      JMenuItem exit = new JMenuItem ("Exit");
      JMenuItem db = new JMenuItem ("Init DB");
      JMenuItem bb = new JMenuItem ("Start Bandwidth Broker");
      JMenuItem fp = new JMenuItem ("Start Flow Provisioner");
      JMenuItem s = new JMenuItem ("Settings");

      MainWindow ()
      {
         super ("Bandwidth Broker Test Panel");
	 setSize (1000,750);
         addMenuBar ();
         exit.addActionListener (this);
         s.addActionListener (this);

	 initDisplay ();
         setVisible (true);

      }

      NS nsBox;
      DB dbBox;
      BB bbBox;
      BB2 bbBox2;
      FP fpBox;
      ProcessBox nmBox, cvsBox, buildBox;

      public void initDisplay ()
      {
	 JTabbedPane jt = new JTabbedPane ();
	 
	 JPanel tests = new TestPanel ();
         JPanel dashb = new JPanel ();
	 JPanel nView = new NetworkPanel ();

	 dashb.setLayout (null);

	 dbBox = new DB (sts);
	 dbBox.setLocation (100, 50);
	 dashb.add (dbBox);

	 nsBox = new NS (sts);
	 nsBox.setLocation (300, 50);
	 dashb.add (nsBox);

	 bbBox = new BB (sts);
	 bbBox.setLocation (100, 150);
	 dashb.add (bbBox);

	 bbBox2 = new BB2 (sts);
	 bbBox2.setLocation (300, 150);
	 dashb.add (bbBox2);

	 fpBox = new FP (sts);
	 fpBox.setLocation (100, 250);
	 dashb.add (fpBox);

	 nmBox = new FaultMonitor (sts);
	 nmBox.setLocation (300, 250);
	 dashb.add (nmBox);

	 nmBox = new PerfMonitor (sts);
	 nmBox.setLocation (500, 250);
	 dashb.add (nmBox);

	 JPanel b = new JPanel ();
	 b.setLayout (null);

	 cvsBox = new CVS (sts);
	 cvsBox.setLocation (100, 50);
	 b.add (cvsBox);

	 buildBox = new Build (sts);
	 buildBox.setLocation (100, 150);
	 b.add (buildBox);
	 
	 LogPanel lp;
	 getContentPane ().add (lp = new LogPanel (), BorderLayout.SOUTH);
	
	 ProcessBox.lp = lp;
	 
	 jt.add ("Build", b);
	 jt.add ("Deployment", dashb);
	 jt.add ("Tests", tests);
	 jt.add ("Network View", nView);
	 getContentPane ().add (jt);
      }



      public void actionPerformed (ActionEvent ae)
      {
         if (ae.getSource () == exit)
	 {
	    //cleanup

	    System.exit (1);
	 }
         else if (ae.getSource () == s)
         {
            new ShowSettings (this, sts);
         }
      }

      private void addMenuBar ()
      {
         JMenuBar jmb = new JMenuBar ();
         JMenu main = new JMenu ("Test");
         main.add (s);
         main.add (exit);

         JMenu setup = new JMenu ("Setup");
         setup.add (db);
         setup.add (bb);
         setup.add (fp);

         jmb.add (main);
         jmb.add (setup);
         setJMenuBar (jmb);
      }
   }

   public static void main (String args [])
   {
      MainWindow m = new MainWindow ();
   }


}
