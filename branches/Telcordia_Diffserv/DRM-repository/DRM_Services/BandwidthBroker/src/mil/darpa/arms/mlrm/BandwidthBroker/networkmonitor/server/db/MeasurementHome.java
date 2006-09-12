//$Id$
/*
 * MeasurementHome.java - Home class for Measurement
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 */
package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;


import java.sql.*;



/**
 * This home class for Measurement
 *
 */

public class MeasurementHome extends TopHome
{
    java.sql.Connection conn;
    
   /**
    * Constructor
    */
   public MeasurementHome (java.sql.Connection c)
   {
      // defines the table
       // super (c, "Measurement");
       super (c, "MEASUREMENT"); // just for hsqldb
      homeBean = new MeasurementBean (c, getTable (), null);
      conn =c;
   }

    public Top createRecord (AttributeValues av) throws CDBException
   {
      String id;
      String table=new String ("MEASUREMENT");

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

      DbUtil.executeUpdate (conn, qStr, false);

      id = "" + getIdentityID ();
      return getInstance (id);
   }

    /** This methods calls the SQL "IDENTITY()". 
    * <br />
    * Returns the ID generated for an IDENTITY column by the
    * previous query. Use this function after you have performed an
    * INSERT query into a table that contains an AUTO_INCREMENT field. 
    *
    * Note that the value of the SQL IDENTITY() function
    * always contains the most recently generated AUTO_INCREMENT value,
    * and is not reset between queries because the value of that function
    * is maintained in the server. 
    */
   public int getIdentityID() 
   {
	int result = -1;
	try
	{
	   ResultSet rs = conn.createStatement().
	                  executeQuery("CALL IDENTITY ()");
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

   public Top getInstance (String id)
   {
      return new MeasurementBean (getConnection (), getTable (), id);
   }

   public Measurement narrow (Top t)
   {
      return (Measurement) t;
   }

   public String getGreaterThanOrLessThanEqualToWhereClause (AttributeValues av,
							     boolean greaterThanEqualTo)
   {
      StringBuffer qStr = new StringBuffer ("");

      if (av != null)
      {
         if (av.size () > 0)
         {
            qStr.append (" WHERE ");
            Attribute [] attr = av.getAttributes ();
            for (int i = 0; i < attr.length; i++)
            {
               String val = attr [i].getValue ();
   
               qStr.append (" " + attr [i].toString ());
	       
               if (val.indexOf ('%') == -1) // no wild card
		   {
		       if (greaterThanEqualTo)
			   qStr.append (" >= ");
		       else
			   qStr.append (" <= ");
		   }
               qStr.append (attr [i].toDbString (val) + " AND ");
	       NetworkMonitorServer.nmlogger.info ("where clause:"+qStr.toString ());
            }
            // remove the last AND
	    String q = qStr.toString ();
            qStr = new StringBuffer (q.substring (0, q.lastIndexOf ("AND")));
	    
         }
      }
      NetworkMonitorServer.nmlogger.info ("final where clause:"+qStr.toString ());
      return qStr.toString ();

   }
    
    public String getOrderByClause (AttributeValues av, 
				    boolean descending)
    {
	StringBuffer qStr = new StringBuffer ("");
	
	if (av != null)
	    {
		if (av.size () > 0)
		    {
			qStr.append (" ORDER BY ");
			Attribute [] attr = av.getAttributes ();
			// do only for fist attribute as only one required for this clause
			qStr.append (" " + attr [0].toString ());
			if (descending)
			    qStr.append (" DESC ");
			else
			    qStr.append (" ASC ");
		    }
	    }
      NetworkMonitorServer.nmlogger.info ("final order by clause:"+qStr.toString ());
      return qStr.toString ();

    }

   public ObjectList findSpecificByAttributes (AttributeValues av, 
						  AttributeValues otherAv,
						  boolean greaterThanEqualTo,
						  TopHome sHome) 
       throws CDBException
   {
      final ObjectList ol = new ObjectList ();
      final TopHome searchHome = sHome;

      String whclause = getGreaterThanOrLessThanEqualToWhereClause (otherAv, 
								    greaterThanEqualTo);
      // remove the extra WHERE
      StringBuffer qStr = new StringBuffer ("SELECT * from " + table);
      String whereClause= getWhereClause (av);
      if (whereClause.compareTo ("")!=0)
	  {
	      qStr.append (whereClause+" AND ");
	      // remove the second "WHERE"
	      qStr.append (whclause.substring (6, whclause.length()));
	  }
      else
	  qStr.append (whclause);
      // sort descending
      String orderByClause = getOrderByClause (otherAv, true);
      qStr.append (orderByClause);

      System.out.println (" NMDB:"+qStr.toString ());
       NetworkMonitorServer.nmlogger.info ("NMDB:"+qStr.toString ());
      ResultProcessor rp = new ResultProcessor ()
	  {
	      Attribute searchAttr = Top.ID;
	      // called for every record in ResultSet
	      public void processRecord (ResultSet rs) throws SQLException
	      {
		  String ofqn = rs.getString (searchAttr.toString ());
		  ol.add (searchHome.getInstance (ofqn));
	      }
	  };
      DbUtil.executeQuery (conn, qStr.toString (), rp);
      return ol;
   }

}                
