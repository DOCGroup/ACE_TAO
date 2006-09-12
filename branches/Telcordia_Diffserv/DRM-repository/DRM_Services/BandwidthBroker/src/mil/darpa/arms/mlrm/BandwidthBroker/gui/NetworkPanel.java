package mil.darpa.arms.mlrm.BandwidthBroker.gui;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.io.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;

public class NetworkPanel extends JPanel implements ActionListener
{
   String showVlan = "100";
   String vlans [];
   JPopupMenu popup = new JPopupMenu ();
   JMenuItem refresh = new JMenuItem ("Refresh Display");
   Hashtable vlanMenuItems = new Hashtable ();
   boolean L2 = false;

   NRDB nrdb;
   Image ship;
   public NetworkPanel ()
   {
      try
      {
	 ship = javax.imageio.ImageIO.read (new File ("ship.jpg"));
	 setLayout (null);
	 //setBackground (Color.white);
	 

         NRDB.numConnections = 4;
         NRDB.dbServerFQN = "pool1db.bb-largenet.arms.emulab.net";
         NRDB.dbLoginName= "arms";
         NRDB.dbPasswordValue = "arms";

         nrdb = new NRDB ();
	 if (L2 == true)
	 {
            vlans = nrdb.getAllVlans ();
	 }

	 refresh.addActionListener (this);
	 popup.add (refresh);

	 if (L2 == true)
	 {
	    for (int i = 0; i < vlans.length; i++)
	    {
	       JMenuItem showVlan = new JMenuItem ("Show VLAN " + vlans [i]);     
	       vlanMenuItems.put (showVlan, vlans [i]);
	       showVlan.addActionListener (this);
	       popup.add (showVlan);
	    }
	 }
         addMouseListener (new PopupListener ());


         readTopologyDisplayFile ();
	 nrdb.releaseConnection ();
      }
      catch (Exception e)
      {
	 e.printStackTrace ();
         System.out.println ("Error reading topology file");
      }
      setLayout (null);

   }

   public void paintComponent (Graphics g)  
   {    
      super.paintComponent (g);
      if (ship != null) 
         g.drawImage (ship, 0,0,this.getWidth(),this.getHeight(),this);  
   }

   public void actionPerformed (ActionEvent ae)
   {
      JMenuItem jmi = (JMenuItem) ae.getSource ();
      if (jmi != refresh)
      {
	 if (L2)
	 {
            showVlan = (String) vlanMenuItems.get (ae.getSource ());
	 }
      }
      this.setVisible (false);
      this.removeAll ();
      try
      {
         nrdb = new NRDB ();
         readTopologyDisplayFile ();
	 nrdb.releaseConnection ();
      }
      catch (Exception e )
      {
      }
      this.setVisible (true);
   }

   private class PopupListener extends MouseAdapter 
   {
       public void mousePressed(MouseEvent e) {
          maybeShowPopup(e);
       }

       public void mouseReleased(MouseEvent e) 
       {
          maybeShowPopup(e);
       }

       private void maybeShowPopup(MouseEvent e) 
       {
          if (e.isPopupTrigger()) 
	  {
            popup.show(e.getComponent(), e.getX(), e.getY());
          }
       }
   }

   Hashtable netObjs = new Hashtable ();
   Vector nList = new Vector ();

   private void processLine (String line) throws Exception
   {
      String element = null, name = null; 
      int x1 = 100, y1 = 100, x2 = 200, y2 = 200;

      StringTokenizer st = new StringTokenizer (line);

      if (st.hasMoreTokens ())
      {
         element = st.nextToken ();
      }
      if (st.hasMoreTokens ())
      {
         name = st.nextToken ();
      }
      if (element == null)
         return;
      if (element.equals ("link"))
      {
	 String n1 = "";
	 String n2 = "";
         if (st.hasMoreTokens ())
         {
	    n1 = st.nextToken ();
	 }
         if (st.hasMoreTokens ())
         {
	    n2 = st.nextToken ();
	 }
	 NetObject no1 = (NetObject) netObjs.get (n1);
	 NetObject no2 = (NetObject) netObjs.get (n2);
	 if (no1 != null & no2 != null)
	 {
	    System.out.println ("Adding link at" + no1.x + ":" + no1.y);
	    nList.add (new Link (name, no1.x + 30, no1.y + 30, 
	                               no2.x + 30, no2.y + 30));
	 }

	 
         return;
      }

      //System.out.println (element + ":" + name + ":" + x1 + ":" + y1);
      if (st.hasMoreTokens ())
      {
         x1 = Integer.parseInt (st.nextToken ());
      }
      if (st.hasMoreTokens ())
      {
         y1 = Integer.parseInt (st.nextToken ());
      }
      if (st.hasMoreTokens ())
      {
         x2 = Integer.parseInt (st.nextToken ());
      }
      if (st.hasMoreTokens ())
      {
         y2 = Integer.parseInt (st.nextToken ());
      }
      if (element.equals ("switch"))
      {
	 Switch s;
         netObjs.put (name, s = new Switch (name, x1, y1));
	 nList.add (s);
      }
      if (element.equals ("host"))
      {
	 Host h;
         netObjs.put (name, h = new Host (name, x1, y1));
	 nList.add (h);
      }
   }

   private void readTopologyDisplayFile () throws Exception
   {
      String fileName = TestBB.sts.get ("networkView.topologyDisplayFile");

      FileReader fr = new FileReader (fileName);
      BufferedReader bf = new BufferedReader (fr);
      nList = new Vector ();

      while (true)
      {
        String line = bf.readLine ();
	if (line == null)
	   break;
        else
	{
	   line = line.trim ();
	   if (line.startsWith ("#"))
	      continue;
	   processLine (line);
	}
      }
      //for (int i = nList.size ()-1; i >-1; i--)
      for (int i = 0; i < nList.size (); i++)
      {
         add ((JPanel)nList.get (i));
      }

   }

   private class NetObject extends JPanel
   {
      String name;
      int x, y;
      NetObject (String name)
      {
	 this.name = name;
         this.setOpaque (false);
      }

      NetObject (String name, int x, int y)
      {
	 this (name);
	 this.x = x;
	 this.y = y;
	 setLocation (x, y);
      }
   }

   private class Switch extends NetObject
   {
      Switch (String name, int x, int y)
      {
         super (name, x, y);
	 setSize (100, 50);
	 setLayout (new BorderLayout ());
	 JLabel l = new JLabel (name, 
	                        new ImageIcon ("switch.gif"), 
				JLabel.CENTER );
	 l.setVerticalTextPosition(JLabel.BOTTOM);
	 l.setHorizontalTextPosition(JLabel.CENTER);
         add (l);
      }
   }

   private class Host extends NetObject
   {
      Host (String name, int x, int y)
      {
         super (name, x, y);
	 setSize (45, 60);
	 setLayout (new BorderLayout ());
	 //JLabel jl = new JLabel (name);
	 //jl.setOpaque (false);
	 //add (jl, BorderLayout.SOUTH);
	 JLabel l = new JLabel (name, 
	                        new ImageIcon ("host.jpg"), 
				JLabel.CENTER );
	 l.setVerticalTextPosition(JLabel.BOTTOM);
	 l.setHorizontalTextPosition(JLabel.CENTER);
         add (l);
      }
   }

   private class Link extends NetObject
   {
      boolean flip = false;
      boolean status = true;
      int w, h;
      String vlans [] = new String [0];
      Link (String name, int x1, int y1, int x2, int y2)
      {
         super (name);
	 int ltx = x1;
	 w = x2 - x1;
	 if (x1 > x2) { ltx = x2; w = -w; flip = !flip; }
	 int lty = y1;
	 h = y2 - y1;
	 if (y1 > y2) {lty = y2; h = -h; flip = !flip;}
	 setLocation (ltx, lty);
	 if (w == 0) w = 2;
	 if (h == 0) h = 2;
	 setSize (w, h);
	 //setBackground (Color.red);
	 //setOpaque (true);
	 try
	 {

	    if (nrdb.getLinkStatus (name).equals ("up"))
	       status = true;
            else
	       status = false;
	    if (L2)
	    {
	       vlans = nrdb.getActiveVlansOnLink (name);
	       Arrays.sort (vlans);
	    }
         }
	 catch (Exception e)
	 {
	    //e.printStackTrace ();
	    System.out.println ("Error getting link information");
	 }
	 System.out.println (name + ltx + ":" + lty + ":" + w + ":" + h + ":" + status);
	
      }

      public void paint (Graphics g)
      {
         super.paint (g);
	 
	 if (L2)
	 {
	    if (Arrays.binarySearch (vlans, showVlan) >= 0)
	    {
	       g.setColor (Color.blue);
	    }
	    else
	    {
	       g.setColor (Color.black);
	    }
	 }
	 else 
	 {
	    g.setColor (Color.black);
	 
	 }
	 if (status == false)
	    g.setColor (Color.red);

	 if (flip)
	 {
	    g.drawLine (0, h-1, w-1, 0);
	    g.drawLine (1, h-1, w-1, 1);
	    g.drawLine (0, h-2, w-2, 0);
	 }
         else
	 {
	    g.drawLine (0, 0, w-1, h-1);
	    g.drawLine (1, 0, w-1, h-2);
	    g.drawLine (0, 1, w-2, h-1);
	 }

	 // reset the graphics color
	 g.setColor (Color.black);
      }
   }
}
