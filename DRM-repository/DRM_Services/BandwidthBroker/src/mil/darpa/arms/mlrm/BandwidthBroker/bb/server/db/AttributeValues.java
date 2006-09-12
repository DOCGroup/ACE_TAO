//$Id: AttributeValues.java,
/*
 * AttributeValues.java - class for holding attribute name-value pair
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;


import java.util.*;

/**
 * This class holds attribute name-value pairs 
 *
 */

public class AttributeValues
{

   private Hashtable ht = new Hashtable ();

   /**
    * Adds a name value pair
    *
    */
    public void addNameValue (Attribute name, String value)
    {
       ht.put (name, value);
    }

   /**
    * gets a value for name
    *
    */
    public String getValueForName (Attribute name)
    {
      return (String) ht.get (name);
    }

   /**
    * gets a values for all the names
    *
    */
    public String [][] getNameValuePairs ()
    {
       String [][] nv = new String [ht.size ()] [2];

       Enumeration e = ht.keys ();

       int i = 0;
       while (e.hasMoreElements ())
       {
          Attribute a = (Attribute) e.nextElement ();
          String name = a.toString ();
          nv [i][0] = name;
          nv [i][1] = (String) ht.get (a);
          i++;
       }
       return nv;
    }

    public Attribute []  getAttributes  ()
    {
       Attribute [] attr = new Attribute [ht.size ()];
       Enumeration e = ht.keys ();

       int i = 0;
       while (e.hasMoreElements ())
       {
          attr [i] = (Attribute) e.nextElement ();
          attr [i].setValue ((String) ht.get (attr [i]));
          i++;
       }
       return attr;
    }

    public String toString ()
    {
       String out = "{";

       String [][] sa = getNameValuePairs ();

       for (int i = 0; i < sa.length; i++)
       {
          out += sa [i][0] + ":" + sa [i][1] + ", ";
       }
       if (sa.length > 0)
          out = out.substring (0, out.length () - 2) + " }";

       return out;
    }

    public int size ()
    {
       return ht.size ();
    }

    public void print ()
    {
	String [][] s = getNameValuePairs ();
	for (int i =0; i < s.length ; i++)
	    {
		System.out.println ("AVP: "+s[i][0]+","+s[i][1]);
	    }
    }
}
