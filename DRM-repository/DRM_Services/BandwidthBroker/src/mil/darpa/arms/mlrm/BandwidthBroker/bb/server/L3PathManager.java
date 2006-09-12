// $Id: BBDriver.java
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// BBDriver.java

// Author : Kirthika Parmeswaran, Shrirang Gadgil
// This class drives the BB backend 

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.io.*;
import java.net.*;
import java.util.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import mil.darpa.arms.mlrm.BandwidthBroker.L2Disc.*;
import mil.darpa.arms.mlrm.BandwidthBroker.ospf.*;

import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

/**
 * BBDriver class is instantiated by AdmissionControlServerImpl. While 
 * AdmissionControlClass provides a CORBA facade to bandwidth broker,
 * the main functionality of BB is embedded in this class.
 */
public class L3PathManager
{
   public static void init (String router)
   {
      try
      {
         //FindPaths.loadL3Paths (null, router, true);
      }
      catch (Exception e)
      {
      }
   }

   public static void processFaultBegin ()
   {
      // launch the script to traceroute and find the new paths
      Runtime r = Runtime.getRuntime ();
      try
      {
         Process p = r.exec ("sh bin/recomputeL3paths.sh");
         System.out.println ("Begin fault processing...");
	 p.waitFor ();
      }
      catch (Exception e)
      {
      }
   }

   public static void processFaultEnd ()
   {
      // launch the script to traceroute and find the new paths
      Runtime r = Runtime.getRuntime ();
      try
      {
         Process p = r.exec ("sh bin/removeOldPaths.sh");
         System.out.println ("End fault processing...");
	 p.waitFor ();
      }
      catch (Exception e)
      {
      }
   }
      // make new path current and remove the old path

   public static boolean comparePathsInMemory (String source, String dest)
   {
      String key = source+"/"+dest;
      String [] oldPath = (String []) FindPaths.oldPaths.get (key);
      String [] path = (String []) FindPaths.curPaths.get (key);

      System.out.println ("In comparePaths " + source + ":" + dest);
      if (path != null && oldPath != null)
      {
         if (path.length != oldPath.length)
            return false;
         else
	 {
	    for (int i = 0; i < path.length; i++)
	    {
               if (path [i].equals (oldPath [i]))
	       {
	          System.out.print (path [i] + ":" + oldPath [i] + "-");
	          continue;
	       }
	       else
	          return false;
	    }
	    return true;
	 }
      }
      else if (path == oldPath)
         return true;

      return false;
      
   }
   // returns true if paths are identical
   public static boolean comparePaths (NRDB nrdb, String r1, String r2) 
   				       throws CDBException
   {
      //System.out.println ("In comparePaths " + r1 + ":" + r2);
      PTP [] path = nrdb.getEdgeToEdgePath (null, null, r1, r2, false);
      PTP [] oldPath = nrdb.getEdgeToEdgePath (null, null, r1, r2, true);
      //System.out.println ("Path lengths - " + path.length + ":" + oldPath.length);

      if (path.length != oldPath.length)
         return false;
         
      for (int i = 0; i < path.length; i++)
      {
         if (path [i].getID().equals (oldPath [i].getID ()))
	 {
	   //System.out.print (path [i].getID() + ":" + oldPath [i].getID () + "-");
	   continue;
	 }
	 else
	    return false;
      }
      //System.out.println ("");
      return true; // paths are identical
   }
}
