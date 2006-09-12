//$Id: L2Path.java
/*
 * L2Path.java - The path for the flow over the L2 switch network
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

/**
 * This class is denotes the path for a flow over a L2 switch network
 *
 */

public interface L2Path extends Top
{
    /** 
     * Ingress L3 next hop interface
     */
    public static final Attribute attrIngressHop = new StringAttribute ("ingressIPHop");
   /**
    * Egress L3 interface for next hop
    *
    */
   public static final Attribute attrEgressHop = new StringAttribute ("egressIPHop");
 
    static final Attribute []  allAttr = {attrIngressHop, attrEgressHop};


}
