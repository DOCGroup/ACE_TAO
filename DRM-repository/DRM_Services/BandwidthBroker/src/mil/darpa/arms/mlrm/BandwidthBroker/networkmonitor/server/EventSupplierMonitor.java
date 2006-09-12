//$Id$
/*
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */
package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;

// Defines an a event supplier monitor event interface 
public interface EventSupplierMonitor extends Runnable
{
    public void start ();
    public void run ();
    public void stop ();

} // end interface 
