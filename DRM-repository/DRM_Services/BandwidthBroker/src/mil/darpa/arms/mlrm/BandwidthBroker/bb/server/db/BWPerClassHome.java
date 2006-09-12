//$Id: BWPerClassHome.java
 /*
 * BWPerClassHome.java - Home class for BWPerClass
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
 * This home class for BWPerClass
 *
 */

public class BWPerClassHome extends TopHome
{
   /**
    * Constructor
    */
   public BWPerClassHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "BWPerClass");
      homeBean = new BWPerClassBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new BWPerClassBean (getConnection (), getTable (), id);
   }

   public BWPerClass narrow (Top t)
   {
      return (BWPerClass) t;
   }
}
