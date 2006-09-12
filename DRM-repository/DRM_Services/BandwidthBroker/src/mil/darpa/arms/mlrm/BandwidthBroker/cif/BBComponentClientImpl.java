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

import mil.darpa.arms.mlrm.Condition;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

// Additional imports for the arguments to the methods

public class BBComponentClientImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.ClientSessionComposition.ComponentImpl
{

    // ==================================================================
    //
    // Constructor.
    //
    // ==================================================================

    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
     * The default constructor.
     *
     * Initialize with concrete implementation class.
     */
    public
    BBComponentClientImpl()
    {
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

        // Check if the connection to the server is set.
        if(get_context().get_connection_toBandwidthBroker() == null)
            throw new org.omg.Components.InvalidConfiguration();

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

	public void push(Condition event) {
		// TODO Auto-generated method stub
		
	}

    // ==================================================================
    //
    // Methods for OMG IDL interface:
    //         mil::darpa::arms::mlrm::BandwidthBroker::FaultMonitor
    //
    // ==================================================================

} // end class BBComponentClientImpl
