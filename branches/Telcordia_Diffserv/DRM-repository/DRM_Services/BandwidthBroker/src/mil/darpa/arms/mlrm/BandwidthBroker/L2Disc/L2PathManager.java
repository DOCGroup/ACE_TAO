package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

/****
 *
 * The methods in this class enable the creation of tree 
 * structures for each VLAN and their maintenance.
 *
 */

import java.lang.*;
import java.util.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

public class L2PathManager {

    static VlanDisc disc = null;
    static TreeBuilder current = null;
    static TreeBuilder last = null;

    static NRDB db = null;
  
    /** variable to indicate whether L2Disc will actually go to the network */
    static boolean loopback = false;

    static boolean inited = false;

    static boolean DEBUG = true;


    /** Initialization, kick of VLAN discovery, after it completes
        build all the VLAN trees */
    public static void init (boolean _loopback) 
    {
      inited = true;
      loopback = _loopback;


      if (!loopback) {
        System.out.println("Invoking Vlan Discovery");
	disc = new VlanDisc();
	disc.getVlanInfo();
      }

      System.out.println("Invoking Vlan Tree computation");
      current = new TreeBuilder();
      current.buildAllTrees();
    }



    /** call the getPath of the TreeBuilder class, return a string of PTP ID's 
        or null on failure */
    public static String[] getPath (String switchA, String switchZ, String vlan) 
    {

      // ensure that the class has inited 
      if (!inited) L2PathManager.init(loopback);

      return (current.getPath(switchA, switchZ, vlan));

    }

    /** Some network event, linkUP, linkDOWN or switch UP/DOWN occured,
        This may result in changed VLAN trees etc. At this point,
        we don't even care what the exact network event it */
    public static void networkEvent(String port, String status) 
    {

      if (!loopback) {
        System.out.println("Invoking Vlan Discovery on Network Event");
	disc.getVlanInfo();
      }

      // first save the last network snapshot
      last = current;
      System.out.println("Invoking new Vlan Tree computation on Network Event");
      current = new TreeBuilder();
      current.buildAllTrees();
    }


    /** Compares the current and old versions of the paths and returns
        true if they are an exact match, false if they do not match */
    public static boolean comparePaths(String switchA, String switchZ, String vlan)
    {
	if (current == null || last == null)
          return true;

	String[] curPath = current.getPath(switchA, switchZ, vlan);
	String[] lastPath = last.getPath(switchA, switchZ, vlan);

	if (curPath.length != lastPath.length)
	  return false;

	for (int i=0; i<curPath.length; i++) {
          if (!curPath[i].equals(lastPath[i]))
            return false;
        }

        return true;

    }

    /** Get the snapshot of the old path before the last network change */
    public static String[] getOldPath (String switchA, String switchZ, String vlan) 
    {
        return (last.getPath(switchA, switchZ, vlan));
    }
      
    public static void main(String[] args)
	{
		System.out.println("About to start");
		L2PathManager.init(true);
		if (args.length == 3)
		  L2PathManager.getPath(args[0], args[1], args[2]);
		else L2PathManager.getPath("1", "4", "100"); 


		L2PathManager.networkEvent("blah", "blah");
		System.out.println("Old and new paths are: " + L2PathManager.comparePaths("1", "4", "100")); 
		System.out.println("Exiting");
	}    




}
