//$Id: CDBConnectionPool.java,
/*
 * CDBConnectionPool.java - Connection pool
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

/**
 * This home class for CDB. It provides transactional interface for 
 * accessing the common data base
 *
 */

public class CDBConnectionPool
{
   /**
    * constructor
    */

   private CDBHome [] pool;
   String host, userName, pwd;

   public CDBConnectionPool (int numConn, String host, 
                             String userName, String pwd)
   {
      pool = new CDBHome [numConn];
      this.host = host;
      this.userName = userName;
      this.pwd = pwd;
   }

   public synchronized CDBHome allocateCDBHome () throws CDBException
   {
      while (true)
      {
         for (int i = 0; i < pool.length; i++)
         {
            if (pool [i] != null)
            {
               if (pool [i].inUse == false)
               {
                  pool [i].inUse = true;
                  return pool [i];
               }
            }
            else
            {
               pool [i] = new CDBHome (host, userName, pwd);
               pool [i].inUse = true;
               return pool [i];
            }
         }
         try
         {
            wait ();
         }
         catch (Exception e)
         {
         }
      }
   }

   public synchronized void freeCDBHome (CDBHome cdbHome) throws CDBException
   {
      if (cdbHome.inUse = true)
      {
         // to be on safer side
         cdbHome.rollback ();
         cdbHome.inUse = false;
         notify ();
      }
   }
}
