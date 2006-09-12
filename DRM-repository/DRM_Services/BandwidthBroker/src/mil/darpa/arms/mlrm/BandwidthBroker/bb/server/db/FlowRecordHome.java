//$Id: FlowRecordHome.java,
/*
 * FlowRecordHome.java - Home class for FlowRecord
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
 * This home class for FlowRecord
 *
 */

public class FlowRecordHome extends TopHome
{
   /**
    * Constructor
    */
   public FlowRecordHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "FlowRecord");
      homeBean = new FlowRecordBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new FlowRecordBean (getConnection (), getTable (), id);
   }

   public FlowRecord narrow (Top t)
   {
      return (FlowRecord) t;
   }
} 
