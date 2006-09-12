//$Id: L2PathHome.java
/*
 * L2PathHome.java - Home class for L2 Path
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
 * This home class for L2Path
 *
 */

public class L2PathHome extends TopHome
{
   /**
    * Constructor
    */
   public L2PathHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L2Path");
      homeBean = new L2PathBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L2PathBean (getConnection (), getTable (), id);
   }

   public L2Path narrow (Top t)
   {
      return (L2Path) t;
   }
}






