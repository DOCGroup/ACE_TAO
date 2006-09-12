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

// More imports needed here for the datatypes passed as arguments

public class FlowProvisionerComponentHomeImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.FPSessionComposition.HomeImpl
{
    // ==================================================================
    //
    // Internal state.
    //
    // ==================================================================

    // ==================================================================
    //
    // Constructor.
    //
    // ==================================================================

    /** The default constructor. */
    public
    FlowProvisionerComponentHomeImpl()
    {

          //    Not needed anymore - revisit MRC
/*
          String bbServerLogFile = new String (rootDir + System.getProperty ("file.separator") 
					  +  "proto" +  System.getProperty ("file.separator") +
					  "cfg" + System.getProperty ("file.separator") +
						"bbserverlog.prop" );

          PropertyConfigurator.configure(bbServerLogFile);
*/
	    // Create a logger for this type of component
	    //     All subsequent calls with this class name will get this instance
	    // Logger bblogger = Logger.getLogger("arms.mlrm.FlowProvisioner");
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
    // Methods for creation of component segments.
    //
    // ==================================================================

    /**
     * Create an executor segment from its identifier.
     *
     * @param segid The executor segment identifier.
     */
    public org.omg.Components.ExecutorSegmentBase
    create_executor_segment(int segid)
    {

	    // This method is called from the HomeBase create() when a component is
	    //     created 
	  
          return new FlowProvisionerComponentImpl();
    }

    // ==================================================================
    //
    // Methods for OMG IDL CCM_FlowProvisionerHome
    //
    // ==================================================================

    // ==================================================================
    //
    // Methods for the deployment.
    //
    // ==================================================================

    /**
     * This method is called by the OpenCCM Component Server
     * to create a home instance.
     */
    public static org.omg.Components.HomeExecutorBase
    create_home()
    {
    	System.err.println( "In Component Home");
        return new FlowProvisionerComponentHomeImpl();
    }
}
