//$Id: L3Hops.java
/*
 * L3Hops.java - The hops for the flow over the L3 switch network
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
 * This class is denotes the hops for a flow over a L3 switch network
 *
 */

public interface L3Hops extends Top
{
    /** 
     * current L3 hop interface
     */
    public static final Attribute attrL3InterfaceID = new NumberAttribute ("l3InterfaceID");
   /**
    * path info
    *
    */
   public static final Attribute attrL3PathID = new NumberAttribute ("l3PathID");
   public static final Attribute attrLinkID = new NumberAttribute ("linkID");
    /**
     * which hop from src to destination 
     */
    public static final Attribute attrHopNumber = new NumberAttribute ("hopNumber");
    public static final Attribute attrIsOldPath = new NumberAttribute ("isOldPath");

    static final Attribute []  allAttr = {attrL3InterfaceID, attrL3PathID, attrLinkID,
                                          attrHopNumber, attrIsOldPath};


}
