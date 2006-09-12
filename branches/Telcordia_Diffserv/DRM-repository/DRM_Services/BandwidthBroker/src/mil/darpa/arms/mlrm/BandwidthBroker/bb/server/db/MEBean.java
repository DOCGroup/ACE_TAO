//$Id: MEBean.java
/*
 * MEBean.java - bean class for MEs
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
 * This is bean class for ManagedElement
 *
 */

abstract class MEBean extends ManagedObjectBean implements ME
{
   MEBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (ME.allAttr);
   
   }
}
