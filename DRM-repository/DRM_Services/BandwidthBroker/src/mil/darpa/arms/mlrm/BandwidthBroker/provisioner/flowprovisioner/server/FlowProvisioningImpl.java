// $Id: FlowProvisioningImpl.java
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
//import BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;

import java.util.*;
import java.io.*;
import java.security.*;
import javax.crypto.*;
import javax.crypto.spec.*;
import com.telcordia.jIOS.*;
import org.apache.log4j.Logger;

// Author: Ravi Vaidyanathan

/** This class implements the functions of FlowProvisioning.
 */
public class FlowProvisioningImpl extends FlowProvisionerPOA {
  
  private ORB orb;

  // store the type of request, needed becos linuxCommands() that has tryToFailFP() is 
   // called twice in addLinuxFlow(), this messes up the inv in tryToFailFP()
   static int inv_linux=0;
 
  private static boolean debug = true;
  private TreeMap allACLs = new TreeMap();
  private TreeMap allNodes = new TreeMap();
  static Logger logger =Logger.getLogger(FlowProvisioningImpl.class.getName());

  // Keeps track of commands used to delete flow in Linux routers
  private TreeMap delCommands = new TreeMap();

  // IOS and CatOS command strings are stored here for reference only
  private static String catPolicer = new String 
    ("set qos policer aggregate $flowID$ rate $rate$ policed-dscp " +
     "erate $excessRate$ $exceedAction$ burst $burst$");

  private static String iosPolicer = new String 
    ("rate-limit input access-group $aclNo$ $rate$ $burst$ " +
     "$excessBurst$ conform-action set-dscp-transmit $dscp$ " +
     "exceed-action $exceedAction$");

  private static String policer = new String 
    ("police $bps$ $burst-normal$ $burst-max$ conform-action " +
     "set-dscp-transmit $dscp$ exceed-action $action$");

  private boolean loopback = false; 
  public static int failWhenFP = 0;
  public static int failAfterXFP = 0;
  // This function converts passed subnetmasks to wilcard masks
  private String _maskToWildcard(String subnetMask) {
    StringTokenizer st = new StringTokenizer(subnetMask, ".");
    StringBuffer wildcard = new StringBuffer();

    while (st.hasMoreTokens()) {
      wildcard.append(Integer.parseInt(st.nextToken())^255);
      if (st.hasMoreTokens())
        wildcard.append(".");
    }
    return(wildcard.toString());
  }

  static Hashtable cuCache = new Hashtable ();
  private class CUSession
  {
     private long sessionTimeOut = 10000; // 10 seconds
     CiscoUtils cu;
     long timeLastUsed;

     public CUSession (CiscoUtils cu)
     {
        this.cu = cu;
        resetTimer ();
     }

     public void resetTimer ()
     {
	timeLastUsed = System.currentTimeMillis ();
     }

     public boolean isExpired ()
     {
        return ((System.currentTimeMillis () - timeLastUsed) > sessionTimeOut);
     }
  }

  // This function issues the specified cmds to the node
  private void _issueCmd(String cmd, FlowProvisioningNodeInfo node) 
  {
    try 
    {
      if (loopback == false) 
      {

         // Search hashtable for an instance of CiscoUtils
         CUSession cus = (CUSession) cuCache.get (node.getNodeName ());
	 boolean needNewSession = false;
	 if (cus == null)
	    needNewSession = true;
	 else if (cus.isExpired ())
	 {
	    needNewSession = true;
	 }
	    
         CiscoUtils cu;
	 if (needNewSession == true)
         {
	    System.out.println ("Creating new telnet session");
            cu = new CiscoUtils (node.getNodeType());
	    cuCache.put (node.getNodeName (), cus = new CUSession (cu));
            // Use the CiscoUtils class to implement this functionality
            // look for the nodename <--> type mapping
      
            if (node.getNodeUserName () != null)
            {
	       System.out.println ("Logging into router with username");
               cu.cisLogin(node.getNodeName(), node.getNodeUserName(),
		           node.getNodePwd(), node.getNodeEnPwd());
            }
            else
            {
	       System.out.println ("Logging into router without username");
               cu.cisLogin(node.getNodeName(), 
		           node.getNodePwd(), node.getNodeEnPwd());
            }
         }
         else
         {
            cu = cus.cu;
	    System.out.println ("Reusing previous telnet session");
         }
   
         if (node.getNodeType().equals("CatOS")) {
           cu.cisExec(cmd);
         } else if (node.getNodeType().equals("IOS")) {
           // For IOS, make sure we write the running config to the startup config
           cu.cisConf(cmd);
	   cus.resetTimer ();
           // cu.cisExec("write memory");
         } else if (node.getNodeType().equals("Linux")) {
           cu.cisExec(cmd);
	   cus.resetTimer ();
         } else {
	   System.out.println("Unimplemented type: " + node.getNodeType());
         }

         // Preserve for the next set of commands
         // cu.cisLogout();
       }
       logger.info (cmd);
    }
    catch (Exception e)
    {
       e.printStackTrace ();
    }
  }
   
  // This function issues the specified cmds to the node
  private void _issueCmdOld(String cmd, FlowProvisioningNodeInfo node) {

    try 
    {
    if (loopback == false) {

      // Use the CiscoUtils class to implement this functionality
      // look for the nodename <--> type mapping
      CiscoUtils cu = new CiscoUtils (node.getNodeType());
      if (node.getNodeUserName () != null)
      {
	 System.out.println ("Logging into router with username");
         cu.cisLogin(node.getNodeName(), node.getNodeUserName(),
		     node.getNodePwd(), node.getNodeEnPwd());
      }
      else
      {
	 System.out.println ("Logging into router without username");
         cu.cisLogin(node.getNodeName(), 
		     node.getNodePwd(), node.getNodeEnPwd());
      }
      if (node.getNodeType().equals("CatOS")) {
        cu.cisExec(cmd);
      } else if (node.getNodeType().equals("IOS")) {
        // For IOS, make sure we write the running config to the startup config
        cu.cisConf(cmd);
        // cu.cisExec("write memory");
      } else if (node.getNodeType().equals("Linux")) {
        cu.cisExec(cmd);
      } else {
	System.out.println("Unimplemented type: " + node.getNodeType());
      }
      cu.cisLogout();
    }
    logger.info (cmd);
    }
    catch (Exception e)
    {
       e.printStackTrace ();
    }
  }

  /** Sets this instance of the flowProvisioner in loopback mode.
   *  This allows the flowProvisioner to be tested without
   *  actually sending commands to a router.
   */
  public void setLoopback() {
    System.out.println("Flow Provisioner in Loopback mode");
    System.out.println("No commands will be sent to the network");
    loopback = true;
  }

    
  /** Perform some init functions for the authentication system associated with
   *  node passwords, etc.
   *  
   * @param file an encrypted file containing information on routers,
   *  including their enabling passwords
   */
  public void initAuth(String file) {
    byte[] buf = new byte[8];
    byte[] b = new byte[100];
    int len;

    // get the decrytion key 
    System.out.print("Enter decryption key (8 chars ONLY): ");
    try {
      //java.io.DataInputStream in = new java.io.DataInputStream(System.in);
      //len = in.read(buf); 
      BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      String temp = in.readLine();	
      SecretKeySpec key = new SecretKeySpec (temp.getBytes(), "DES");
      Cipher c = Cipher.getInstance("DES");
      c.init(Cipher.DECRYPT_MODE, key);
      
      CipherInputStream read_back = 
	new CipherInputStream(new FileInputStream(file), c);
      StringBuffer sb = new StringBuffer();

      while ((len = read_back.read(b)) != -1) {
        sb.append(new String(b, 0, len));
      }
      read_back.close();

      // now tokenize this string and parse the info
      StringTokenizer st = new StringTokenizer(sb.toString());
      FlowProvisioningNodeInfo node; 
      String nodeName;
      while (st.countTokens() >= 5) {
        nodeName = st.nextToken();
        node = new FlowProvisioningNodeInfo(nodeName,
					    st.nextToken(), st.nextToken(), 
					    st.nextToken(), st.nextToken());
        if (debug)
	  node.print();
        allNodes.put(nodeName, node);
      } 

    } catch (Exception e) {
      e.printStackTrace();
    } 
  }

  /** Set the ORB.
   * @param orb_val The ORB to use.
   */
  public void setORB(ORB orb_val) {
    orb = orb_val; 
  }

    
  private void addIOSFlow(FlowProvisioningNodeInfo node,
			  FlowProvisioningSpec flow,
			  String portName,
			  short dscp,
			  int rate,
			  int burst) 
    {

    // construct the aclName for this flow with the flowID
    flow.aclName.append ("_"+flow.flowID);

    /* get a new class map for this */
    // class-map $class-map-name$
    // the class map name needs to be unique - many classes possible on a single 
    // interface - append the acl no to the acl name
    String className = new String("CLASS_"+flow.aclName.toString());
    _issueCmd("class-map " + className + "\r" + "match access-group name "+flow.aclName.toString(), node);
    
    // only supported exceed action is drop
    // need to get into interface config mode
    // "police $bps$ $burst-normal$ $burst-max$ conform-action set-dscp-transmit $dscp$ exceed-action $action$" 
    flow.policer = new StringBuffer("police " + rate + " " + burst +  
				    " conform-action transmit " + " exceed-action drop"); 
      
    // Send the policer command to the router - under the policy-map
    
    //String policyName = new String("MLRMAccessQoS");
    String policyName = "POLICY_" + flow.aclName.toString();
    _issueCmd("policy-map " + policyName +"\rclass " + className +"\r" + "set ip dscp " + dscp + 
	      "\r" + flow.policer.toString(), node);
    
    _issueCmd("int " + portName +"\r" + "service-policy input " + policyName, node);
      // start generation of IOS ACL
    flow.acl = new StringBuffer("ip access-list extended " + flow.aclName.toString() + "\n permit ");
    }

  private void addIOSFlowOldVersion(FlowProvisioningNodeInfo node,
			  FlowProvisioningSpec flow,
			  String portName,
			  short dscp,
			  int rate,
			  int burst) {
    // start generation of IOS policer
    // ("rate-limit input access-group $aclNo$ $rate$ $burst$ $excessBurst$ 
    //  conform-action set-dscp-transmit $dscp$ exceed-action $exceedAction$");
    
    /* ACL number generation needs to be fixed - do some math on aclName?*/
    flow.aclNo = node.getNodeAcl();
    
    /* get a new class map for this */
    // class-map $class-map-name$
    // the class map name needs to be unique - many classes possible on a
    // single  interface - append the acl no to the acl name
    String className = new String(flow.aclName.toString() + "_" + flow.aclNo);

    _issueCmd("class-map " + className + "\r" +
	      "match access-group " + flow.aclNo, node);
    
    // only supported exceed action is drop
    // need to get into interface config mode
    // "police $bps$ $burst-normal$ $burst-max$ conform-action
    // set-dscp-transmit $dscp$ exceed-action $action$" 
    flow.policer = new StringBuffer("police " + rate + " " +  burst + 
				    " conform-action transmit " +
				    " exceed-action drop"); 
    
    // Send the policer command to the router - under the policy-map
    _issueCmd("policy-map " + flow.aclName.toString() +"\rclass " + 
	      className +"\r" + "set ip dscp " + dscp + "\r" +
	      flow.policer.toString(), node);
    
    _issueCmd("interface " + portName +"\r" + "service-policy input " + 
	      flow.aclName.toString(), node);

    // start generation of IOS ACL
    flow.acl = new StringBuffer("access-list  " + flow.aclNo + " permit");
  }

  private void addCatOSFlow(FlowProvisioningNodeInfo node,
			     FlowProvisioningSpec flow,
			     String flowID,
			     TrustState trust,
			     short dscp,
			     int rate,
			     int burst,
			     int excessRate,
			     ExceedAction excess) {
      // construct the CatOS policer
      flow.policer = new StringBuffer("set qos policer aggregate "
				+  flowID + " rate " + rate
				+ " policed-dscp erate " + excessRate);

      if (excess.value() == ExceedAction._DROP)
	    flow.policer = flow.policer.append(" drop");
      else flow.policer = flow.policer.append(" policed-dscp");

      flow.policer.append(" burst " + burst);

      // Send the policer command to the switch/router
      _issueCmd(flow.policer.toString(), node);

      // start generation of CatOS ACL
      flow.acl = new StringBuffer("set qos acl ip " + 
	  flow.aclName.toString() + " ");

      // check the trust State 
      switch (trust.value()) {
	    case (TrustState._none): flow.acl =
				       flow.acl.append("dscp " + dscp);
	    break;

	    case (TrustState._DSCP): flow.acl = flow.acl.append("trust-dscp");
				break;

	    case (TrustState._IP_PREC): flow.acl = 
					  flow.acl.append("trust-iprec");
				break;

	    case (TrustState._L2_COS): flow.acl = flow.acl.append("trust-cos");
				break;
      }

      // add the policer
      flow.acl = flow.acl.append(" aggregate "+ flowID);
  }

  /** Construct a set of commands for a Linux router.
   *  The "add" parameter is true for adding a flow
   *  or false for deletion.
   */
  private String linuxCommands( boolean addFlow,
				FlowProvisioningNodeInfo node,
				FlowProvisioningSpec flow,
				String flowID,
				String portName,
				short dscp,
				int rate,
				int burst,
				FlowInfo filter) 
    {
    final String IPTAB="/usr/bin/sudo /usr/local/sbin/iptables ";
    final String TC="/usr/bin/sudo /sbin/tc ";
    String proto, source, destination;
    StringBuffer acl;
    StringBuffer cmds;
    StringBuffer delCmds;
    int aclNo;
    switch (filter.proto.value()) {
    case ProtocolType._tcp:
      proto = "tcp";
      break; 
    case ProtocolType._udp:
      proto = "udp";
      break; 
    default:
    case ProtocolType._ip:
      proto = "ip";
    }
    source = filter.srcIP.dottedDecimal;
    destination = filter.destIP.dottedDecimal;
    acl = new StringBuffer(" -p " + proto);

    if (! source.equals("any")) {
      acl.append(" -s "+ source + "/" + filter.srcIP.subnetMask); 
    }
    if (! destination.equals("any")) {
      acl.append(" -d " + destination + "/" + filter.destIP.subnetMask); 
    }
    if (proto.equals("tcp") || proto.equals("udp")) {
      if (filter.srcPorts.low != -1) {
	if (filter.srcPorts.low == filter.srcPorts.high)
	  acl.append (" --source-port " +  filter.srcPorts.low);
	else
	  acl.append (" --source-port " +  filter.srcPorts.low +
		      ":" + filter.srcPorts.high);
      }
      if (filter.destPorts.low != -1) {
	if (filter.destPorts.low == filter.destPorts.high)
	  acl.append (" --destination-port " +  filter.destPorts.low);
	else
	  acl.append (" --destination-port " +  filter.destPorts.low +
		      ":" + filter.destPorts.high);
      }
    }
    if (filter.dscp != -1) {
      acl.append(" -m dscp --dscp " + filter.dscp);
    }

    try {
      aclNo = Integer.parseInt(flowID);
      // Marks cannot be larger than 15
      aclNo &= 0x7f;
    } catch (NumberFormatException nfe) {
      logger.error("Non-numeric flow id: " + flowID);
      aclNo = 1;
    }
    
    // Valid marks
    cmds = new StringBuffer();
    String add_del = "-D";
    if (addFlow) {
      add_del = "-A";
    }
/*    cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING -p " + proto + " -i " +
		portName + " -j MARK --set-mark " + aclNo + " ; ");

    cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING -p " + proto + " -i " +
		portName + " -j DSCP --set-dscp " + dscp + " ; ");

*/
    if (addFlow)
    {
       // Make the commands idempotent by issuing a delete before add
       
       cmds.append(IPTAB + "-t mangle " + "-D" +
		" PREROUTING " + acl +
		" -j MARK --set-mark " + aclNo + " ; ");

       cmds.append(IPTAB + "-t mangle " + "-D" +
		" PREROUTING" + acl + " -j DSCP --set-dscp " + dscp + " ; ");

    }
    cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING " + acl +
		" -j MARK --set-mark " + aclNo + " ; ");

    cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING" + acl + " -j DSCP --set-dscp " + dscp + " ; ");
    //Commented the following commands as they are repetitive: Kirthika
    /*cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING " + acl +
		" -j MARK --set-mark " + aclNo + " ; ");

    cmds.append(IPTAB + "-t mangle " + add_del +
		" PREROUTING" + acl + " -j DSCP --set-dscp " + dscp + " ; ");
  */
    add_del = "del";
 


    if (addFlow) {
      add_del = "add";
      cmds.append(TC + "qdisc add dev " + portName +
		  " ingress" + " ; ");

      
    }
      System.out.println ("Trying to fail during provisioning: failWhen="+failWhenFP+"  and failAfterX="+failAfterXFP);
   
    tryToFailFP (1);    

    cmds.append(TC + "filter " + add_del + " dev " + portName + 
		" parent ffff: protocol ip prio 50 handle " +
		aclNo + " fw police rate " + rate/1000 + "kbit burst " +
		burst + " drop flowid :1");

    return cmds.toString();
  }

    private void addLinuxFlow(FlowProvisioningNodeInfo node,
			    FlowProvisioningSpec flow,
			    String flowID,
			    String portName,
			    short dscp,
			    int rate,
			    int burst,
			    FlowInfo filter)
       throws IOException
  {
    String cmds;
    logger.info("\n-----Begin addLinuxFlow at " +
      new Date().toString() + "\r");

    inv_linux=1;
    cmds = linuxCommands(true, node, flow, flowID, portName,
			 dscp, rate, burst, filter);
    

    logger.info(cmds);
    LinuxUtils._issueCmd(cmds,node.getNodeName (), loopback);

    inv_linux=2;
    // Save the delete command for stateless version of delete
    cmds = linuxCommands(false, node, flow, flowID, portName,
			 dscp, rate, burst, filter);
    
    String dkey = node.getNodeName() + ":" + portName + ":" + flowID;
    logger.info("Save del under key=" + dkey + ".  Cmd=" + cmds);
    delCommands.put(dkey, cmds);
  }

    /** Get node authentication, OS information to perform provisioning of different devices and OSes */
    private FlowProvisioningNodeInfo getNodeInfo (String nodeName) throws IOException
    {
	// prompt for Authentication information
      System.out.println("Authentication Information for " + nodeName + 
			" is not available"); 
      System.out.println("Enter user name: ");
      BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      
      String user = in.readLine();
      System.out.println("Enter password: ");
      String pwd = in.readLine();
      System.out.println("Login pwd =" + pwd + ".");
      if (pwd.length() < 2) {
	  pwd = in.readLine();
	  System.out.println("Login pwd =" + pwd + ".");
      }
      System.out.println("Enter enable password: ");
      String enPwd = in.readLine();
      System.out.println("Enter device sw type (IOS/CatOS/Linux): ");
      String type = in.readLine();
      FlowProvisioningNodeInfo node = new FlowProvisioningNodeInfo(nodeName, user, pwd, enPwd, type);
      System.out.println("New node: " + nodeName + "," +
			 user + "," + pwd + "," + enPwd + "," + type
			 + ".");
      allNodes.put(nodeName, node);
      return node;
    }

  /** Adds policing and marking information for the specified flow.
   *  Characteristics of the node (e.g., node type) are determined
   *  from the encrypted file loaded by <code>initAuth</code>.
   *  If the node is not defined there, the user is expected to
   *  enter the information interactively (see <code>initAuth</code>
   *  for more info).
   *
   * @param nodeName   name of the router to be controlled
   * @param portName   interface name (e.g., "Ethernet1/1")
   * @param flowID     flow identifier (from BB database)
   * @param trust      trust (for CatOS QoS)
   * @param dscp       diffserv code point (e.g., "10") for outgoing
   * @param filter     5-tuple rule
   * @param rate       policing rate (bit rate in bits per second)
   * @param burst      maximum allowed burst size in byte
   * @param excessRate tolerated excess
   * @param normal     [not currently used]
   * @param excess     action to take when exceeding BW 
   */
  public void addFlow (String nodeName,
		       String portName,
		       String flowID, 
		       TrustState trust, 
		       short dscp,
		       FlowInfo filter, 
		       int rate,
		       int burst,
		       int excessRate, 
		       ExceedAction normal, 
		       ExceedAction excess) {
  
    int index;

    System.out.println("Adding Flow: " + flowID);

    // look up the style for this node
    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);

    if (node == null) {
      // prompt for Authentication information
	try {
	   node=getNodeInfo (nodeName);
	} catch (IOException ioe) {
	     ioe.printStackTrace();
	}
    }

    // init a new flow
    FlowProvisioningSpec flow = new FlowProvisioningSpec(nodeName,
							 portName, flowID);

    // construct the aclName for this flow
    flow.aclName = new StringBuffer("ACL_" + nodeName + "_" + portName);

    // pull out any special chars
    while ((index = flow.aclName.indexOf("/")) != -1) {
	  flow.aclName = flow.aclName.deleteCharAt(index);
    }

    
    // check the style of this node - CatOS or IOS 
    if (node.getNodeType().equals("CatOS")) {
    
	ArrayList aclEntries = (ArrayList) allACLs.get(flow.aclName.toString()); 

	// does the ACL already exist?
	if (aclEntries == null) {
	    aclEntries = new ArrayList();
	    allACLs.put(flow.aclName.toString(), aclEntries);
	}
	
	aclEntries.add(flow);
	
	addCatOSFlow(node,flow,
		     flowID,trust,dscp,rate,burst,excessRate,excess);

    } else if (node.getNodeType().equals("IOS")) {

      addIOSFlow(node,flow,
		 portName, dscp, rate, burst);

    } else if (node.getNodeType().equals("Linux")) {

      // For Linux, we need the source & destination
      // when the call is made
      String source;
      String destination;
      try
      {
         addLinuxFlow(node, flow, flowID,
		   portName, dscp, rate, burst,
		   filter);
	 
     }
      catch (IOException ie)
      {
         logger.error("Could not configure addLinuxFlow");
      }
      return;
    } else {

      System.out.println("Unknown node type ignored: " +
			 node.getNodeType());

    }

    // now look into the flowInfo
    switch (filter.proto.value()) {

		case ProtocolType._tcp:
			flow.acl = flow.acl.append(" tcp ");
			break; 

		case ProtocolType._udp:
			flow.acl = flow.acl.append(" udp ");
			break; 

		case ProtocolType._ip:

		default:
			flow.acl = flow.acl.append(" ip ");
			break;
  	}

    // drop in the srcIP
    flow.acl = flow.acl.append (filter.srcIP.dottedDecimal + " ");

    if (!filter.srcIP.dottedDecimal.equals("any")) {
      flow.acl = flow.acl.append (_maskToWildcard(filter.srcIP.subnetMask) 
				  + " "); 
    }

    // Check the source port information
    if (filter.srcPorts.low != -1) {

      if (filter.srcPorts.low == filter.srcPorts.high)
        flow.acl = flow.acl.append ("eq " +  filter.srcPorts.low);
      else
        flow.acl = flow.acl.append ("range " +  filter.srcPorts.low +
				    " " + filter.srcPorts.high);
    }

    // drop in the dstIP
    flow.acl = flow.acl.append (" " + filter.destIP.dottedDecimal + " ");

    if (!filter.destIP.dottedDecimal.equals("any"))
      flow.acl = flow.acl.append (_maskToWildcard(filter.destIP.subnetMask) +
				  " "); 

    // Check the dest port information
    if (filter.destPorts.low != -1) {

      if (filter.destPorts.low == filter.destPorts.high)
        flow.acl = flow.acl.append ("eq " +  filter.destPorts.low);
      else
        flow.acl = flow.acl.append ("range " +  filter.destPorts.low +
				    " " + filter.destPorts.high);
    }

    // check the DSCP information 
    if (filter.dscp != -1)
      flow.acl = flow.acl.append(" dscp " + filter.dscp);

    // issue the ACL commands to the switch/router
    _issueCmd(flow.acl.toString(), node);

    // commit the ACL for CatOS
    if (node.getNodeType().equals("CatOS")) 
      _issueCmd("commit qos acl " + flow.aclName.toString(), node);
   

  }

  private void  delIOSFlow(FlowProvisioningNodeInfo node,
			   String flowID, 
			   String portName)
    {
	int i;

    System.out.println("Deleting Flow: " + flowID);
  
    // construct the ACL name
     StringBuffer aclName = new StringBuffer("ACL_" + node.getNodeName () + "_" + portName+ "_"+flowID);

    // pull out any special chars
    while ((i = aclName.indexOf("/")) != -1) {
	aclName = aclName.deleteCharAt(i);
    }
    
    String className = "CLASS_"+ aclName.toString();
    String policyName = "POLICY_" + aclName.toString();
    // String cmd = "policy-map " + policyName + "\r" + "no class " + className;
    String cmd = "policy-map " + policyName + "\r" + "no class " + className + "\r"
                  + "no ip access-list extended " + aclName.toString() + "\r"
                  + "no class-map " + className;
    
    _issueCmd(cmd, node);
    // No delete of service policy : Kirthika
    //_issueCmd("no ip access-list extended " + aclName.toString(), node);
    //try {Thread.sleep (2000);}catch (Exception e){};
    //_issueCmd("no class-map " + className, node);
    
    return;
}

  private void  delIOSFlowOldVersion(FlowProvisioningNodeInfo node,
			   FlowProvisioningSpec flow, 
			   ArrayList aclEntries,
			   String portName, 
			   StringBuffer aclName) {
    // IOS delete commands
    String className = aclName.toString() + "_" + flow.aclNo;
    String cmd = "policy-map " + aclName.toString() + "\r" + 
      "no class " + className;
    _issueCmd(cmd, node);
    _issueCmd("no class-map " + className, node);
    
    // if there is only one guy left - blow away the policy map
    // and the interface association as well 
    if (aclEntries.size() == 1) {
      _issueCmd("interface " + portName +"\r" + "no service-policy input " +
		aclName.toString(), node);
      _issueCmd("no policy-map " + aclName.toString(), node);
    }
    
    // clear the ACL
    _issueCmd("no " + flow.acl.toString(), node);
    
    // release the ACL no from the Node class
    node.freeNodeAcl(flow.aclNo);
  }

  private void delCatOSFlow(FlowProvisioningNodeInfo node,
			    String flowID, 
			    StringBuffer aclName,
			    int index) {

            String cmd = "clear qos acl " + aclName + " "+ index;
            _issueCmd(cmd, node);

            // commit it 
            _issueCmd("commit qos acl " + aclName, node);;

            // clear the policer
	    _issueCmd("clear qos policer aggregate " + flowID, node);

  }

  /** Stateful Linux delete.
   */
  private void delLinuxFlow(String nodeName,
			    String flowID,
			    String portName) 
  {
    logger.info("\n-----Begin delLinuxFlow at " +
      new Date().toString() + "\n");
    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);
    if (node == null) {
      logger.error("Attempt to delete unknown flow: " + nodeName +
		   ":" + portName + ":" + flowID);
    }
    String dkey = nodeName + ":" + portName + ":" + flowID;
    try {
      String delCmds = (String)delCommands.get(dkey);
      if (delCmds == null) {
	logger.error("Tried to delete Linux flow, key= " + dkey);
      } else {
	logger.info("Linux delete: " + delCmds);
	LinuxUtils._issueCmd(delCmds,nodeName, loopback);
	delCommands.remove(dkey);
      }
    } catch (NullPointerException npe) {
      logger.error("Called delLinuxFlow with bad key");
      npe.printStackTrace();
    } catch (ClassCastException cce) {
      logger.error("Could not cast key in call to delLinuxFlow");
      cce.printStackTrace();
    } catch (IOException ie) {
      logger.error("Could not configure delLinuxFlow");
      ie.printStackTrace();
    }
  }

  /** Stateless Linux delete.
   */
  private void delLinuxFlow(String nodeName,
		       String portName,
		       String flowID, 
		       TrustState trust, 
		       short dscp,
		       FlowInfo filter, 
		       int rate,
		       int burst,
		       int excessRate, 
		       ExceedAction normal, 
		       ExceedAction excess) throws IOException
  {
    String cmds;
    logger.info("\n-----Begin delLinuxFlow SL at " +
		new Date().toString() + "\n");
    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);
    if (node == null) {
      // Should already be present
      logger.error("Attempt to delete unknown node");
      return;
    }
    FlowProvisioningSpec flow = new FlowProvisioningSpec(nodeName,
							 portName, flowID);
    inv_linux=2;
    cmds = linuxCommands(false, node, flow, flowID, portName,
			 dscp, rate, burst, filter);
    try
    {
       LinuxUtils._issueCmd(cmds, nodeName, loopback);    
    }
    catch (IOException e)
    {
      logger.error("Error configuring the router");
    }

    logger.info("Linux SL delete: " + cmds);
    return;
  }

  /** Deletes policing and marking information for the specified flow.
   *  For a successful deletion, all the parameters must match
   *  the original add flow.  For now, this is only needed for
   *  Linux and if a stateless implementation is required.
   *
   * @param nodeName   name of the router to be controlled
   * @param portName   interface name (e.g., "Ethernet1/1")
   * @param flowID     flow identifier (from BB database)
   * @param trust      trust (for CatOS QoS)
   * @param dscp       diffserv code point (e.g., "10") for outgoing
   * @param filter     5-tuple rule
   * @param rate       policing rate (bit rate in bits per second)
   * @param burst      maximum allowed burst size in byte
   * @param excessRate tolerated excess
   * @param normal     [not currently used]
   * @param excess     action to take when exceeding BW 
   */
  public void delFlowSL(String nodeName,
		       String portName,
		       String flowID, 
		       TrustState trust, 
		       short dscp,
		       FlowInfo filter, 
		       int rate,
		       int burst,
		       int excessRate, 
		       ExceedAction normal, 
		       ExceedAction excess) {
  
    System.out.println("Deleting Flow (SL): " + flowID);

    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);
    /* To make the FP stateless, node state needs to be obtained on demand, if it is not found*/
    if (node == null) {
	//System.out.println("Unable to find node in delFlow");
	// return;
	try {
	    node=getNodeInfo (nodeName);
	 } catch (IOException ioe) {
	     ioe.printStackTrace();
	 }
    }
    /*if (node == null) {
      // Should already be present
      logger.error("Attempt to delete unknown node");
      return;
      }*/

    // init a new flow
    FlowProvisioningSpec flow = new FlowProvisioningSpec(nodeName,
							 portName, flowID);

    // Non-Linux use a simpler method
    
    if (node.getNodeType().equals("Linux")) {
      try
      {
         delLinuxFlow(nodeName, 
		   portName, 
		   flowID,
		   trust,
		   dscp, 
		   filter,
		   rate, 
		   burst,
		   excessRate,
		   normal,
		   excess);
      }
      catch (IOException ei)
      {
         logger.error ("Could not configure delLinuxFlow");
      }
    } else if (node.getNodeType().equals("CatOS") ||
	node.getNodeType().equals("IOS")) {
      delFlow(nodeName, portName ,flowID);
    } else if (! node.getNodeType().equals("Linux")) {
      logger.error("Unknown node type: " + node.getNodeType());
    }
    return;
  }

  /** Deletes a flow.
   * @param nodeName   name of the flow
   * @param portName   port identifier
   * @param flowID     flow identifier
   */
  public void delFlow (String nodeName, String portName, String flowID) {

    int i;

    System.out.println("Deleting Flow: " + flowID);

    // get the type corresponding to this node 
    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);

    /* To make the FP stateless, node state needs to be obtained on demand, if it is not found*/
    if (node == null) {
	//System.out.println("Unable to find node in delFlow");
	// return;
	try {
	    node=getNodeInfo (nodeName);
	 } catch (IOException ioe) {
	     ioe.printStackTrace();
	 }
    }
   
    if (node.getNodeType().equals("IOS")) {
	
	delIOSFlow(node, flowID, portName);
	return;
    }

    // construct the ACL name
    StringBuffer aclName = new StringBuffer("ACL_" + nodeName + 
					    "_" + portName);

    // pull out any special chars
    while ((i = aclName.indexOf("/")) != -1) {
	aclName = aclName.deleteCharAt(i);
    }
    
    // get the array List
    ArrayList aclEntries = (ArrayList) allACLs.get(aclName.toString());
    
    if (aclEntries == null)
	return;
    FlowProvisioningSpec flow;
    for (i=0; i < aclEntries.size(); i++) {
	
        flow = (FlowProvisioningSpec) aclEntries.get(i);
	if (flow.flowID.equals(flowID)) {
	    
	  // found the flow to delete - construct the delete commands 
	    if (node.getNodeType().equals("CatOS")) {
	    
		delCatOSFlow(node, flowID, aclName, i+1);
		
	    } 
	    
          else if (node.getNodeType().equals("Linux")) {
	      
	      delLinuxFlow(nodeName, flowID, portName);
	      
          } else { 
	      
	    System.out.println("Unknown node type ignored: " +
			       node.getNodeType());
	    
          }
	    
          aclEntries.remove(i);
    	  return;
	}
    }
  }

  // Markdown information - This is the marking for packets that exceed
  // their allocated capacity
  // The markdown command in general is of the form:
  // "mls qos map policed-dscp <dscp_list> to <markdown_dscp>"
  public void addMarkdown (MarkdownType type, 
			short dscp, short markdownDscp, String [] nodes) {

    StringBuffer markdownCmd; 

    
    switch(type.value()) {

      case MarkdownType._NORMAL:
        markdownCmd = new StringBuffer("mls qos map policed-dscp " + dscp +
				       " to " + markdownDscp);
        break;

      case MarkdownType._EXCESS:
        markdownCmd = new StringBuffer("mls qos map exceed-dscp " + dscp +
				       " to " + markdownDscp);
        break;
    }

  }

  // This function adds the named QoS policy to the router with nodeName
  // It also implicitly deletes any existing policies on the router
  public void addQoSPolicy(String nodeName,
			   String[] intfs,
			   String policyName) {

    String [] allPolicies = {"STANDARD_OPS", "BATTLE_MODE"};
    int i, j;

    System.out.println("Adding Policy: "+ policyName + " on node: "+ nodeName);

    FlowProvisioningNodeInfo node = 
		(FlowProvisioningNodeInfo) allNodes.get(nodeName);

    if (node == null) {
      System.out.println("Unknown node");
      return;
    }


    // **** TBD JEB **** --- Linux might differ
    for (i=0; i < intfs.length; i++) { 
      StringBuffer cmd = new StringBuffer ("interface " + intfs[i] + "\r");

      // issue the delete commands, just in case
      for (j=0; j < allPolicies.length; j++) {
        cmd.append("no service-policy output " + allPolicies[j] + "\r");
      }

      _issueCmd(cmd.toString(), node);
    }

    // now add the new policy
    for (i=0; i < intfs.length; i++) { 
      StringBuffer cmd = new StringBuffer ("interface " + intfs[i] + "\r");
      cmd.append("service-policy output " + policyName + "\r");
      _issueCmd(cmd.toString(), node);
    }


  }

    // A similar method is in BBDriver too, got to get this into some common place.: Kirthika    
    static int inv = 0;
    public void tryToFailFP (int when)
    {
	if (failAfterXFP == 0)
	return;
	
	if (when == 1 && inv_linux == 2) // for add Flow linuxcommands () which has tryToFailFP () is called twice
	    inv++;
	
        System.out.println ("when="+when+" failWhen="+failWhenFP+
			    " inv="+inv+" inv_linux="+inv_linux);
       if (inv <= failAfterXFP)
        return;
       if (failWhenFP == when)
       {
          System.out.println ("Failure simulated on invocation #" + inv + "\n"
                            + "Failure type = " + when +" during flow provisioning on routers \n");
	  Runtime r = Runtime.getRuntime ();
          try
	  {
             r.exec ("bin/failpool1.sh");
             System.out.println ("Pool failure invoked.");
             System.exit (1);
	  }
	  catch (Exception e)
	  {
	  }
          //System.exit (1);
       }
    }


} // end class FlowProvisioningImpl
