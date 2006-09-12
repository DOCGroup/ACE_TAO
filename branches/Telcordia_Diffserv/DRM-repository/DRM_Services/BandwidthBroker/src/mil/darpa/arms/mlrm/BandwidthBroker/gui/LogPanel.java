package mil.darpa.arms.mlrm.BandwidthBroker.gui;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class LogPanel extends JPanel
{
   JTabbedPane tp = new JTabbedPane ();

   public LogPanel ()
   {
      setPreferredSize (new Dimension (0, 300));
      init ();
   }

   private void init ()
   {
      JLabel l;

      setLayout (new BorderLayout ());
      add (l = new JLabel ("Process Consoles"), BorderLayout.NORTH);
      l.setBackground (Color.white);
      l.setOpaque (true);
      add (tp);
   }

   public void addLogPane (String name, Component p)
   {
      tp.add (name, p);
   }
   
   public void removeLogPane (Component p)
   {
      tp.remove (p);
   }
   
   public void selectLogPane (Component p)
   {
      tp.setSelectedComponent (p);
   }
}
