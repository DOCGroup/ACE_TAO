//$Id$
/*
 * Measurement.java - The flow information
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
package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;


import java.sql.*;

/**
 * This class denotes the flow information
 *
 */

public interface Measurement extends Top
{
  
    /** 
     * probe id
     */
    public static final Attribute attrProbeID = new StringAttribute ("probeID");

    /** 
     * probe timestamp
     */
    public static final Attribute attrProbeTime = new StringAttribute ("probeTime");
   /**
    * delay
    *
    */
   public static final Attribute attrDelay = new NumberAttribute ("delay");
    /**
     * jitter
     */
    public static final Attribute attrJitter = new NumberAttribute ("jitter");
     /**
     * loss
     */
    public static final Attribute attrLoss = new NumberAttribute ("loss");
    /** 
     * type of service
     */
    public static final Attribute attrDSCP = new NumberAttribute ("dscp");
   
    static final Attribute []  allAttr = {attrProbeID,
					  attrProbeTime,
					  attrDelay,
					  attrJitter,
					  attrLoss,
					  attrDSCP
					  };
}


