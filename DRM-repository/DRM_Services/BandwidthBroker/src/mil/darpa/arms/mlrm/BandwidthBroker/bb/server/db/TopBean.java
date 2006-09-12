//$Id: TopBean.java
/*
 * TopBean.java - The base class for all the network entities beans
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

import java.sql.*;
/**
 * This class is a base class for all the network objects
 *
 */

abstract public class TopBean implements Top
{
   private String id;

   protected AttributeList allAttributes = new AttributeList ();

   private String table;
   private Connection c;

   public TopBean (Connection c, String table, String id)
   {
      this.table = table;
      this.c = c;
      this.id = id;

      addToAllAttributes (Top.allAttr);
   }

   public Attribute getAttributeByName (String name)
   {
      Attribute a;
      
      for (int i = 0; i < allAttributes.size (); i++)
      {
         a = allAttributes.get (i);
         if (a.toString ().equals (name))
            return a;
      }
      return null;
   }

   protected void addToAllAttributes (Attribute [] aa)
   {
      for (int i = 0 ; i < aa.length; i++)
      {
         allAttributes.add (aa [i]);
         //System.out.println (aa [i]);
      }
   }
   /**
    * Returns a hashtable of all the attributes values of the object
    * the name of the attribute is the key in hashtable
    */
   public AttributeValues getAttributes () throws CDBException
   {
      return getAttributes (allAttributes);
   }

   /**
    * Returns a hashtable of attributes values of the object for 
    * attributes specified in AttributeList
    * the name of the attribute is the key in hashtable
    */
   public AttributeValues getAttributes (AttributeList al) throws CDBException
   {
      final AttributeValues av = new AttributeValues ();
      final AttributeList aList = al;
      

      String qStr = "SELECT";

      for (int i = 0; i < aList.size (); i++)
      {
         qStr += " " + aList.get (i) + ",";
      }

      qStr = qStr.substring (0, qStr.length () - 1) + " from " + table + " ";

      qStr += "WHERE " + ID + " = '" +  id + "'";

      ResultProcessor rp = new ResultProcessor ()
      {
         // called for every record in ResultSet
         public void processRecord (ResultSet rs) throws SQLException
         {
            for (int i = 0; i < aList.size (); i++)
            {
               String value = rs.getString (aList.get (i).toString ());
               if (value == null)
                  value = "";
               av.addNameValue (aList.get (i), value);
            }
         }
      };

      DbUtil.executeQuery (c, qStr, rp);

      return av;
   }

   public void setAttributes (AttributeValues values) throws CDBException 
   {
      Attribute [] attr = values.getAttributes ();

      String qStr = "UPDATE " + table + " ";
      qStr += "SET ";

      for (int i = 0; i < attr.length; i++)
      {
         qStr += " " + attr [i] + " = " + attr [i].getDbValue () + ",";
      }

      // remove last comma
      qStr = qStr.substring (0, qStr.length () - 1) + " ";

      /* for mysql
      qStr = qStr.substring (0, qStr.length () - 1) + ") = (";

      for (int i = 0; i < attr.length; i++)
      {
         qStr += " " + attr [i].getDbValue () + ",";
      }

      qStr = qStr.substring (0, qStr.length () - 1) + ") ";
      */

      qStr += "WHERE " + ID + " = '" +  id + "'";
      //System.out.println (qStr);
      DbUtil.executeUpdate (c, qStr, false);

   }

   public String getID ()
   {
      return id;
   }

   protected Connection getConnection ()
   {
      return c;
   }

   public boolean equals (Object o)
   {
      try
      {
         TopBean tb = (TopBean) o;
         return id.equals (tb.id);
      }
      catch (Exception e)
      {
      }
      return false;
   }

   public int hashCode ()
   {
      String hStr = getClass().getName ()+ ":" + id;
      return hStr.hashCode ();
   }

}
