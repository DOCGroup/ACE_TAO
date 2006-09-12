//$Id: TopHome.java
/*
 * TopHome.java - The base class for aggregate functions on all the 
 * network entities
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

import java.sql.*;
import java.util.*;

/**
 * This class is a base class for aggregate functions on all the network 
 * objects
 *
 */
public abstract class TopHome
{
   // oracle table associated with the object.
   protected String table;  
   private Connection c = null;
   protected TopBean homeBean;
   boolean genFQN = false;

   public TopHome (Connection c, String table)
   {
      this.table = table;
      this.c = c;
   }

   public String getTable ()
   {
      return table;
   }

   public Connection getConnection ()
   {
      return c;
   }

   /**
    * finds objects by fqn
    *
    */
   public Top findByID (String id) throws CDBException
   {
      AttributeValues av = new AttributeValues ();

      av.addNameValue (Top.ID, id);
      ObjectList ol = findByAttributes (av);
      if (ol.size () == 0)
      {
         // does not exist
         throw new CDBException ();
      }
      else
         return getInstance (id);
   }


   abstract public Top getInstance (String id);

   /**
    * finds objects that match attribute values
    * attribute values can contain wild character such as %
    *
    */
   public ObjectList findByAttributes (AttributeValues av) throws CDBException
   {
      return findByAttributes (av, Top.ID, this);
   }

   public void remove (AttributeValues av) throws CDBException
   {
      String qStr = "DELETE  from "  + table + getWhereClause (av);
      DbUtil.executeUpdate (c, qStr, false);
   }

   public void removeAll () throws CDBException
   {
      String qStr = "DELETE  from "  + table;
      DbUtil.executeUpdate (c, qStr, false);
   }

   public String [] selectAttrWhereIn (Attribute findAttr, 
                                       Attribute whereAttr, 
				       String [] set) throws CDBException
   {
      final java.util.Vector out = new Vector ();
      final Attribute fa = findAttr;

      String qStr = "SELECT " + findAttr + " from " + table;
      if (set != null && whereAttr != null);
      {
         StringBuffer whereClause = new StringBuffer (100);
	 whereClause.append (" where " + whereAttr + " IN (");
	 for (int i = 0; i < set.length; i++)
	 {
	    whereClause.append (whereAttr.toDbString (set [i]) + ",");
	 }
	 if (set.length > 0)
	    whereClause.deleteCharAt (whereClause.length () -1);
	 whereClause.append (")");
         qStr = qStr + whereClause.toString ();
      }

      ResultProcessor rp = new ResultProcessor ()
      {

         // called for every record in ResultSet
         public void processRecord (ResultSet rs) throws SQLException
         {
            out.add (rs.getString (fa.toString ()));
	    
         }
      };
      System.out.println ("In query:" + qStr);
      DbUtil.executeQuery (c, qStr, rp);
      String [] rc = new String [out.size ()];

      out.toArray (rc);
      return rc;
   }
    
   public void bulkUpdate (AttributeValues values, AttributeValues where) throws CDBException 
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

      qStr += getWhereClause (where);
      //System.out.println (qStr);
      DbUtil.executeUpdate (c, qStr, false);

   }
   public String getWhereClause (AttributeValues av)
   {
      String qStr = "";

      if (av != null)
      {
         if (av.size () > 0)
         {
            qStr += " WHERE ";
            Attribute [] attr = av.getAttributes ();
            for (int i = 0; i < attr.length; i++)
            {
               String val = attr [i].getValue ();
   
               qStr += " " + attr [i].toString ();
   
               if (val.indexOf ('%') == -1) // no wild card
                  qStr += " = ";
               else
                  qStr += " LIKE ";
               qStr +=  attr [i].toDbString (val) + " AND ";
            }
            // remove the last AND
            qStr = qStr.substring (0, qStr.lastIndexOf ("AND"));
         }
      }

      return qStr;

   }

   public Vector findAttributesByAttributes (AttributeValues av, 
                                       Attribute fAttr []) throws CDBException
   {
      final Attribute [] findAttr = fAttr;
      final Vector t [] = new Vector [findAttr.length];

      String qStr = "SELECT ";

      for (int i = 0; i < fAttr.length; i++)
      {
	 t [i] = new Vector ();
         qStr += " " + fAttr[i] + ",";
      }
      qStr = qStr.substring (0, qStr.length () - 1) + " from " + table + " " +
                    getWhereClause (av);


      ResultProcessor rp = new ResultProcessor ()
      {
         // called for every record in ResultSet
         public void processRecord (ResultSet rs) throws SQLException
         {
	    for (int i = 0; i < findAttr.length; i++)
	    {
               String value = rs.getString (findAttr [i].toString ());
	       t[i].add (value);
	    }
         }
      };
      DbUtil.executeQuery (c, qStr, rp);

      final Vector rc = new Vector ();

      for (int i = 0; i < findAttr.length; i++)
      {
         String [] rca = new String [t [i].size ()];
	 t [i].toArray (rca);
         rc.add (rca);
      }
      
      return rc;
   }

   protected ObjectList findByAttributes (AttributeValues av, 
                                       Attribute sAttr,
                                       TopHome sHome) throws CDBException
   {
      final ObjectList ol = new ObjectList ();
      final Attribute searchAttr = sAttr;
      final TopHome searchHome = sHome;

      String qStr = "SELECT " + searchAttr.toString () + " from " + table +
                    getWhereClause (av);


      ResultProcessor rp = new ResultProcessor ()
      {
         // called for every record in ResultSet
         public void processRecord (ResultSet rs) throws SQLException
         {
            String ofqn = rs.getString (searchAttr.toString ());
            ol.add (searchHome.getInstance (ofqn));
         }
      };
      DbUtil.executeQuery (c, qStr, rp);
      return ol;
   }

   public Top create (AttributeValues av) throws CDBException
   {
      String id;

      Attribute [] attr = av.getAttributes ();

      String qStr = "INSERT  into " + table + " (";

      for (int i = 0; i < attr.length; i++)
      {
         qStr += " " + attr [i].toString () + ",";
      }

      qStr = qStr.substring (0, qStr.length () - 1) + ") values (";

      for (int i = 0; i < attr.length; i++)
      {
         qStr += " " + attr [i].toDbString (attr [i].getValue ()) + ",";
      }
      qStr = qStr.substring (0, qStr.length () - 1) + ") ";

      DbUtil.executeUpdate (c, qStr, false);

      id = "" + getLastInsertID ();
      return getInstance (id);
   }

   /** This methods calls the SQL "LAST_INSERT_ID()". 
    * <br />
    * Returns the ID generated for an AUTO_INCREMENT column by the
    * previous query. Use this function after you have performed an
    * INSERT query into a table that contains an AUTO_INCREMENT field. 
    *
    * Note that the value of the SQL LAST_INSERT_ID() function
    * always contains the most recently generated AUTO_INCREMENT value,
    * and is not reset between queries because the value of that function
    * is maintained in the server. 
    */
   public int getLastInsertID() 
   {
	int result = -1;
	try
	{
	   ResultSet rs = c.createStatement().
	                  executeQuery("SELECT LAST_INSERT_ID()");
	   if (rs.next())
	   {
              result = rs.getInt(1);
	   }
	}
	catch (SQLException e)
	{
	   //printSQLException(e, "lastInsertID");
	}
	return result;
    }


   public void remove (String id) throws CDBException
   {
      String qStr = "DELETE from " + table + 
	  //  " WHERE  " + Top.ID + " = '" + id + "'" ;
	  " WHERE  " + Top.ID + " = " + id;  // ID is a long : Kirthika
      //System.out.println (qStr);
      DbUtil.executeUpdate (c, qStr, false);
   }

   public Attribute getAttributeByName (String name)
   {
      return homeBean.getAttributeByName (name);
   }

}
