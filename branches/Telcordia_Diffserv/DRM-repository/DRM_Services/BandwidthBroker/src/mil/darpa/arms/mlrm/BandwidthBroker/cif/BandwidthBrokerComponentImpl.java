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
//		Ravi Vaidyanathan <sgadgil@research.telcordia.com>
//          Rick Porter <sgadgil@research.telcordia.com>

//
// Date:   August 28, 2005

package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import mil.darpa.arms.mlrm.Condition;
import mil.darpa.arms.mlrm.BandwidthBroker.BandwidthBrokerInternalEvent;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.AdmissionControlLocalImpl;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlException;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlResult;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BWBetnPoolsSeqHolder;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BWBetnSubnetsSeqHolder;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BWPercentPerQoS;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BWQueryType;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BandwidthPolicy;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.FlowInfo;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.SchedulingAction;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.switchInfo;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.QOSRequired;

public class BandwidthBrokerComponentImpl extends
		mil.darpa.arms.mlrm.BandwidthBroker.BBSessionComposition.ComponentImpl {

	private AdmissionControlLocalImpl _delegate;
	private BBConditionImpl _currentStatus = new BBConditionImpl();

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
	public BandwidthBrokerComponentImpl() {
		try {
	
			System.err.println( "In BB Component Constructor");
			_delegate = new AdmissionControlLocalImpl();
			
		} catch (AdmissionControlException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	// ==================================================================
	//
	// Internal methods.
	//
	// ==================================================================

	// ==================================================================
	//
	// Public methods for CCM Component 
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
	 * 
	 * @exception org.omg.Components.InvalidConfiguration
	 *                Thrown if the configuration is invalid.
	 */
	public void configuration_complete()
			throws org.omg.Components.InvalidConfiguration {

		if (_delegate == null)
			throw new org.omg.Components.InvalidConfiguration();
		
		   if (_delegate.getBBDriver().getConfigureNw() == 1)
		   {
			   _delegate.getBBDriver().connectToFlowProvisioner(
					   get_context().get_connection_toFlowProvisioner());
			   
		   }
		   // Now ok to initialize the L2 paths
		   _delegate.initL2PathManager();
			   

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
	public void ccm_remove() throws org.omg.Components.CCMException {
	}

	// ==================================================================
	//
	// Methods for OMG IDL interface:
	// mil::darpa::arms::mlrm::BandwidthBroker::AdmissionControl
	//
	// ==================================================================

	public AdmissionControlResult flowRequest(
			FlowInfo theFlowInfo,
			QOSRequired qos, 
			SchedulingAction schAction, 
			org.omg.CORBA.StringHolder flowID,
			org.omg.CORBA.IntHolder dscp) 
	    throws AdmissionControlException {
	    return flowRequest_ft(theFlowInfo, qos,schAction, flowID,
			       dscp, "");
	}

	public AdmissionControlResult flowRequest_ext(
			FlowInfo theFlowInfo,
			QOSRequired qos, 
			SchedulingAction schAction, 
			org.omg.CORBA.StringHolder flowID,
			org.omg.CORBA.IntHolder dscp,
			int priority,
			String reference) 
	    throws AdmissionControlException {

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.flowRequest_ext(theFlowInfo, qos, schAction, flowID,
				dscp, priority, reference);
		
		// Build the Condition and push to subscribers
		// Put this anywhere you need to push an event
        get_context().push_toBBEventSubscribers ( _currentStatus );


		return result;

	}

	public AdmissionControlResult flowRequest_ft(
			FlowInfo theFlowInfo,
			QOSRequired qos, 
			SchedulingAction schAction, 
			org.omg.CORBA.StringHolder flowID,
			org.omg.CORBA.IntHolder dscp,
			String reference) 
	    throws AdmissionControlException {

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.flowRequest_ft(theFlowInfo, qos, schAction, flowID,
				dscp, reference);
		
		// Build the Condition and push to subscribers
		// Put this anywhere you need to push an event
        get_context().push_toBBEventSubscribers ( _currentStatus );


		return result;

	}

	// Operation to commit the request with the flow token identifying
	// the original request

	public AdmissionControlResult flowRequestCommit(String flowID)
			throws AdmissionControlException {
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.flowRequestCommit(flowID);

		return result;
	}

	// For deleting an application flow
	// Input parameter:
	// flowToken: flowToken that identifies the flow/s that are to be
	// deleted (for bi-directional case, both forward and
	// reverse flows will be deleted based on the same token)
	// Return value is a message with one of the following responses:
	// NOFLOWID, BADDATA, ERROR on error else DELETE

	public AdmissionControlResult flowDelete(String flowID)
			throws AdmissionControlException {
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.flowDelete(flowID);

		return result;

	}

	// Flow Update
	// Input parameters are:
	// flowToken : flowToken that identifies the flow that is to be updated
	// oldFlowInfo : Orginal flow request information for validation
	// purposes
	// newFlowInfo : Flow information for updation
	// oldQoS : Original QoS properties required for validation
	// purposes
	// newQoS : QoS properties for updation
	// Return message could be one of the following:
	// DECISION_ADMIT, DECISION_DENY, DECISION_REV_DENY,
	// DUPLICATE_REQ,
	// NOFLOWID, BADDATA, ERROR or REQ_DROP

	public AdmissionControlResult unidirectionalFlowUpdate(String flowID,
			int oldfwdRate, int newfwdRate) throws AdmissionControlException {

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.unidirectionalFlowUpdate(flowID, oldfwdRate, newfwdRate);
		return result;
	}

	public AdmissionControlResult bidirectionalFlowUpdate(String flowID, 
						   int oldfwdRate, 
						   int newfwdRate, 
						   int revRate, 
						   int newRevRate) 
	throws AdmissionControlException
    {

		AdmissionControlResult result;

  		// simply call the implementation
		result = _delegate.bidirectionalFlowUpdate ( flowID,
							   	oldfwdRate,
								newfwdRate,
								revRate,
								newRevRate );
		return result;
    }	

	public AdmissionControlResult flowQosUpdate(String flowID,
			QOSRequired oldQos, QOSRequired newQos)
			throws AdmissionControlException {

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.flowQosUpdate(flowID, oldQos, newQos);
		return result;
	}

	// Query for actual allocated bandwidth for a flow based on the
	// flow token. The forward and reverse bandwidth rates are
	// returned.

	public AdmissionControlResult allocatedFlowBandwidthQuery(String flowToken,
			org.omg.CORBA.IntHolder fwdBW, org.omg.CORBA.IntHolder revBW)
			throws mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlException {

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.allocatedFlowBandwidthQuery(flowToken, fwdBW, revBW);
		return result;
	}

	// Query for available bandwidth between subnets.
	// Target bandwidth required between the
	// source and destination subnets can be specified.
	// This method returns the
	// target bandwidth (unchanged) per service class, if it can be met
	// If it cannot be met, or if input target value specified is zero for a
	// service class,
	// the bandwidth returned is the remaining bandwidth divided equally among
	// all
	// flows exercising the same path/subpath.
	// Currently, service classes supported are expedited
	// forwardinghihgPriority, and
	// HighReliablity and HighThroughputmmVoice and mmVideo Assured Forwarding
	// classes

	// The operation supports taking in a whole series of source and destination
	// subnets,
	// so that Bandwidth Broker can determine atomically
	// whether such allocation is possible
	// among the pairs of subnets, taking into account the fact that
	// paths/subpaths may be
	// shared by the traffic between subnet pairs.
	// At the end of the operation, no bandwidth is reserved
	// or committed. So, if a sequence of these operations is issued, the
	// Bandwidth Broker treats each request as a new one

	// This query can also be used to find physical bandwidth or provisioned
	// bandwidth for a specified class. type parameter should be set to
	// provsioned

	public AdmissionControlResult subnetbandwidthQuery (
			BWBetnSubnetsSeqHolder subnetBandwidthInfo, 
			BWQueryType type)
			throws mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlException 
	{

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.subnetbandwidthQuery(subnetBandwidthInfo, type);
		return result;
	}

	// Query for available bandwidth between pools.
	// Target bandwidth required between the
	// source and destination pools can be specified.
	// This method returns the
	// target bandwidth (unchanged) per service class, if it can be met
	// If it cannot be met, or if input target value specified is zero for a
	// service class,
	// the bandwidth returned is the remaining bandwidth divided equally among
	// all
	// flows exercising the same path/subpath.
	// Currently, service classes supported are highPriority,
	// HighReliablity and mmVoice and mmVideo.

	// The operation supports taking in a whole series of source and destination
	// pools,
	// so that Bandwidth Broker can determine atomically
	// whether such allocation is possible
	// among the pairs of pools, taking into account the fact that
	// paths/subpaths may be
	// shared by the traffic between pool pairs.
	// At the end of the operation, no bandwidth is reserved
	// or committed. So, if a sequence of these operations is issued, the
	// Bandwidth Broker treats each request as a new one
	public AdmissionControlResult poolbandwidthQuery(
			BWBetnPoolsSeqHolder poolBandwidthInfo, 
			BWQueryType type)
			throws AdmissionControlException 
	{

		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.poolbandwidthQuery(poolBandwidthInfo, type);
		return result;
	}

	// Policy action used by the Infrastructure Allocator to change
	// the allocation of bandwidth to various service classes This will result
	// in switches and routers to be
	// reconfigured during operation.
	// As such, ensure care
	// before invoking this operation.
	// The total percentage should be less than hundred.
	// No need to specify Best Effort class or its bandwidth %

	public AdmissionControlResult allocateBandwidthPercentPerQoS 
	     ( BWPercentPerQoS[] bwPercentageSeq ) 
	    throws AdmissionControlException 
	{
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.allocateBandwidthPercentPerQoS(bwPercentageSeq);

		return result;
	}

	public AdmissionControlResult changeBandwidthAllocationPolicyTo
	     ( BandwidthPolicy bwPolicy) 
	    throws AdmissionControlException 
	{
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.changeBandwidthAllocationPolicyTo(bwPolicy);

		return result;
	}

	public AdmissionControlResult standby() throws AdmissionControlException {
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.standby();

		return result;

	}

	public AdmissionControlResult resume() throws AdmissionControlException {
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.resume();

		return result;

	}

	public AdmissionControlResult processNetworkEvent(switchInfo f) {
		AdmissionControlResult result;

		// simply call the implementation
		result = _delegate.processNetworkEvent(f);

		return result;

	}

	// for the Event Sink ('consumes')
	public void push(BandwidthBrokerInternalEvent event) 
	{
		// TODO Auto-generated method stub
		
//		System.err.println( "Received Bandwidth Broker Internal Event: "+
//				event.nmEvent.cookie);
		
	}

}
