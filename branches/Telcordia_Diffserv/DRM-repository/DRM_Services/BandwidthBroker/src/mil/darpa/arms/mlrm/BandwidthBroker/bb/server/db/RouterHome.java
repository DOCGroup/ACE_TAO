//$Id: RouterHome.java
/*
 * RouterHome.java - Home class for LSR (label swicthed routers)
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
 * This home class for Router
 *
 */

public class RouterHome extends TopHome
{
   /**
    * Constructor
    */
   RouterHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "Router");
      homeBean = new RouterBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new RouterBean (getConnection (), getTable (), id);
   }

   public Router narrow (Top t)
   {
      return (Router) t;
   }
}
