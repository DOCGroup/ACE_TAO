package mil.darpa.arms.mlrm.BandwidthBroker.ospf;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import java.util.*;

public class FindPaths
{
   public static Hashtable oldPaths = new Hashtable ();
   public static Hashtable curPaths = new Hashtable ();

   public static void loadL3Paths (NRDB nrdb, String router) throws CDBException, Exception
   {
      loadL3Paths (nrdb, router, false);
   }
   public static void loadL3Paths (NRDB nrdb, String router, boolean inMemory) throws CDBException, Exception
   {
      ReadOSPFDatabaseSNMP rodbs = new ReadOSPFDatabaseSNMP();
      rodbs.read (router);
      int numRouters = Router.allRouters.size ();

      nrdb.makeL3PathsOld ();
      String routerID [] = new String [numRouters];
      for (int i = 0; i < numRouters; i++)
      {
	 String rName = ((Router)Router.allRouters.get (i)).name;
         routerID [i] = nrdb.routerIPtoID (rName);
      }

      for (int i = 0; i < numRouters; i++)
      {
	 Router source = (Router) Router.allRouters.get (i);
         for (int j = 0; j < numRouters; j++)
	 {
	    if (j == i)
	       continue;
	    Router dest = (Router) Router.allRouters.get (j);
            String [] path = Router.getRoute (source, dest);
	    String key = source+"/"+dest;
	    Object previous = curPaths.put (key, path);

	    if (previous != null)
	       oldPaths.put (key, previous);
	    if (inMemory == false)
	    {
               nrdb.setEdgeToEdgePath (routerID [i], routerID[j], path);
	       //nrdb.setL3Path (source.name, dest.name, path);
	    }
	 }
      }
      nrdb.commit ();
   }
   
   public static void main (String args []) throws Exception
   {
      System.out.println ("Syntax:- FindPaths <routerIPAddr> <databasehost>");


      NRDB.numConnections = 1;
      NRDB.dbServerFQN = args [1];
      //NRDB.dbLoginName= "arms";
      //NRDB.dbPasswordValue = "arms"; // use default

      System.out.println ("Downloading OSPF database.");
      long start = System.currentTimeMillis ();
      ReadOSPFDatabaseSNMP rodbs = new ReadOSPFDatabaseSNMP();
      rodbs.read(args [0]);
      long end = System.currentTimeMillis ();
      System.out.println ("Time taken to download SNMP data = " + (end - start));
      int numRouters = Router.allRouters.size ();

      NRDB nrdb = new NRDB ();
      nrdb.makeL3PathsOld ();
      String routerID [] = new String [numRouters];
      for (int i = 0; i < numRouters; i++)
      {
	 String rName = ((Router)Router.allRouters.get (i)).name;
         routerID [i] = nrdb.routerIPtoID (rName);
      }

      for (int i = 0; i < numRouters; i++)
      {
	 Router source = (Router) Router.allRouters.get (i);
         for (int j = 0; j < numRouters; j++)
	 {
	    if (j == i)
	       continue;
	    Router dest = (Router) Router.allRouters.get (j);
            String [] path = Router.getRoute (source, dest);


            nrdb.setEdgeToEdgePath (routerID [i], routerID[j], path);
	    //nrdb.setL3Path (source.name, dest.name, path);
	    /*System.out.println (source.name + "->" + dest.name);
	    for (int k = 0; k < path.length; k++)
	       System.out.print (path [k] + ",");
	    System.out.println ("");
	    */
	 }
      System.out.println ("Time taken to commit data = " + (System.currentTimeMillis () - end));
      }
      nrdb.commit ();
   }
}

