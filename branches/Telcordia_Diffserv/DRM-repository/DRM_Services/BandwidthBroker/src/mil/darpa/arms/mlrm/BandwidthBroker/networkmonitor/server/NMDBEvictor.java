//$Id$
/*
 * NMDBEvictor.java - garbage collection of the incore DB
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
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.TimerTask;

public class NMDBEvictor extends TimerTask
{
    private NMDB _nmdb=null;
    private long _purgeTimePeriod = 1800000; // in milliseconds
   
    public NMDBEvictor (NMDB db, long purgeTimePeriod) throws CDBException 
    {
	_nmdb = db;
	_purgeTimePeriod = purgeTimePeriod;
    }

    /** At every timeout, remove entries with timestamp older than <purgeTimePeriod>.*/
    public void run ()
    {
	NetworkMonitorServer.nmlogger.info ("NMDB Evictor invoked!");
	long timestamp = System.currentTimeMillis () - _purgeTimePeriod; 
	try {
	    _nmdb.garbageCollection (timestamp);
	}catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("NMDB exception, could not purge"+
						    " entries older than:"+
						    new Long (timestamp).toString ());
	    }
    }
}
