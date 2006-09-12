//$Id$
/*
 * ProbeSetup.java - setting up the probe 
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;

public class ProbeDefaults
{
    String gap_=null;
    String packetSize_=null;
    String tos_=null;
    String packetsNum_=null;
    String measurementPeriod_ =null;

    ProbeDefaults (String gap,
		   String packetSize,
		   String tos,
		   String packetsNum,
		   String measurementPeriod)
    {
	gap_=gap;
	packetSize_= packetSize;
	tos_= tos;
	packetsNum_= packetsNum;
	measurementPeriod_ = measurementPeriod;
    }

    public String getGap ()
    {
	return gap_;
    }

    public void setGap (String g)
    {
       gap_=g;
    }

     public String getPacketSize ()
    {
	return packetSize_;
    }

    public void setPacketSize (String p)
    {
       packetSize_=p;
    }

     public String getTos ()
    {
	return tos_;
    }

    public void setTos (String t)
    {
       tos_=t;
    }

     public String getPacketsNum ()
    {
	return packetsNum_;
    }

    public void setPacketsNum (String p)
    {
       packetsNum_=p;
    }

     public String getMeasurementPeriod ()
    {
	return measurementPeriod_;
    }

    public void setMeasurementPeriod (String m)
    {
       measurementPeriod_=m;
    }

    
}
