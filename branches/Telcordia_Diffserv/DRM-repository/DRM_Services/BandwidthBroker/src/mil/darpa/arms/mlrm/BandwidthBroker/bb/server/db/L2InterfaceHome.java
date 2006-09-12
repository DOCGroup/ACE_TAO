//$Id: L2InterfaceHome.java
/*
 * L2InterfaceHome.java - Home class for LSR (label swicthed routers)
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
 * This home class for L2Interface
 *
 */

public class L2InterfaceHome extends TopHome
{
   /**
    * Constructor
    */
   public L2InterfaceHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L2Interface");
      homeBean = new L2InterfaceBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L2InterfaceBean (getConnection (), getTable (), id);
   }

   public L2Interface narrow (Top t)
   {
      return (L2Interface) t;
   }
}






