package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.event.*;
import java.awt.*;

public class ShowSettings extends JDialog implements ActionListener
{
   JButton save = new JButton ("Save");
   JButton cancel = new JButton ("Cancel");
   JButton delete = new JButton ("Delete");
   JButton add = new JButton ("Add");
   JTable jt;
   Settings s;

   public ShowSettings (JFrame f, Settings s)
   {
      super (f, "BB Settings", true);
      this.s = s;
      Container c = getContentPane ();

      setSize (500, 300);
      setLocation (200, 200);

      c.setLayout (new BorderLayout ());
      JPanel jp = new JPanel ();
      jp.add (add);
      jp.add (delete);
      jp.add (save);
      jp.add (cancel);
      jp.setPreferredSize (new Dimension (0, 40));
      c.add (jp, BorderLayout.SOUTH);
      c.add (new JLabel ("BB Settings"), BorderLayout.NORTH);

      c.add (new JScrollPane (jt = new JTable (s)));

      add.addActionListener (this);
      delete.addActionListener (this);
      save.addActionListener (this);
      cancel.addActionListener (this);
      show ();
   }

   private class GetASetting  extends JDialog implements ActionListener
   {
      boolean done = false;
      String name, value;
      JTextField n = new JTextField (20);
      JTextField v = new JTextField (20);
      JButton add = new JButton ("Add");
      JButton cancel = new JButton ("Cancel");

      GetASetting (JDialog parent) 
      {
         super (parent, "Add a setting", true);
	 setSize (new Dimension (300, 200));
	 JPanel jp = new JPanel ();
	 jp.add (new JLabel ("Name")); jp.add (n);
	 jp.add (new JLabel ("Value")); jp.add (v);
	 jp.add (add);
	 jp.add (cancel);
	 add.addActionListener (this);
	 cancel.addActionListener (this);
	 getContentPane ().add (jp);
	 show ();
      }
      public void actionPerformed (ActionEvent ae)
      {
        if (ae.getSource () == add)
	{
	   done = true;
	   name = n.getText ();
	   value = v.getText ();
	}
	dispose ();
      }
   }

   public void actionPerformed (ActionEvent ae)
   {
      if (ae.getSource () == add)
      {
         GetASetting gas = new GetASetting (this);

	 if (gas.done == true)
	    s.add (gas.name, gas.value);
	 
      }
      else if (ae.getSource () == delete)
      {
         int row =  jt.getSelectedRow ();
	 if (row != -1)
	 {
	    s.delete (row);
	 }
      }
      else if (ae.getSource () == save)
      {
         s.save ();
      }
      else if (ae.getSource () == cancel)
      {
         dispose ();
      }
      
   }
}
