//$Id: Switch.java
/*
 * Switch.java - class for L2 swiches
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class represents Switches
 * Authors: Kirthika Parmeswaran, Shrirang Gadgil
 */

public interface Switch extends ME
{
    /**
     * Read Community
     */
    public static final Attribute attrReadCommunity = new StringAttribute ("readCommunity");

    /** 
     * Switch that is monitored by FaultMonitor
     */
    public static final Attribute attrIsMonitored = new NumberAttribute ("isMonitored");


    static final Attribute []  allAttr = {attrReadCommunity, attrIsMonitored};

   /**
    * get L2 interfaces of the Switch
    *
    */
   public L2Interface [] getL2Interfaces (AttributeValues av) throws CDBException;
}
