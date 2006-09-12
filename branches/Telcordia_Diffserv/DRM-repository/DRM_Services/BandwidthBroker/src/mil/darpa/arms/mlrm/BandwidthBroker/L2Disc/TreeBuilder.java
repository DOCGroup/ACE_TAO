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

public class TreeBuilder {

    /** maintains a hash table of vlan ID to the VlanTree */
    Hashtable vlanTrees;
    NRDB db;

    static boolean DEBUG = true;

    public TreeBuilder () {
	vlanTrees = new Hashtable();
    }

    /** Fetch all the VLANs in the database, and then build trees 
	around them */
    public void buildAllTrees() {

	try {

	    db = new NRDB();
	    String[] vlans = db.getAllVlans();

	    for (int i=0; i<vlans.length; i++) {
	        String sw = db.getRootSwitch(vlans[i]);
	        VlanTree tree = VlanTree.buildTree(sw, Integer.parseInt(vlans[i]), db);
		tree.isTreeComplete();
	
	        if (DEBUG)
	    	    System.out.println(tree.toString());

	        vlanTrees.put(vlans[i], tree);
	    }
	    db.releaseConnection();
	} catch (CDBException cdbe) {
	    cdbe.printStackTrace();
	}
    }

    /** Resolves and calls the appropriate VlanTree's getPath method to 
        obtain a path between the two specified switches */
    public String[] getPath (String sw1, String sw2, String vlan) 
    {

	VlanTree tree = (VlanTree) vlanTrees.get(vlan);
	if (tree == null) {
	    System.out.println("No Vlan Tree for specified VLAN: "+vlan);
	    return null;
	}
	return (tree.getPath(sw1, sw2));

    }
	

    public static void main(String[] args) { 

	TreeBuilder builder = new TreeBuilder();
	builder.buildAllTrees();
	if (args.length >= 3)
	    builder.getPath(args[0], args[1], args[2]);
	else builder.getPath("1", "2","100");

    }
    



}
