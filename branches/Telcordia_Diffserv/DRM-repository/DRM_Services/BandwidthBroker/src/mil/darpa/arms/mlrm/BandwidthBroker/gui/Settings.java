package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.io.*;


public class Settings extends AbstractTableModel
{
   public Hashtable set = new Hashtable ();
   Vector setData = new Vector ();


   public String get (String name)
   {
      NVPair nv = (NVPair) set.get (name);
      if (nv != null)
         return nv.value;
      else
         return "";
   }

   private class NVPair 
   {
      String name;
      String value;

      NVPair (String name, String value)
      {
         this.name = name; this.value = value;
      }
   }
   
   String fileName;
   public Settings (String fileName)
   {
      this.fileName = fileName;
      try
      {
         BufferedReader br = 
	            new BufferedReader (new FileReader (fileName));

	 String line;
	 while ((line = br.readLine ()) != null)
	 {
	    System.out.println (line);
	    String name = null, value = null;
	    StringTokenizer st = new StringTokenizer (line, "=");
	    if (st.hasMoreTokens ())
	    {
	       name = st.nextToken ();
	    }
	    if (st.hasMoreTokens ())
	    {
	       value = line.substring (line.indexOf ('=')+1);
	       value = value.trim ();
	    }
	    if (name != null && value != null)
	    {
	       name = name.trim ();
	       add (name, value);
	    }
	    
		  
	 }
      }
      catch (IOException e)
      {
         e.printStackTrace ();
      }
   }

   public void save ()
   {
      try
      {
         FileWriter fw = new FileWriter (fileName);
         for (int i = 0; i < setData.size (); i++)
         {
            NVPair nv = (NVPair) setData.get (i);
	    String s = nv.name + " = " + nv.value + "\n";
	    fw.write (s, 0, s.length ());
         }
         fw.close ();
      }
      catch (IOException e)
      {
	 JOptionPane.showMessageDialog (null, "Save Error", "Error saving the settings", JOptionPane.ERROR_MESSAGE);
         System.out.println ("Error saving the settings");
      }

   }

   public void add (String name, String value)
   {
      NVPair nv;
      setData.add (nv = new NVPair (name, value));
      set.put (name, nv);
      fireTableDataChanged ();
   }
   
   public void delete (String name)
   {
      NVPair nv = (NVPair) set.remove (name);
      setData.remove (nv);
      fireTableDataChanged ();
   }
   public void delete (int row)
   {
      NVPair nvp = (NVPair) setData.get (row);
      delete (nvp.name);
   }

   public int getRowCount ()
   {
      return setData.size ();
   }

   public int getColumnCount()
   {
      return 2;
   }

   public boolean isCellEditable (int row, int col)
   {
      if (col == 1)
      {
         return true;
      }
      else
         return false;
   }

   public Object getValueAt(int row, int column)
   {
      NVPair nvp = (NVPair) setData.get (row); 

      if (column == 0)
      {
          return nvp.name;
      }
      else
         return nvp.value;
   }

   public void setValueAt(Object o, int row, int column)
   {
      NVPair nvp = (NVPair) setData.get (row); 

      if (column == 1)
      {
          nvp.value = (String) o;
      }
   }


}
