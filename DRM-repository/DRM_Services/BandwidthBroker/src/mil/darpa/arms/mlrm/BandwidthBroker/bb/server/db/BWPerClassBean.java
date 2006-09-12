//$Id: BWPerClassBean.java,
/*
 * BWPerClassBean.java - bean class for BWPerClass
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

/**
 * This is bean class for BWPerClass
 *
 */

class BWPerClassBean extends TopBean implements BWPerClass
{
   BWPerClassBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (BWPerClass.allAttr);
   }

}
