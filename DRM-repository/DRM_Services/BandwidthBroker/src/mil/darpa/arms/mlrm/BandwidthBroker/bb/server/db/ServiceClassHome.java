//$Id: ServiceClassHome.java
 /*
 * ServiceClassHome.java - Home class for ServiceClass
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
 * This home class for ServiceClass
 *
 */

public class ServiceClassHome extends TopHome
{
   /**
    * Constructor
    */
   public ServiceClassHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "ServiceClass");
      homeBean = new ServiceClassBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new ServiceClassBean (getConnection (), getTable (), id);
   }

   public ServiceClass narrow (Top t)
   {
      return (ServiceClass) t;
   }
}
