//$Id: ME.java
/*
 * ME.java - The base class for all the managed elements entities
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
 * This class represents an ME
 * Authors : Kirthika Parmeswaran, Shrirang Gadgil
 */

public interface ME extends ManagedObject
{
    //  public static final Attribute METype = new StringAttribute ("METype");
    /** 
     * equipment identifier
     */
    public static final Attribute attrEquipCode = new StringAttribute ("equipmentID");
    /**
     * vendor name
     */
    public static final Attribute attrVendorName = new StringAttribute ("vendor");
    /**
     * OS version
     */
    public static final Attribute attrOSVersion = new StringAttribute ("osVersion");
    /**
     * telnet password
     */
    public static final Attribute attrPW = new StringAttribute ("password");
    /**
     * enabling password
     */
    public static final Attribute attrEnablePW = new StringAttribute ("enablePW");
    /**
     * managedIPAddr
     */
    public static final Attribute attrManIP = new StringAttribute ("managedIPAddr");
   
    
    static final Attribute []  allAttr = {attrEquipCode, attrVendorName, attrOSVersion,
					  attrPW, attrEnablePW, attrManIP };
}

