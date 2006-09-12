// $Id$
//
// The DARPA ARMS Program
//
// The software supporting this interface is governed by Government Purpose 
// Rights
//
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// Comments:  

// Authors: Kirthika Parmeswaran <kirthika@research.telcordia.com>,
//          Balakrishnan Dasarathy <das@research.telcordia.com>,
//	      Murray Conarty <murray.conarty@prismtechnologies.com>,
//          Shree Gadgil <sgadgil@research.telcordia.com>
//          Rick Porter <sgadgil@research.telcordia.com>

//
// Date:   August 28, 2005

package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.ExceedAction;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.FlowInfo;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.MarkdownType;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.TrustState;
import mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.*;

// More imports needed for the data types used as arguments

public class FlowProvisionerComponentImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.FPSessionComposition.ComponentImpl
{

    private FlowProvisioningLocalImpl _delegate;

    // ==================================================================
    //
    // Constructor.
    //
    // ==================================================================

    /**
     * The default constructor.
     *
     * Initialize with concrete implementation class.
     */
    public
    FlowProvisionerComponentImpl()
    {
    	System.err.println("In FP Component Constructor");
	    _delegate = new FlowProvisioningLocalImpl();
    }

    // ==================================================================
    //
    // Internal methods.
    //
    // ==================================================================


    // ==================================================================
    //
    // Public methods.
    //
    // ==================================================================


    // ==================================================================
    //
    // Methods for OMG IDL Components::EnterpriseComponent
    //
    // ==================================================================

    /**
     * Complete the component configuration.
     *
     * @exception org.omg.Components.InvalidConfiguration
     *            Thrown if the configuration is invalid.
     */
    public void
    configuration_complete()
    throws org.omg.Components.InvalidConfiguration
    {

         if ( _delegate ==  null ) 
		   throw new org.omg.Components.InvalidConfiguration();
	
	
  	    boolean loopback;
 	    String key;

 	    Auth auth = new Auth();

 	    while(auth.completed == 0)
 	    {
 	    	try 
	 		{
	 		  Thread.sleep (500);
	 		}
	 		catch(Exception exx)
	 		{
	
	 		}
	 	}

 	    key = auth.keyval;
 	    loopback = auth.loopBack;
 	    String authFile = "/routerAuth";

 	    _delegate.initAuth(authFile,key);	    
             
 	    
 	    if(loopback)
 		  _delegate.setLoopback();
	 	     
		// Check for valid configuration here
    }

    // ==================================================================
    //
    // Methods for the OMG IDL org.omg.Components.SessionComponent
    //
    // ==================================================================

    /**
     * Container callback to signal that the component is removed.
     *
     * @throw org.omg.Components.CCMException For any problems.
     */
    public void
    ccm_remove()
    throws org.omg.Components.CCMException
    {
    }

    // ==================================================================
    //
    // Methods for OMG IDL interface:
    //         mil::darpa::arms::mlrm::BandwidthBroker::FlowProvisioner
    //
    // ==================================================================


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
		       ExceedAction excess) 
  {

	  System.err.println ("In FlowProvisionerComponentImpl add flow");
	_delegate.addFlow ( nodeName, portName, flowID, trust, dscp, filter, rate, burst,
			excessRate, normal, excess );

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
		       ExceedAction excess) 
  {

	_delegate.delFlowSL ( nodeName, portName, flowID, trust, dscp, filter, rate, burst,
			excessRate, normal, excess );  
  }

  /** Deletes a flow.
   * @param nodeName   name of the flow
   * @param portName   port identifier
   * @param flowID     flow identifier
   */
  public void delFlow (String nodeName, String portName, String flowID) 
  {

	_delegate.delFlow ( nodeName, portName, flowID);

  }

  // Markdown information - This is the marking for packets that exceed
  // their allocated capacity
  // The markdown command in general is of the form:
  // "mls qos map policed-dscp <dscp_list> to <markdown_dscp>"
  public void addMarkdown (
		    MarkdownType type, 
			short dscp, 
			short markdownDscp, 
			String [] nodes) 
  {

	_delegate.addMarkdown ( type, dscp, markdownDscp, nodes );


  }

  // This function adds the named QoS policy to the router with nodeName
  // It also implicitly deletes any existing policies on the router
  public void addQoSPolicy(String nodeName,
			   String[] intfs,
			   String policyName) 
  {

	_delegate.addQoSPolicy ( nodeName, intfs, policyName );


  }

} // end class FlowProvisioningImpl
