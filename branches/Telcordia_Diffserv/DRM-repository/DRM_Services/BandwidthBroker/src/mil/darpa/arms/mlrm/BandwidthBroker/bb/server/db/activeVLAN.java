//$Id: activeVLAN.java
/*
 * activeVLAN.java - class for active VLANs
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
 * This class represents active VLANs
 *
 */

public interface activeVLAN extends Top
{
    public static final Attribute attrVlanID = new NumberAttribute ("vlanID");
    public static final Attribute attrPtpID  = new NumberAttribute ("ptpID");
    public static final Attribute attrSwitchID = new StringAttribute ("switchID");

   static final Attribute []  allAttr = {attrVlanID, attrPtpID, attrSwitchID};

}




