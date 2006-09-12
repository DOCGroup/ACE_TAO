//$Id: FlowProvisioningSpec.java,
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157 
package mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import java.util.*;
import com.telcordia.jIOS.*;

// Author: Ravi Vaidyanathan, Kirthika Parmeswaran

// This class represents a single unique flow and all of its data
// optimized for communication with the switches
class FlowProvisioningSpec {

  public String nodeName;
  public String portName;
  public String flowID;

  public int editBufferIndex;

  public StringBuffer policer;
  public StringBuffer acl;
  public StringBuffer aclName;
  public int aclNo;

  public FlowProvisioningSpec (String _nodeName, String _portName, String _flowID) {

    nodeName = _nodeName;
    portName = _portName;
    flowID = _flowID;

  }


}


