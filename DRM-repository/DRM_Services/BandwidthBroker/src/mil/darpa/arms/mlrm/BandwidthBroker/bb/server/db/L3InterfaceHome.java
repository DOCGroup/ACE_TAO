//$Id: L3InterfaceHome.java
/*
 * L3InterfaceHome.java - Home class for LSR (label swicthed routers)
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
 * This home class for L3Interface
 *
 */

public class L3InterfaceHome extends TopHome
{
   /**
    * Constructor
    */
   public L3InterfaceHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "L3Interface");
      homeBean = new L3InterfaceBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new L3InterfaceBean (getConnection (), getTable (), id);
   }

   public L3Interface narrow (Top t)
   {
      return (L3Interface) t;
   }
}
