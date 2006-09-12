//$Id$
/*
 * NetworkMonitorEventSubscriptionsHome.java - The network event subscription information
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */
package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;
/**
 * This home class for NetworkMonitorEventSubscriptions
 *
 */

public class NetworkMonitorEventSubscriptionsHome extends TopHome
{
   /**
    * Constructor
    */
   public NetworkMonitorEventSubscriptionsHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "NetworkMonitorEventSubscriptions");
      homeBean = new NetworkMonitorEventSubscriptionsBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new NetworkMonitorEventSubscriptionsBean (getConnection (), getTable (), id);
   }

   public NetworkMonitorEventSubscriptions narrow (Top t)
   {
      return (NetworkMonitorEventSubscriptions) t;
   }
}
