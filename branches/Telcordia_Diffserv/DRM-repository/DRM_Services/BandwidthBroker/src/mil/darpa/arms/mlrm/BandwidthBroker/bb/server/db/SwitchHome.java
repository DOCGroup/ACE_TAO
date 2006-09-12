//$Id: SwitchHome.java
/*
 * SwitchHome.java - Home class for LSR (label swicthed routers)
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This home class for Switch
 *
 */

public class SwitchHome extends TopHome
{
   /**
    * Constructor
    */
   SwitchHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "Switch");
      homeBean = new SwitchBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new SwitchBean (getConnection (), getTable (), id);
   }

   public Switch narrow (Top t)
   {
      return (Switch) t;
   }
}
