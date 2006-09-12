//$Id: L2InterfaceBean.java
/*
 * L2InterfaceBean.java - bean class for L2Interfaces
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
 * This is bean class for L2Interfaces
 *
 */

class L2InterfaceBean extends ManagedObjectBean implements L2Interface
{
   L2InterfaceBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (CTP.allAttr);
      addToAllAttributes (L2Interface.allAttr);
   }

}
