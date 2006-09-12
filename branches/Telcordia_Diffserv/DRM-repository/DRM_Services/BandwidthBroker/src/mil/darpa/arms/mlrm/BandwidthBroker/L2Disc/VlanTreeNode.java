package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;
import java.util.*;

/***
 * Represents a VlanTreeNode
 *
 */
public class VlanTreeNode {

    /**
     * Construct a new node with the given value & no children 
     */
    VlanTreeNode(String _name, String _parentPtp) {
	name = _name;
	parentPtp = _parentPtp;

	children = new ArrayList();
	childPtp = new ArrayList();
	numChildren = 0;
    }

    /** Returns a string representation of this node */
    public String toString() {
	if (parentPtp != null)
	    return(parentPtp + "-> " + name + " Up: " + up.toString() + ", Down: " + down.toString());
	return(name);
    }

    /** Node name - this represents the switch ID or name */
    String name;

    /** This is the PTP ID that connects to the parent */
    String parentPtp;

    /** list of children */
    ArrayList children;

    /** list of PTP Ids that connect to the children */
    ArrayList childPtp;

    /** Path to the Root */
    ArrayList up;

    /** Path from the Root */
    ArrayList down;

    /** Number of children */
    int numChildren;

}
