//  FaultMonitorClientImpl.java
package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import org.omg.CORBA.*;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;

// This class implements the functions of ConditionConsumer
class FaultMonitorClientImpl extends FaultMonitorClientPOA {
  
  private ORB orb;

  // set the ORB
  public void setORB(ORB orb_val) {
    orb = orb_val; 
  }


  public void push_NetworkFault(switchInfo fault) {

    System.out.println("In notify Fault, Switch: " + fault.name + ", Status: " + fault.status.value());

    // decapsulate portInfo
    System.out.println("Port Name: " + fault.ports[0].portName + ", Opn Status: " + fault.ports[0].operStatus.value() + ", Event Type: " + fault.ports[0].event.value());

  }

} // end class FaultMonitorClientImpl
