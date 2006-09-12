//$Id: L3Path.java
/*
 * L3Path.java - The path for the flow over the L3 router network
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
 * This class is denotes the path for a flow over a L3 router network
 *
 */

public interface L3Path extends Top
{
    /** 
     * Ingress L3 next hop interface
     */
    public static final Attribute attrIngressID = new StringAttribute ("ingressID");
   /**
    * Egress L3 interface for next hop
    *
    */
   public static final Attribute attrEgressID = new StringAttribute ("egressID");
 
    static final Attribute []  allAttr = {attrIngressID, attrEgressID};


}
