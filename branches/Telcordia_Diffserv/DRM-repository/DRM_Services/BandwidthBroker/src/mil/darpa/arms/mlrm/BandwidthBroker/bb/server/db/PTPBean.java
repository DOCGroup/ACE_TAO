//$Id: PTPBean.java
/*
 * PTPBean.java - bean class for PTP
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
 * This bean class for Physical Termination Point (PTP)
 *
 */

class PTPBean extends ManagedObjectBean implements PTP
{
   PTPBean (java.sql.Connection c, String table, String fqn)
   {
      super (c, table, fqn);

      addToAllAttributes (PTP.allAttr);
      // create attributeList
      // attributes of Managed Object
      /*
      allAttributes.add (adminState);
      allAttributes.add (containedInNetwork);
      allAttributes.add (opnState);
      */

   }
}
