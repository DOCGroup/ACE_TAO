//$Id$
/*
 * NMDB.java - setting up the incore DB
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;

public class NMDB
{
    private static HSQLDBHome hsqldbHome = null;
    private static MeasurementHome measurementHome = null;
   
    public NMDB () throws CDBException 
    {
	if (hsqldbHome == null)
	    {
		hsqldbHome = new HSQLDBHome ("nm_file", "sa", "");
		
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
		    //ex2.printStackTrace();  // second time we run program; HSQL bug
		    //  should throw execption since table
		    // already there
		    //
		    // this will have no effect on the db
		}
	    }
    }

     public static String populateMeasurementInDB  (String probeID, 
						    String probeTime,
						    String delay,
						    String jitter,
						    String loss,
						    String tos)
	throws CDBException
    {
	measurementHome = hsqldbHome.getMeasurementHome ();
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (Measurement.attrProbeID, probeID);
	    av.addNameValue (Measurement.attrProbeTime, probeTime);
	    av.addNameValue (Measurement.attrDelay, delay);
	    av.addNameValue (Measurement.attrJitter, jitter);
	    av.addNameValue (Measurement.attrLoss, loss);
 	    av.addNameValue (Measurement.attrDSCP, tos);
	    Top t= measurementHome.createRecord (av);
	    Measurement measurement = measurementHome.narrow (t);
	    av = measurement.getAttributes ();
	    av.print ();
	    return measurement.getID();
	}
	catch (Exception e)
	    {
		e.printStackTrace ();
		throw new CDBException ();
	    }
    }

    public static ObjectList findMeasurementByProbeIDAndTimestamp (String probeID, 
								   long timeStamp,
								   boolean greaterThanEqualTo)
	throws CDBException
    {
	AttributeValues av1= new AttributeValues ();
	AttributeValues av2= new AttributeValues ();
	
	av1.addNameValue (Measurement.attrProbeID, probeID);
	av2.addNameValue (Measurement.attrProbeTime, new Long (timeStamp).toString ());
	return measurementHome.findSpecificByAttributes (av1, 
					 av2, 
					 true, 
					 measurementHome);
    }

    public void garbageCollection (long timestamp)
	throws CDBException
    {
	measurementHome = hsqldbHome.getMeasurementHome ();

	ObjectList ol=null;
	AttributeValues av1= new AttributeValues ();
	av1.addNameValue (Measurement.attrProbeTime, new Long (timestamp).toString ());

	// Get all records less than equal to timestamp
	ol =  measurementHome.findSpecificByAttributes (new AttributeValues (), 
							av1, 
							false, 
							measurementHome);
	if (ol==null || ol.size ()==0)
	    {
		NetworkMonitorServer.nmlogger.info ("NMDB garbage collection: No"+
						    " entries with timestamp <="+ 
						    new Long (timestamp).toString ());
		return;
	    }
	// Purge these records
	AttributeValues av=null;
	for (int i=0; i < ol.size (); i++)
	    {
		av = ol.get (i).getAttributes ();
		measurementHome.remove (av);
	    }
	NetworkMonitorServer.nmlogger.info ("NMDB garbage collection done of "+
					    new Integer (ol.size()).toString()
					    + " entries with timestamp less than equal to"+ 
					    new Long (timestamp).toString ());
	return;
    }

    public static void printMeasurementRecord (String ID)
	throws CDBException 
    {
	try {
	Top t = measurementHome.findByID (ID);
	System.out.println ("Found by ID: "+ID+
			    " the following measurement table entry:");
	Measurement measurement = measurementHome.narrow (t);
	(measurement.getAttributes ()).print();
      }catch (CDBException e)
	    {
		e.printStackTrace ();
		throw e;
	    }
	return;
    }

}
