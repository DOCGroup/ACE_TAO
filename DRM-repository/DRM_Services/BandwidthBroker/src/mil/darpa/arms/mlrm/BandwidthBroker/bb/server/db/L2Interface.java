//$Id: L2Interface.java
/*
 * L2Interface.java - class for L2 switchs
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
 * This class represents L2Interface
 *
 */

public interface L2Interface extends CTP
{
   public static final Attribute attrSwitchID = new NumberAttribute ("switchID");
    public static final Attribute attrPTPID = new NumberAttribute ("ptpID");
   static final Attribute []  allAttr = {attrSwitchID, attrPTPID};

    
}
