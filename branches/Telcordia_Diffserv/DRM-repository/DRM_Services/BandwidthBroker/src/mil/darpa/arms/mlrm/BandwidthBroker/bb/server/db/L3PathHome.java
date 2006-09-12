//$Id: L3PathHome.java
/*
 * L3PathHome.java - Home class for L3 Path
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
 * This home class for L3Path
 *
 */

public class L3PathHome extends TopHome
{
   /**
    * Constructor
    */
   public L3PathHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L3Path");
      homeBean = new L3PathBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L3PathBean (getConnection (), getTable (), id);
   }

   public L3Path narrow (Top t)
   {
      return (L3Path) t;
   }
}






