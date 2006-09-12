package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;

import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.NetworkProbesException;

public interface NetworkMonitorCallback {
	
	public long getDelay (
			String _probeID, 
			long _averagingWindow, 
			long  _measurementPeriod, 
			long  instant)
	throws NetworkProbesException;

}
