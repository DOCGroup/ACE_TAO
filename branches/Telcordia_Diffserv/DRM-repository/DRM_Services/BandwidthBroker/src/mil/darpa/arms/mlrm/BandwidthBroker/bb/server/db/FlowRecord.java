//$Id: FlowRecord.java
/*
 * FlowRecord.java - The flow information
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Telcordia Drive 
 * Piscataway, NJ 08854-4157
 *
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class denotes the flow information
 *
 */

public interface FlowRecord extends Top
{
    /** 
     * Status of the flow
     */
    public static final Attribute attrStatus = new StringAttribute ("status");

    // committing: bandwidth is reserved but configuration is not complete
    public static final String statusCommitting = "committing";
    // reserved: bandwidth is reserved by application and will be configured later
    public static final String statusReserved = "reserved";
    // raincheck: bandwidth was reserved by application and is no longer available due to network change
     public static final String statusRaincheck = "raincheck";
    // The flows are reserved and committed
    public static final String statusCommitted = "committed";
    // The flow bw has been released from db but configuration is not removed 
    public static final String statusDeleting = "deleting";
    // The flow has been deleted from db and configuration is removed 
    public static final String statusDeleted = "deleted";
    // The "committed" flow bw has been released but not de-configured, the status is not yet "down"
    public static final String statusFailing = "failing";
    // The "committing" flow bw has been released, the status is not yet "down"
    public static final String statusCancelled = "cancelled";
    // preempted flows
    public static final String statusDown = "down";

    /** 
     * Eviction Status of the flow, if a flow is reserved but not
     * comitted with <flowReservationTimeout> seconds, it is deleted
     */
    public static final Attribute purgeStatus = new NumberAttribute ("purgeStatus");


    /** 
     * IP address of flow source
     */
    public static final Attribute attrSrcIP = new StringAttribute ("srcIP");
   /**
    * source subnet mask
    *
    */
   public static final Attribute attrSrcMask = new StringAttribute ("srcMask");
    /**
     * port of source
     */
    public static final Attribute attrSrcPort = new StringAttribute ("srcPort");
     /**
     * port range (high end) of source
     */
    public static final Attribute attrSrcPortHigh = new StringAttribute ("srcPortHigh");
    /** 
     * IP address of flow destination
     */
    public static final Attribute attrDestIP = new StringAttribute ("destIP");
   /**
    * destination subnet mask
    *
    */
   public static final Attribute attrDestMask = new StringAttribute ("destMask");
    /**
     * port of destination
     */
    public static final Attribute attrDestPort = new StringAttribute ("destPort");
     /**
     * port range (high end) of destination
     */
    public static final Attribute attrDestPortHigh = new StringAttribute ("destPortHigh");
    /**
     * protocol
     */
    public static final Attribute attrProtocol = new StringAttribute ("protocol");
    /**
     * dscp
     */
    public static final Attribute attrDSCP = new NumberAttribute ("dscp");
    /**
     * minimum bandwidth
     */
    public static final Attribute attrBWMin = new NumberAttribute ("bandwidthMin");
    /**
     * maxiimum bandwidth
     */
    public static final Attribute attrExtraBudget = new NumberAttribute ("extraBudget");
    /**
     * allocated bandwidth
     */
    public static final Attribute attrBWAlloc = new NumberAttribute ("bwAllocated");
    /**
     * filter
     */
    public static final Attribute attrACL = new NumberAttribute ("acl");    
    /**
     * svc class - af, ef etc
     */
    public static final Attribute attrSvcClassID = new NumberAttribute ("svcClassID");
    /**
     * Interface of edge where the traffic enters
     */
    public static final Attribute attrInInterfaceID = new NumberAttribute ("inInterfaceID");
    /**
     * Interface of edge where the traffic exits
     */
    public static final Attribute attrOutInterfaceID = new NumberAttribute ("outInterfaceID");
    /**
     * Are edges L2 interfaces?
     */
    public static final Attribute attrL2Endpoints = new NumberAttribute ("l2Endpoints");
    public static final Attribute attrPriority = new NumberAttribute ("priority");
    public static final Attribute attrL3PathID = new NumberAttribute ("l3PathID");
    /**
     * reference
     */
    public static final Attribute attrReference = new StringAttribute ("reference");


    static final Attribute []  allAttr = {attrStatus,
					  purgeStatus,
                                          attrSrcIP,
                                          attrSrcMask,
                                          attrProtocol,
					  attrDSCP,
                                          attrSrcPort,
                                          attrSrcPortHigh,
                                          attrDestIP,
                                          attrDestMask,
                                          attrDestPort,
                                          attrDestPortHigh,
                                          attrBWMin,
                                          attrExtraBudget,
                                          attrBWAlloc,
                                          attrACL,
                                          attrSvcClassID,
                                          attrInInterfaceID,
                                          attrOutInterfaceID,
					  attrL2Endpoints,
					  attrPriority,
					  attrL3PathID,
					  attrReference
                                          };



}


