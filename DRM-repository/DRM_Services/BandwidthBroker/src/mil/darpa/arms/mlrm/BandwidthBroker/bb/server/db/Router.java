//$Id: Router.java
/*
 * Router.java - class for LSR (label swicthed routers)
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
 * This class represents Routers
 * Authors: Kirthika Parmeswaran, Shrirang Gadgil
 */

public interface Router extends ME
{
    /** 
     * router with ingress/egress interfaces
     */
   public static final Attribute attrIsPE = new NumberAttribute ("isProviderEdge");
    /**
     * access list entry at router 
     */
    public static final Attribute attrLastACL = new NumberAttribute ("lastACL");

    static final Attribute []  allAttr = {attrIsPE, attrLastACL};

   /**
    * get L3 interfaces of the Router
    *
    */
   public L3Interface [] getL3Interfaces (AttributeValues av) throws CDBException;
}
