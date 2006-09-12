//$Id: L2HopsHome.java
/*
 * L2HopsHome.java - Home class for L2Hops
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
 * This home class for L2Hops
 *
 */

public class L2HopsHome extends TopHome
{
   /**
    * Constructor
    */
   public L2HopsHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L2Hops");
      homeBean = new L2HopsBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L2HopsBean (getConnection (), getTable (), id);
   }

   public L2Hops narrow (Top t)
   {
      return (L2Hops) t;
   }
}
