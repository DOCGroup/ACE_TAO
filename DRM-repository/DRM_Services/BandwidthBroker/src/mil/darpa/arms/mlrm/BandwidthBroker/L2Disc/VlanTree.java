package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

/****
 *
 * Tree methods provide access to an n-ary tree structure.  
 * The insertChild method adds a new node.  Non-destructive
 * access methods are findSubtree, findLeaf, getChild, getIndexOfChild,
 * getChildCount, and getRoot.  The isLeaf method indicates a tree node is a
 * leaf.
 *
 *
 */

import java.lang.*;
import java.util.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

public class VlanTree {

    /** Tree root */
    VlanTreeNode root;

    /** Vlan ID of this tree */
    int vlanID;

    /** NRDB Instance */
    NRDB db;

    /** Hashtable of all nodes indexed on name (switch ID) */
    Hashtable allNodes = new Hashtable();

    static boolean DEBUG = true;

    /**
     * Construct a tree with the given VlanTreeNode as root.
     */
    public VlanTree (VlanTreeNode root, int _vlanID) {
	this.root = (VlanTreeNode) root;
	this.vlanID = _vlanID;
    }

    /**
     *  Add the given node as a child to the parent node  
     */
    public boolean addChild (VlanTreeNode parent, VlanTreeNode child, String childPtp) {

	int i;

	if (parent == null) 
	    return false;

	parent.children.add(child);
	parent.childPtp.add(childPtp);
	parent.numChildren = parent.children.size();

	return true;


    }

    /**
     * Return the ith child of the given parent.  
     */
    public VlanTreeNode getChild(VlanTreeNode parent, int i) {

	/*
	 * Return null if parent is null, parent has no children, or i is out
	 * of range.
	 */
	if ((parent == null) || (i < 0) || (i >= parent.numChildren)) {
	    return null;
	}

	return ((VlanTreeNode) parent.children.get(i));

    }


    /**
     * Return the root of this tree.
     */
    public VlanTreeNode getRoot() {
	return root;
    }

    /**
     * Return true if given node is a leaf.
     */
    public boolean isLeaf(VlanTreeNode node) {
	if (node == null)
	    return false;
	return (node.numChildren == 0);
    }


    /**
     * Return an indented preorder traversal.
     */
    public String toString() {
	return ("VLAN TREE for VLAN ID: "+ vlanID + "\n" + toString(root, 0));
    }

    /**
     * Recursive work doer for toString.
     */
    public String toString(VlanTreeNode t, int indent) {

	VlanTreeNode child;
	String result = "";

	if (t == null) {
	    return "";
	}

	// for (int j = 0; j < indent; j++) result += " ";
	if (t.name != null) {
	    result += t.toString() + "\n";
	}

	for (int i = 0; i < t.numChildren; i++) {
	    child = (VlanTreeNode) t.children.get(i);
	    for (int j = 0; j < indent + 3; j++) result += " ";
	    result += "<-" + (String) t.childPtp.get(i) + "-";
	    result += toString(child, indent + 3);
	}
	return result;
    }


    /** gets a path between two switches */
    public String[] getPath(String sw1, String sw2) {

	VlanTreeNode node1, node2;
	ArrayList path = new ArrayList();

	if (DEBUG)
          System.out.print("VlanTree::getPath(): ");


	node1 = (VlanTreeNode) this.allNodes.get(sw1);
	node2 = (VlanTreeNode) this.allNodes.get(sw2);

	if (node1 == null || node2 == null) {

	    System.out.println("No path exits between " +sw1+ " and "
				+sw2+" for vlan "+vlanID);
	    return null;
	}


	// is node1 root
	if (node1 == this.getRoot()) {

	    // the path is represented by node 2's down path
	    if (DEBUG)
	        System.out.println(node2.down.toString());
	    return ((String[]) node2.down.toArray(new String[] {}));

	}


	// is node2 root
	if (node2 == this.getRoot()) {

	    // the path is node1's up path
	    if (DEBUG)
	        System.out.println(node1.up.toString());
	    return ((String[]) node1.up.toArray(new String[] {}));

	}


	// is node2 on node1's path to root - i.e. node2.up is a substring of node1.up
	if (node1.up.toString().regionMatches(false,0,node2.up.toString(),0,node2.up.toString().length()-1)) {

	    // subtract node2.up from node1.up
	    for (int i=node2.up.size(); i<node1.up.size(); i++) 
		path.add(node1.up.get(i));

	    if (DEBUG)
	        System.out.println(path.toString());
	    return ((String[]) path.toArray(new String[] {}));
	}

	// is node1 on node2's path to root - i.e. node1.down is a substring of node2.down
	if (node2.down.toString().regionMatches(false,0,node1.down.toString(),0,node1.down.toString().length()-1)) {

	    // subtract node1.down from node2.down
	    for (int i=node1.down.size(); i<node2.down.size(); i++) 
		path.add(node2.down.get(i));

	    if (DEBUG)
	        System.out.println(path.toString());
	    return ((String[]) path.toArray(new String[] {}));
	}

	// no roots or substrings - take unique parts of node 1's up and node 2's down to
        // construct the PTP id path
	for (int i=0; i<node1.up.size(); i++) {

	    if (i < node2.up.size()) {
		if (!node2.up.contains(node1.up.get(i)))
		    path.add(node1.up.get(i));
	    }
	    else path.add(node1.up.get(i));
	  
	}

	for (int i=0; i<node2.down.size(); i++) {

	    if (i < node1.down.size()) {
		if (!node1.down.contains(node2.down.get(i)))
		    path.add(node2.down.get(i));
	    }
	    else path.add(node2.down.get(i));
	  
	}
        if (DEBUG)
	    System.out.println(path.toString());
	return ((String[]) path.toArray(new String[] {}));

    }


    /** Builds a tree starting with a root value */
    public static VlanTree buildTree(String val, int vlanID, NRDB db) {

	try {
	    ArrayList up = new ArrayList();
	    ArrayList down = new ArrayList();

	    VlanTreeNode root = new VlanTreeNode(val, null);
	    VlanTree tree = new VlanTree(root, vlanID);
	    tree.allNodes.put(val, root);
	    tree.db = db;
	    tree.buildSubTree(root, null, up, down);
	    return tree;
	} catch (CDBException cdbe) {
	    cdbe.printStackTrace();
	    return null;
	} 

    }

    /** recursively builds a tree for this vlanID */
    public void buildSubTree(VlanTreeNode node, VlanTreeNode parent,
				ArrayList up, ArrayList down) throws CDBException {

	Hashtable ht;

	if (parent == null)
	    ht = db.getConnectedVlanLinks(node.name, null, vlanID);
	else ht = db.getConnectedVlanLinks(node.name, parent.name, vlanID); 

	if (ht.size() == 0)
	  return;

	Enumeration localPtps = ht.keys();
	Enumeration remotePtps = ht.elements();

	while (remotePtps.hasMoreElements()) {
	    String remPtp = (String) remotePtps.nextElement();
	    String locPtp = (String) localPtps.nextElement();
            String name = db.getSwitchFromPtp(remPtp);
	    VlanTreeNode child = new VlanTreeNode(name, remPtp);
	    allNodes.put(name, child);
	    addChild(node, child, locPtp);
	    ArrayList childDown = (ArrayList) down.clone();
	    ArrayList childUp = (ArrayList) up.clone();
	    childDown.add(remPtp);
	    // childUp.add(0, remPtp);
	    childUp.add(locPtp);
	    child.up = (ArrayList) childUp.clone();
	    child.down = (ArrayList) childDown.clone();
	    buildSubTree(child, node, childUp, childDown);
	}


    }


    /** Checks to see if this VlanTree is complete, a tree is complete
	if all switches that are members of this VLAN are represented
	in the VlanTree */
    public void isTreeComplete() throws CDBException {

	String[] sw = db.getAllMemberSwitches(vlanID);

	for (int i=0; i<sw.length; i++) {
	    if (!allNodes.containsKey(sw[i]))
		System.out.println("Warning: VlanTree does not contain member switch: " +sw[i]);
	}

    }

}
