//$Id$
/*
 * DelFlowsEvictor.java - garbage collection of the "deleted" flow records in DB
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

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
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

public class DelFlowsEvictor extends TimerTask
{
    NRDB nrdb=null;   
    /** At every timeout, remove "deleted" flow record entries.*/
    public void run ()
    {
       try
       {
        nrdb = new NRDB ();
	nrdb.removeDeletedFlowRecords ();
        nrdb.commit ();
        nrdb.releaseConnection ();
        BBServer.bblogger.info ("Deleted Flows Evictor invoked!");
	}catch (CDBException ce)
	    {
		BBServer.bblogger.warn ("CDB exception, could not purge"+
			              " *deleted* flow record entries");
	    }
        BBServer.bblogger.info ("Deleted Flows Evictor finished!");
    }
}
