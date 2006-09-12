//$Id$
/*
 * HSQLDBHome.java - Home class accessing HSQLDB
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

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;


import java.sql.*;

/**
 * This home class for HSQLDB. It provides transactional interface for 
 * accessing the common data base
 *
 */

public class HSQLDBHome 
{
   static boolean jdbcInit = false;
   Connection c = null;
   boolean inUse = false;

   /**
    * get a Database connection to HSQLDBHome for a given user given
    * the dbFileNamePrefix which denotes the database files prefix for
    * HSQLDB
    * 
    */
   public HSQLDBHome (String dbFileNamePrefix, String user, String password) throws CDBException
   {
      try
      {
         c = connect (dbFileNamePrefix, user, password);
         System.out.println ("Connected to HSLDB as " + user + "with db: " + dbFileNamePrefix);
      }
      catch (SQLException se)

      {
         System.out.println ("Error connecting to HSQLDB");
         throw new CDBException ();
      }
      
   }

   public static void setLogging (boolean logFlag)
   {
      DbUtil.log = logFlag;
   }

   public TopHome getHomebyName (String table) throws CDBException
   {
      
      if (table.equals ("MEASUREMENT"))
         return getMeasurementHome ();
      else
	  throw new CDBException ();
      
   }

    public MeasurementHome getMeasurementHome ()
    {
	return new MeasurementHome (c);
    }
    
    public void resetDB () throws CDBException
    {
	AttributeValues av = new AttributeValues();
	av.addNameValue (Top.ID, new String ("> 0"));
	MeasurementHome measurementHome = getMeasurementHome ();
	measurementHome.remove (av);
    }

   private Connection connect (String dbFileNamePrefix, String user, String password) throws SQLException
   {
      Connection c;

      if (jdbcInit == false)
      {
      
	  /**
	   * Load the JDBC Driver
	   */
	  try{
	  Class.forName ("org.hsqldb.jdbcDriver").newInstance();
	  }
	  catch (Exception ex) {
	      ex.printStackTrace();
	      throw new SQLException ("Unable to load driver.");
	  }
	  jdbcInit = true;
      }

      /**
       * make a JDBC connection
       */
      try{
	  
	  /* c = DriverManager.getConnection ("jdbc:mysql://"+host+"/ARMSBB" + 
					   "?user=" + user +
					   "&password=" + password);*/
	  c = DriverManager.getConnection ("jdbc:hsqldb:mem:"+dbFileNamePrefix, //"jdbc:hsqldb:"+dbFileNamePrefix, 
					    user,
					    password);
      }
      catch(Exception ex){
	  ex.printStackTrace();
        throw new SQLException("Unable to connect");
      }
      c.setAutoCommit(false);      

      return c;
      
   }

     /*
    * commit changes to database;
    *
    */
   public void commit () throws CDBException
   {
      try
      {
         c.commit ();
      }
      catch (SQLException se)
      {
         throw new CDBException ();
      }
   }

    //use for SQL commands CREATE, DROP, INSERT and UPDATE
    public synchronized void update(String expression) throws SQLException {

        Statement st = null;

        st = c.createStatement();    // statements

        int i = st.executeUpdate(expression);    // run the query

        if (i == -1) {
            System.out.println("db error : " + expression);
        }

        st.close();
    }    // void update()


    // For testing mainly : Kirthika
    public static void main (String[] args)
    {
	
	try{
	    
	    HSQLDBHome hsqldbHome = new HSQLDBHome ("nm_file", "sa", "");

	    System.out.println ("create DB first");
	    try {

            //make an empty table
            //
            // by declaring the id column IDENTITY, the db will automatically
            // generate unique values for new rows- useful for row keys
		hsqldbHome.update(
			  "CREATE TABLE MEASUREMENT ( ID INTEGER IDENTITY NOT NULL, "+
			  "probeID VARCHAR (1024), probeTime VARCHAR (1024),"+
			  "delay BIGINT, jitter BIGINT, loss BIGINT, dscp INT,PRIMARY KEY (ID))");
	    } catch (SQLException ex2) {

            //ignore
            //ex2.printStackTrace();  // second time we run program
            //  should throw execption since table
            // already there
            //
            // this will have no effect on the db
	    }
	    
	    //Insert records and retrieve them for testing

	    // measurement test
	    MeasurementHome measurementHome = hsqldbHome.getMeasurementHome ();
	    System.out.println ("Table " + measurementHome.getTable ());
	    String measurementID = hsqldbHome.testMeasurementAdd (measurementHome);
	    System.out.println ("measurementID="+measurementID);
	    // Find test
	    Top t = measurementHome.findByID (measurementID);
	    System.out.println ("Found by ID: "+measurementID+
				" the following measurement table entry:");
	    Measurement measurement = measurementHome.narrow (t);
	    (measurement.getAttributes ()).print();
	   
	    // test Measurement Delete 
	    measurementHome.remove (measurementID);
	    System.out.println ("Measurement name: firstMeasurement ID:"+
	    measurementID+ " has been removed!");
	    hsqldbHome.commit();
	}
	catch (Exception e)
	    {
		System.out.println ("Exception!");
		e.printStackTrace ();
	    }
    }
      
    public String testMeasurementAdd (MeasurementHome measurementHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (Measurement.attrProbeID, new String ("probe_1"));
	    av.addNameValue (Measurement.attrProbeTime, new String ("123456"));
	    av.addNameValue (Measurement.attrDelay, new String ("3200"));
	    av.addNameValue (Measurement.attrJitter, new String ("0"));
	    av.addNameValue (Measurement.attrLoss, new String ("0"));
 	    av.addNameValue (Measurement.attrDSCP, new String ("24"));
	    Top t= measurementHome.createRecord (av);
	    Measurement measurement = measurementHome.narrow (t);
	    av = measurement.getAttributes ();
	    av.print ();
	    return measurement.getID();
	}
	catch (Exception e)
	    {
		e.printStackTrace ();
		throw e;
	    }
    }
  
  
}
