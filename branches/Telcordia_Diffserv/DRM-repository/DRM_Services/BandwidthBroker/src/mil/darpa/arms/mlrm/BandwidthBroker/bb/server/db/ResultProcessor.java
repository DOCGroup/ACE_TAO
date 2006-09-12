//$Id: ResultProcessor.java
/*
 * ResultProcessor.java - an interface 
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

import java.sql.*;

public abstract class ResultProcessor
{
   protected String result = null;
   protected long resultLong = 0;

   public ResultProcessor ()
   {
      init ();
   }
   public abstract void processRecord (ResultSet rs) throws SQLException; // called for every record in ResultSet
   
   public String getResult ()
   {
      return result;
   }

   public long getResultLong ()
   {
      return resultLong;
   }

   protected void init () // To be overridden if init required
   {
   }
}
