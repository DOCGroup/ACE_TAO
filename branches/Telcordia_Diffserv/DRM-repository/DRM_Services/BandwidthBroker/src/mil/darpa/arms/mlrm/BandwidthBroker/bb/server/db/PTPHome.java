//$Id: PTPHome.java
 /*
 * PTPHome.java - Home class for PTP
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
 * This home class for PTP
 *
 */

public class PTPHome extends TopHome
{
   /**
    * Constructor
    */
   public PTPHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "PTP");
      homeBean = new PTPBean (c, getTable (), null);
   }

  public  Top getInstance (String id)
   {
      return new PTPBean (getConnection (), getTable (), id);
   }

   public PTP narrow (Top t)
   {
      return (PTP) t;
   }
}






