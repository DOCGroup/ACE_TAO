//$Id: L2PathBean.java
/*
 * L2PathBean.java - bean class for L2Path
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
 * This bean class for L2Path
 *
 */

class L2PathBean extends TopBean implements L2Path
{
   L2PathBean (java.sql.Connection c, String table, String fqn)
   {
      super (c, table, fqn);

      addToAllAttributes (L2Path.allAttr);
  
   }
}
