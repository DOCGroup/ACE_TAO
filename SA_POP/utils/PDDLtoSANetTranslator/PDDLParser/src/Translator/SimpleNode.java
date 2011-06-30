package Translator;

import java.util.HashSet;
import java.util.Set;

public class SimpleNode {
	
	public final String tn;
	
	public final Set<SimpleNode> neighbors = new HashSet<SimpleNode>();

	public SimpleNode(String tn) {
		
		this.tn = tn;
	}

	public static void connect(SimpleNode simpleNode, SimpleNode simpleNode2) {
		simpleNode.neighbors.add(simpleNode2);
		simpleNode2.neighbors.add(simpleNode);
	}

}
