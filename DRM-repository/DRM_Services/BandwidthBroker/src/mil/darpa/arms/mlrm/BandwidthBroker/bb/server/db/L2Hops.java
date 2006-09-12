//$Id: L2Hops.java,
/*
 * L2Hops.java - The hops for the flow over the L2 switch network
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
 * This class is denotes the hops for a flow over a L2 switch network
 *
 */

public interface L2Hops extends Top
{
    /** 
     * current L2 hop interface
     */
    public static final Attribute attrL2InterfaceID = new NumberAttribute ("l2InterfaceID");
   /**
    * path info
    *
    */
   public static final Attribute attrL2PathID = new NumberAttribute ("l2PathID");
    /**
     * which hop from src to destination 
     */
    public static final Attribute attrHopNumber = new NumberAttribute ("hopNumber");

    static final Attribute []  allAttr = {attrL2InterfaceID, attrL2PathID, attrHopNumber};


}
