//$Id: PTP.java
/*
 * PTP.java - class for Physical Termination Point (PTP)
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
 * This class represents PTP
 *
 */

public interface PTP extends ManagedObject
{
    public static final Attribute attrRate = new NumberAttribute ("rate");
    public static final Attribute attrPhyAddress = new StringAttribute ("phyAddress");
    public static final Attribute attrSwitchID = new StringAttribute ("switchID");
    public static final Attribute attrRouterID = new StringAttribute ("routerID");

    public static final Attribute attrIfIndex = new NumberAttribute ("ifIndex");
    public static final Attribute attrLastEvent = new StringAttribute ("lastEvent");
   static final Attribute []  allAttr = {attrRate,attrPhyAddress, attrSwitchID, attrRouterID, attrIfIndex, attrLastEvent};

}




