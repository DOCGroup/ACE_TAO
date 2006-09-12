//$Id: HostSubnet.java
/*
 * HostSubnet.java - The host subnet for the flow over the L3 switch network
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
 * This class is denotes the host subnet for a flow 
 *
 */

public interface HostSubnet extends Top
{
    /** 
     * current host subnet network id
     */
    public static final Attribute attrHostSubnetAddr = new StringAttribute ("hostSubnetAddr");
   /**
    * subnet mask
    *
    */
   public static final Attribute attrHostSubnetMask = new StringAttribute ("hostSubnetMask");
    /**
     * which l3 interface
     */
    public static final Attribute attrL3InterfaceID = new NumberAttribute ("l3InterfaceID");
    /**
     * which pool name
     */
    public static final Attribute attrPoolName = new StringAttribute ("poolName");
    /**
     * L2Interface
     */
    public static final Attribute attrL2InterfaceID = new NumberAttribute ("l2InterfaceID");
    /**
     * L3Gateway
     */
    public static final Attribute attrL3Gateway= new StringAttribute ("l3Gateway");
    /**
     * Hostname
     */
    public static final Attribute attrHostname = new StringAttribute ("hostName");
    static final Attribute []  allAttr = {attrHostSubnetAddr, attrHostSubnetMask, attrL3InterfaceID, attrPoolName, attrL2InterfaceID, attrL3Gateway, attrHostname};


}


