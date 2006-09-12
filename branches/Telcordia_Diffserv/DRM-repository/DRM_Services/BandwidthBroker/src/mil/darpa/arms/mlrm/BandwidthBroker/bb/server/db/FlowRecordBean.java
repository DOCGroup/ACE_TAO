//$Id: FlowRecordBean.java
/*
 * FlowRecordBean.java - bean class for FlowRecord
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
 * This is bean class for FlowRecord
 *
 */

class FlowRecordBean extends TopBean implements FlowRecord
{
   FlowRecordBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (FlowRecord.allAttr);
   }

}
