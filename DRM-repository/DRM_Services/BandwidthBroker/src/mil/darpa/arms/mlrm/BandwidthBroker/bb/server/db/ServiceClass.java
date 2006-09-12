//$Id: ServiceClass.java
/*
 * ServiceClass.java - The service class for the flow over the L3 switch network
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
 * This class is denotes the service class for a flow over a L3 switch network
 *
 */

public interface ServiceClass  extends Top
{
    /** 
     * Service class name
     */
    public static final Attribute attrName = new StringAttribute ("name");
    /** 
     * QoS codepoint
     */
    public static final Attribute attrDSCP = new NumberAttribute ("dscp");
   /**
    * precedence for packet policing
    * (used at L2 primarily)
    */
   public static final Attribute attrDropPrecedence = new NumberAttribute ("dropPrecedence");
    /**
     * utilization percentage of link bandwidth
     */
    public static final Attribute attrUtilLimit = new NumberAttribute ("utilLimit");

    static final Attribute []  allAttr = {attrName, attrDSCP, attrDropPrecedence, attrUtilLimit};


}
