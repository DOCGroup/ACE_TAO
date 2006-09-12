//$Id: DbUtil.java
/*
 * DBUtil.java -
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
import java.io.*;

public class DbUtil
{

   public static String sqlTraceFile = "sqlTrace";
   public static boolean log = true;

   public static void logString (String l)
   {
      if (log)
      {
         l += "\n";

         try
         {
            FileWriter fw = new FileWriter (sqlTraceFile, true);
            fw.write (l, 0, l.length ());
            fw.close ();
         }
         catch (IOException ie)
         {
	    if (sqlTraceFile.equals ("sqlTrace"))
               System.out.println (l); 
	    else
	    {
               System.out.println ("Changing to ./sqlTrace");
               sqlTraceFile = "sqlTrace";
	       logString (l);
	    }
         }
      }
   }

   public static void executeQuery (Connection conn, String qStr, ResultProcessor rp) throws CDBException
   {
      try
      {
   
         logString (qStr);
         Statement s = conn.createStatement ();

         ResultSet rs = s.executeQuery (qStr);

         while (rs.next () == true)
	 {
            rp.processRecord (rs);
	 }
              
         rs.close ();
         s.close ();
   
      }
      catch (SQLException se)
      {
         System.out.println (qStr + "\nSQL Exception" + se.getMessage ());
         throw new CDBException ();
      }

   }

   public static int executeUpdate (Connection conn, String qStr, boolean commit) throws CDBException
   {
      int rows;
      try
      {
         logString (qStr);
         Statement s = conn.createStatement ();

         rows = s.executeUpdate (qStr);

         if (commit)
            conn.commit();

         s.close ();
         return rows;
   
      }
      catch (SQLException se)
      {
         System.out.println (qStr);
         System.out.println ("SQL Exception:" + se.getMessage());
         throw new CDBException ();
      }

   }

     public static boolean execute (Connection conn, String qStr, boolean commit) throws CDBException
   {
      int rows;
      try
      {
         logString (qStr);
         Statement s = conn.createStatement ();

         boolean b = s.execute (qStr);

         if (commit)
            conn.commit();

         s.close ();
         return b;
   
      }
      catch (SQLException se)
      {
         System.out.println (qStr);
         System.out.println ("SQL Exception:" + se.getMessage());
         throw new CDBException ();
      }

   }
}
