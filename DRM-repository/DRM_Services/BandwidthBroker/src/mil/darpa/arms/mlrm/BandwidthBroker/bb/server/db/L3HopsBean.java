//$Id: L3HopsBean.java
/*
 * L3HopsBean.java - bean class for L3Hops
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
 * This is bean class for L3Hops
 *
 */

class L3HopsBean extends TopBean implements L3Hops
{
   L3HopsBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (L3Hops.allAttr);
   }

}
