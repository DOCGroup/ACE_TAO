//$Id: BWPerClass.java,
/*
 * BWPerClass.java - The bandwidth per class for the flow over the L3 switch network
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
 * This class is denotes the bandwidth per class for a flow over a L3 switch network
 *
 */

public interface BWPerClass  extends Top
{
    /** 
     * allocated bandwidth
     */
    public static final Attribute attrBWAlloc = new NumberAttribute ("bwAllocated");
   /**
    * total bandwidth
    *
    */
   public static final Attribute attrBWTotal = new NumberAttribute ("bwTotal");
    /**
     * Service class index
     */
    public static final Attribute attrSvcClassID = new NumberAttribute ("svcClassID");
    /**
     * link bandwidth
     */
    public static final Attribute attrPtpID = new NumberAttribute ("ptpID");

    public static final Attribute attrInbound = new NumberAttribute ("inbound");

    static final Attribute []  allAttr = {attrBWAlloc, attrBWTotal, 
                                          attrSvcClassID, attrPtpID, attrInbound};


}
