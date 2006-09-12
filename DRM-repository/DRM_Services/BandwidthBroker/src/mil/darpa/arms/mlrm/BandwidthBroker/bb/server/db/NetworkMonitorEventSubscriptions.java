//$Id$

/*
 * NetworkMonitorEventSubscriptions.java - The network event subscription information
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */
package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/** * This class represents NetworkMonitorEventSubscriptions
 *
 */

public interface NetworkMonitorEventSubscriptions extends Top
{
    public static final Attribute attrEventConsumer = new StringAttribute ("eventConsumer");
    public static final Attribute attrCookie = new StringAttribute ("cookie");
    public static final Attribute attrProbeID  = new StringAttribute ("probeID");
    public static final Attribute attrEventSupplierMonitor = new StringAttribute ("eventSupplierMonitor");

   static final Attribute []  allAttr = {attrEventConsumer, attrCookie, attrProbeID, attrEventSupplierMonitor};

}




