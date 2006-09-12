//$Id: FlowProvisioningNodeInfo.java
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

// Author: Ravi Vaidyanathan

// This class represents Node Information, including authentication
// credentials and type of device
public class FlowProvisioningNodeInfo {

  static int ACL_START = 150;
  static int NUM_ACLS = 50;
  private String nodeName;
  private String userName;
  private String pwd;
  private String enPwd;
  private String type;
  private int[] aclNo;


  public FlowProvisioningNodeInfo (String _nodeName, String _userName, 
		String _pwd, String _enPwd, String _type) {

    if (_userName.equals("null"))
      userName = null;
    else userName = _userName;

    nodeName = _nodeName;
    pwd = _pwd;
    enPwd = _enPwd;
    type = _type;

    // initialize the ACL Nos
    aclNo = new int [NUM_ACLS];

    for (int i=0; i<NUM_ACLS; i++)
      aclNo[i] = -1;


  }

  public FlowProvisioningNodeInfo (String _nodeName, String _pwd, 
		String _enPwd, String _type) {

    nodeName = _nodeName;
    pwd = _pwd;
    enPwd = _enPwd;
    type = _type;

    // initialize the ACL Nos
    aclNo = new int [NUM_ACLS];

    for (int i=0; i<NUM_ACLS; i++)
      aclNo[i] = -1;
  }

  public String getNodeName() {
    return (nodeName);
  } 

  public String getNodeType() {
    return (type);
  } 


  public String getNodeUserName() {
    return (userName);
  }

  public String getNodePwd() {
    return (pwd);
  }

  public String getNodeEnPwd() {
    return (enPwd);
  }

  public int getNodeAcl() {

    for (int i=0; i<NUM_ACLS; i++) {
      if (aclNo[i] == -1) {
        aclNo[i] = 1;
        return (i+ACL_START);
      }
    }

    return (-1);
  }

  public void freeNodeAcl(int no) {

    if ((no - ACL_START) >= 0 && (no - ACL_START) < NUM_ACLS)
      aclNo[no-ACL_START] = -1;


  }

  public void print() {

    if (userName != null)
      System.out.println(nodeName + ": " + userName + ": " + pwd + ": " + enPwd + ": " + type);
    else System.out.println(nodeName + ": " + pwd + ": " + enPwd + ": " + type);

  }

}


