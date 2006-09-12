//$Id: HostSubnetHome.java
/*
 * HostSubnetHome.java - Home class for HostSubnet
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
 * This home class for HostSubnet
 *
 */

public class HostSubnetHome extends TopHome
{
   /**
    * Constructor
    */
   public HostSubnetHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "HostSubnet");
      homeBean = new HostSubnetBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new HostSubnetBean (getConnection (), getTable (), id);
   }

   public HostSubnet narrow (Top t)
   {
      return (HostSubnet) t;
   }
}
