//$Id: ManagedObject.java
/*
 * ManagedObject.java - The base class for all the managed network entities
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
 * This class is a base class for all the network objects
 *
 */

public interface ManagedObject extends Top
{
    /** 
     * Name of entity
     */
    public static final Attribute attrName = new StringAttribute ("name");
   /**
    * valid values are "Locked", "Unlocked" and "Shutting down"
    *
    */
   public static final Attribute attrAdminState = new StringAttribute ("adminState");
   /**
    * valid values are "Enabled", "Disabled" and "Degraded"
    *
    */
   public static final Attribute attrOpnState = new StringAttribute ("operationalState");
    //   public static final Attribute containedInNetwork = new StringAttribute ("containedInNetwork");
    
    
    //  static final Attribute []  allAttr = {name, adminState, opnState, containedInNetwork};
  static final Attribute []  allAttr = {attrName, attrAdminState, attrOpnState};


}
