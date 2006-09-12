//$Id: L3InterfaceBean
/*
 * L3InterfaceBean.java - bean class for L3Interfaces
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
 * This is bean class for L3Interfaces
 *
 */

class L3InterfaceBean extends ManagedObjectBean implements L3Interface
{
   L3InterfaceBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (L3Interface.allAttr);
   }

}
