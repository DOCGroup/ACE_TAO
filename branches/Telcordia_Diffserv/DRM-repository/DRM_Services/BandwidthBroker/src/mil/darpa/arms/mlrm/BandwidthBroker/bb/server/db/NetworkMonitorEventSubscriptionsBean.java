//$Id$
/*
 * NetworkMonitorEventSubscriptionsBean.java - The network event subscription information
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
 * This is bean class for NetworkMonitorEventSubscriptions
 *
 */

class NetworkMonitorEventSubscriptionsBean extends TopBean implements NetworkMonitorEventSubscriptions
{
   NetworkMonitorEventSubscriptionsBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (NetworkMonitorEventSubscriptions.allAttr);
   }

}
