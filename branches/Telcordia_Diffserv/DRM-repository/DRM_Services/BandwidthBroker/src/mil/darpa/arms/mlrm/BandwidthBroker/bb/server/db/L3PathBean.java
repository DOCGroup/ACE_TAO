//$Id: L3PathBean.java
/*
 * L3PathBean.java - bean class for L3Path
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
 * This bean class for L3Path
 *
 */

class L3PathBean extends TopBean implements L3Path
{
   L3PathBean (java.sql.Connection c, String table, String fqn)
   {
      super (c, table, fqn);

      addToAllAttributes (L3Path.allAttr);
  
   }
}
