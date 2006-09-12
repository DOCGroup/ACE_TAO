package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.FaultMonitorException;

public interface FaultMonitorCallback {
	
   /** A fault occured and we were informed of it */

   public void notifySubscribersOfFault(String swName, String portName, 
	   String status) 
   throws FaultMonitorException;

}
