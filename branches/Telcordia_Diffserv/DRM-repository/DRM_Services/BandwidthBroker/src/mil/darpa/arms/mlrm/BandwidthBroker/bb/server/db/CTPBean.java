//$Id: CTPBean.java
/*
 * CTPBean.java - bean class for CTP
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
 * This bean class for Connection Termination Point (CTP)
 *
 */

abstract class CTPBean extends ManagedObjectBean implements CTP
{
   CTPBean (java.sql.Connection c, String table, String fqn)
   {
      super (c, table, fqn);

      addToAllAttributes (CTP.allAttr);
      // create attributeList
      // attributes of Managed Object
      /*
      allAttributes.add (adminState);
      allAttributes.add (containedInNetwork);
      allAttributes.add (opnState);
      */

   }
}
