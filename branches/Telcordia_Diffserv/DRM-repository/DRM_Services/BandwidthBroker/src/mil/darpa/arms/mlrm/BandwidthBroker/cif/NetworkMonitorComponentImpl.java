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

import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.MeasurementEndpointsHolder;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.MeasurementEndpointsType;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.NMEventCondition;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.NMEventConditionType;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.NetworkMonitorEventSubscriptionException;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.NetworkProbesException;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.NetworkMonitorLocalImpl;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

// Add imports for the method argument types

public class NetworkMonitorComponentImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.NMSessionComposition.ComponentImpl
{

    private NetworkMonitorLocalImpl _delegate;

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
    NetworkMonitorComponentImpl()
    {
    	System.err.println("In NetworkMonitorComponentImpl Constructor");
	    try {
			_delegate = new NetworkMonitorLocalImpl();
		} catch (NetworkProbesException e) {
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

         if ( _delegate == null ) 
		throw new org.omg.Components.InvalidConfiguration();

         System.err.println("NM pushing EVENT");
         get_context().push_toNetworkEventSubscribers(new BBInternalEventImpl());
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
    //         mil::darpa::arms::mlrm::BandwidthBroker::NetworkMonitor
    //
    // ==================================================================

    /** Get network performace measurement such as latency (in usecs), jitter,
     * loss etc. Currently it only returns latency */
    public void get_measurement(MeasurementEndpointsType type, 
								MeasurementEndpointsHolder latencyInfo) 
	throws NetworkProbesException
    {
	   //_delegate.get_measurement ( type, latencyInfo );
    }

    public void subscribeForLatencyEvent(String consumerComponentName, 
					 MeasurementEndpointsType endpointType, 
					 MeasurementEndpointsHolder latencyInfo, 
					 NMEventConditionType conditionType,
					 NMEventCondition condition, 
					 String cookie) 
	throws NetworkMonitorEventSubscriptionException,NetworkProbesException
    {
	  // _delegate.subscribeForLatencyEvent (consumerComponentName, 
		//					   endpointType,
		//					   latencyInfo,
		//					   conditionType,
		//					   condition,
		//					   cookie );

    }


    public void unsubscribeForLatencyEvent(String consumerComponentName, 
					   String cookie) 
	throws NetworkMonitorEventSubscriptionException
    {
	   //_delegate.unsubscribeForLatencyEvent (consumerComponentName, 
       //      					     cookie );


    }




} // end class NetworkMonitorComponentImpl
