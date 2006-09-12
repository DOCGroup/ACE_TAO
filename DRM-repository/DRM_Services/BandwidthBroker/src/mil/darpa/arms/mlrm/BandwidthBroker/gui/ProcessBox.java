package mil.darpa.arms.mlrm.BandwidthBroker.gui;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;
import java.util.*;

import com.jcraft.jsch.*;

public abstract class ProcessBox extends JPanel implements ActionListener
{

   public static LogPanel lp;
   boolean disabled = false;

   Settings s;
   String name = "unnamed";
   JLabel l;
   boolean selected = false;
   JPopupMenu popup = new JPopupMenu ();
   JMenuItem start = new JMenuItem ("Start");
   JMenuItem stop = new JMenuItem ("Stop");
   JMenuItem status = new JMenuItem ("Status");

   // override this function to get a different look and feel
   public void fillBox ()
   {
      setLayout (new BorderLayout ());
      add (l = new JLabel (name));
      if (disabled)
         l.setBackground (Color.gray);
      else
         l.setBackground (Color.white);
      l.setOpaque (true);
      l.setHorizontalAlignment (JLabel.CENTER);
      l.setBorder (BorderFactory.createLineBorder (Color.black, 1));
   }

   ProcessBox (Settings s) 
   {
      this.s = s;
      setSize (new Dimension (150, 40));
      init ();
      fillBox ();
      popup.add (start);
      popup.add (stop);
      popup.add (status);

      start.addActionListener (this);
      stop.addActionListener (this);
      status.addActionListener (this);
      l.addMouseListener (new PopupListener ());
      
   }
   abstract public void init ();

   public void actionPerformed (ActionEvent ae)
   {
      if (ae.getSource () == start)
      {
         System.out.println ("Starting " + name + " process.");
	 startProcess ();
	 l.setBackground (Color.green);
      }
      else if (ae.getSource () == stop)
      {
         System.out.println ("Stopping " + name + " process");
	 stopProcess ();
	 l.setBackground (Color.white);
      }
      else if (ae.getSource () == status)
      {
         System.out.println ("Querying status for " + name + " process");
      }
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
	    if (!disabled)
               popup.show(e.getComponent(), e.getX(), e.getY());
          }
       }
   }


   private class MyUserInfo implements UserInfo
   {
      String password = "Eaton#40";

      public boolean promptYesNo (String s) { return true;}
      public String getPassword () { return password;}
      public boolean promptPassword (String s) { return true; }
      public boolean promptPassphrase (String s) { return false; }
      public String getPassphrase () { return ""; }
      public void showMessage (String message) { System.out.println (message);}
   }

   private static Hashtable pwdCache = new Hashtable ();

   private class SSHSession extends Thread
   {

      BufferedReader so;
      Session s;

      PrintStream si;
      boolean up = false;

      public SSHSession (String host, String user, String password)
      {
         if (password == null)
	 {
	    password = (String) pwdCache.get (host + "/" + user);
	    if (password == null)
	       password = getPassword (user, host);
	 }
	 else
	 {
	    pwdCache.put (host + "/" + user, password);
	 }
	 MyUserInfo myu = new MyUserInfo ();
	 myu.password = password;
	 s = initSession (user, host, myu);
	 if (s != null)
	    up = true;
      }

      void disconnect ()
      {
	 if (s != null)
            s.disconnect ();
	 s = null;
	 up = false;
      }

      private String getPassword (String user, String host)
      {
         return "Eaton#40";
      }

      private Session initSession (String user, String host, UserInfo myu)
      {
         try
         {
            JSch jsch = new JSch ();
	    Session session = null;
	    session = jsch.getSession (user, host, 22);
	    session.setUserInfo (new MyUserInfo ());
   
            java.util.Properties config = new java.util.Properties ();
   
	    config.put("compression.s2c", "none");
	    config.put("compression.c2s", "none");
	    session.setConfig(config);
   
	    //session.setTimeout(5000);
	    session.setTimeout(0);
	    session.connect();
   
            Channel channel = session.openChannel ("shell");

	    OutputStream out;
	    InputStream in;

	    out = channel.getOutputStream ();
	    in = channel.getInputStream ();
	    channel.connect ();
	 
	    so = new BufferedReader (new InputStreamReader (in)); 
	    si = new PrintStream (out);
	    return session;

         }
	 catch (Exception e)
	 {
	    e.printStackTrace ();
	    System.out.println ("Session could not be started.");
	 }
	 return null;
      }

      PConsole pc;

      public void run ()
      {
	 try
	 {
	    String l;

	    while (s != null) 
	    {
               if ((l = so.readLine ()) != null)
	       {
	          pc.addLine (l);
	       }
	    }
	 }
	 catch (Exception e)
	 {
	    pc.addLine ("Exception running the SSH Session");
	 }
      }
     
   }

   private class PConsole extends JPanel
   {
      JTextArea ta;

      int maxRows = 200;

      PConsole ()
      {
         setLayout (new BorderLayout ());
	 add (new JScrollPane (ta = new JTextArea ()));
	 ta.setFont (new Font ("Monospaced", Font.PLAIN, 16));
      }

      public void addLine (String line)
      {
         ta.append (line);
	 ta.append ("\n");

	 while (ta.getLineCount () > maxRows)
	 {
	    try
	    {
	       int eraseLen = ta.getLineStartOffset (1);
	       ta.replaceRange ("", 0, eraseLen);
	    }
	    catch (Exception e)
	    {
	    }
	 }
      }
   }

   SSHSession ss;

   String getHost ()
   {
      return "pool1bb.bb-largenet.arms.emulab.net";
   }
   public void stopProcess ()
   {
      if (ss != null)
      {
         ss.disconnect ();
         lp.removeLogPane (ss.pc);
         ss = null;
      }
   }
   public void startProcess ()
   {
      if (ss != null)
         return;

      try
      {
	 ss = new SSHSession (getHost (), "sgadgil", "Eaton#40");

	 if (ss.up)
	 {
	    ss.si.println (makeScript ());
	    ss.pc = new PConsole ();
	    lp.addLogPane (name, ss.pc);
	    ss.start ();

	    String p = null;
	    if ((p = getPassword ()) != null)
	    {
	       Thread.sleep (5000);
	       ss.si.println (p);
	    }
	 }
	 else
	    ss = null;

      }
      catch (Exception e)
      {
      }
	     
	 
   }

   public String getPassword ()
   {
      return null;
   }

   public String makeScript ()
   {
      return "ls -al";
      //to be overridden

   }

}
