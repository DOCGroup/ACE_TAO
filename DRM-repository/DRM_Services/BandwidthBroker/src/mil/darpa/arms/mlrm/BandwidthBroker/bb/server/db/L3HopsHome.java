//$Id: L3HopsHome.java
/*
 * L3HopsHome.java - Home class for L3Hops
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
 * This home class for L3Hops
 *
 */

public class L3HopsHome extends TopHome
{
   /**
    * Constructor
    */
   public L3HopsHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L3Hops");
      homeBean = new L3HopsBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L3HopsBean (getConnection (), getTable (), id);
   }

   public L3Hops narrow (Top t)
   {
      return (L3Hops) t;
   }
}
