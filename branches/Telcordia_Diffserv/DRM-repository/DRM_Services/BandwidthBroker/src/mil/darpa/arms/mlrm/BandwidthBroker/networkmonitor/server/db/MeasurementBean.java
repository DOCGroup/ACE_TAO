//$Id$
/*
 * MeasurementBean.java - bean class for Measurements
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;


import java.sql.*;


/**
 * This is bean class for Measurements
 *
 */

class MeasurementBean extends TopBean implements Measurement
{
   MeasurementBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (Measurement.allAttr);
      //allAttributes.add (isProviderEdge);
   }

  
}
