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

import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.FaultMonitorException;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.NEeventType;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.switchInfoHolder;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer.FaultMonitorLocalImpl;

// Additional imports for the arguments to the methods

public class FaultMonitorComponentImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.FMSessionComposition.ComponentImpl
{

    private FaultMonitorLocalImpl _delegate;

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
    FaultMonitorComponentImpl()
    {
	    _delegate = new FaultMonitorLocalImpl();
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

         System.err.println("FM pushing EVENT");
         get_context().push_toFaultEventSubscribers(new BBInternalEventImpl());

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
    //         mil::darpa::arms::mlrm::BandwidthBroker::FaultMonitor
    //
    // ==================================================================

  /** Fetch the Switch Status from SNMP */
  public void getSwitchStatus (String swName, switchInfoHolder info) 
		throws FaultMonitorException 
  {

	_delegate.getSwitchStatus ( swName, info );

  }

  /** Subscribe to a Fault Monitor Event */
  public void subscribeToFaultEvent (String eventSinkName, 
	String cookie, String[] switches, 
	NEeventType type) throws FaultMonitorException 
  {
	_delegate.subscribeToFaultEvent ( eventSinkName, cookie, switches, type );
  }

  /**  Unsubscribe a component for fault monitor events */
  public void unsubscribeToFaultEvent (String eventSinkName, 
	String cookie) throws FaultMonitorException
  {
	_delegate.unsubscribeToFaultEvent ( eventSinkName, cookie );
  }

} // end class FaultMonitorComponentImpl
