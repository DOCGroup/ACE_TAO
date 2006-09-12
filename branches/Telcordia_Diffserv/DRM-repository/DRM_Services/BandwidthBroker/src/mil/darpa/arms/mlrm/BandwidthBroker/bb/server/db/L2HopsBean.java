//$Id: L2HopsBean.java
/*
 * L2HopsBean.java - bean class for L2Hops
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
 * This is bean class for L2Hops
 *
 */

class L2HopsBean extends TopBean implements L2Hops
{
   L2HopsBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (L2Hops.allAttr);
   }

}
