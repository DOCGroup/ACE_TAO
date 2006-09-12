//$Id: ServiceClassBean.java
/*
 * ServiceClassBean.java - bean class for ServiceClass
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
 * This is bean class for ServiceClass
 *
 */

class ServiceClassBean extends TopBean implements ServiceClass
{
   ServiceClassBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (ServiceClass.allAttr);
   }

}
