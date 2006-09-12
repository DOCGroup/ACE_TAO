package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;

// Defines an a fault monitor event interface that
// clients of FaultMonitorServer must implement
public interface FaultMonitorEvent {

  public void faultMonitorEvent(switchInfoHolder swInfo);
  

} // end interface FaultMonitorEvent
