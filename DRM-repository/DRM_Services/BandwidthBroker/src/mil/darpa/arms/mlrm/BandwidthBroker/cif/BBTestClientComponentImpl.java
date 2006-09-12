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

import java.io.*;

import mil.darpa.arms.mlrm.Condition;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControl;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlResult;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BBProtocolType;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BandwidthPolicy;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.BandwidthRate;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.FlowInfo;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.SchedulingAction;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.IPAddress;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.PortInfo;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.QOSRequired;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.BBServer;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.omg.CORBA.IntHolder;
import org.omg.CORBA.StringHolder;

// Additional imports for the arguments to the methods

public class BBTestClientComponentImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.TestClientSessionComposition.ComponentImpl
{
    // ==================================================================
    //
    // Internal state.
    //
    // ==================================================================

    /** The name of the component. */
    private String testName = "";
    
    private String propertyFile = "";
    
    /** Arguments (from property file) for the test */
	private String action = "";
	private int rate = 0;
	private String flowID = "";
	private String policy = "";
	private String srcIP = null, destIP = null;
	private String qos = "";


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
    BBTestClientComponentImpl()
    {
    	System.err.println("In BBTestClientComponentImpl cstrct");
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
       //if(get_context().get_connection_theBandwidthBroker() == null)
          //  throw new org.omg.Components.InvalidConfiguration();

    	invokeTest();
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
    /**
     * The mutator method for the attribute name.
     *
     * @param n The name.
     */
    public void
    testName(String n)
    {
    	System.err.println("In BBTestClientComponentImpl setting testName");
        testName = n;

    }

    /**
     * The accessor method for the attribute name.
     *
     * @return The name.
     */
    public String
    testName()
    {
//    	System.err.println("In BBTestClientComponentImpl getting testName");
        return testName;
    }

    /**
     * The mutator method for the attribute name.
     *
     * @param n The name.
     */
    public void
    propertyFile(String n)
    {
    	System.err.println("In BBTestClientComponentImpl setting propertyFile");
        propertyFile = n;

    }

    /**
     * The accessor method for the attribute name.
     *
     * @return The name.
     */
    public String
    propertyFile()
    {
        return propertyFile;
    }

	public void push(Condition event) {
		// TODO Auto-generated method stub
		System.err.println("Received Condition event");
		
	}
	
    // ==================================================================
    //
    // Internal methods.
    //
    // ==================================================================



	public void invokeTest()
	{
		
	  // Connect to BandwidthBroker Component Facet
	  AdmissionControl adControl = 
	    	  get_context().get_connection_toBandwidthBroker();
			   
		
	  parseConfigFile();
	  
	  
		
	  // invoke the operation and print the result
	  //String srcIP = "192.168.81.100";
	  String smask="255.255.255.0";
	  //String destIP = "192.168.82.101";
	  String dmask="255.255.255.0";

	  //      srcIP = "mako"; destIP = "javelin"; 
	  //smask = ""; dmask = "";
	  
	  // invoke the operation and print the result
	  //short pn1 = 20;
	  short pn1 = -1;
	  short pn2 = -1;
	  PortInfo sPort = new PortInfo (pn1,pn1); //low,high same
	  PortInfo dPort = new PortInfo (pn2,pn2);
	  BBProtocolType protocol = BBProtocolType.tcp; //tcp
	  int fwdRate = 1000;
	  int flowDuration = 1000;
	
	  // Several exceptions thrown related to flowRequest
      try {
    	  
	  AdmissionControlResult r =null;
	  if (action.equals ("add"))
	  {
	     fwdRate = rate;
	     FlowInfo uniFI = new FlowInfo (new IPAddress (srcIP,smask),
					 sPort, 
					 new IPAddress (destIP,dmask), 
					 dPort,
					 protocol, 
					 new BandwidthRate (fwdRate,10), 
					 new BandwidthRate (0,0), 
					 false,
					 flowDuration);
		  
		  QOSRequired qosr = findQoS (qos);
		  SchedulingAction schAction 
		    = SchedulingAction.reserve_and_commit; //reserve and actiavte the flow
		  StringHolder fID = new StringHolder();
	          IntHolder dscp = new IntHolder ();
		

		  // Create new flow, flowID is obtained

		    r = adControl.flowRequest (uniFI, qosr, schAction, fID, dscp);

		  if (r == AdmissionControlResult.DECISION_ADMIT)
		    System.err.println (" Client flow request for flowID:"+fID.value
					+" result= flow request admitted with dscp = " +
	                                dscp.value);
		  else
		    System.err.println (" Client flow request for flowID:"+fID.value
		    +" result= flow request denied");
		  
	   }
	   else if (action.equals ("del"))
	   {
		  
		     // Now delete the flow using the same flowID
		     r = adControl.flowDelete (flowID);
		     if (r == AdmissionControlResult.DECISION_DELETE)
		       System.err.println (" Client flow delete request for flowID:"+
	                                   flowID + 
					   " result= flow deleted");
		     else
		       System.err.println (" Client flow delete request for flowID:" +
	                                   flowID +
		                        " result= no flow ID");
		}
		else if (action.equals ("mod"))
		{
		  
		     // Now delete the flow using the same flowID
		     r = adControl.unidirectionalFlowUpdate(flowID, 0, rate);
		     if (r == AdmissionControlResult.DECISION_ADMIT)
		       System.err.println (" Client flow modify request for flowID: "+
	                                   flowID + " new rate: " + rate +  
					   " result= flow modified");
		     else
		       System.err.println (" Client flow delete request for flowID:" +
	                                   flowID +" new rate: " + rate +  
		                        " result= modify denied" );
		}
		else if (action.equals ("policy"))
		{
	  
		      r = null;
		      if (policy.equals ("1"))		     
		         r  = adControl.changeBandwidthAllocationPolicyTo (BandwidthPolicy.policy1);
		      else if (policy.equals ("2"))
		         r  = adControl.changeBandwidthAllocationPolicyTo (BandwidthPolicy.policy2);
		      if (r != null)
		      {
		          if (r == AdmissionControlResult.DECISION_ADMIT)
		        	  	System.err.println (" Changed to bandwidth allocation policy:"+
		        	  			new Integer (BandwidthPolicy.policy2.value ()).toString());
		          else
		        	  	System.err.println (" Failure in changing to bandwidth allocation policy:"+
		        	  			new Integer (BandwidthPolicy.policy2.value ()).toString());
		      }
		 }
		 else if (action.equals ("query"))
	     {
	              //testBWQuery (adControl);
	     }
		 else if (action.equals ("standby"))
	     {
		       r = adControl.standby ();
		       System.err.println ("Released all the flows. Bandwidth Broker is now in standby mode.");
	     }
		 else if (action.equals ("resume"))
	     {
		       r = adControl.resume ();
		       System.err.println ("Bandwidth Broker operations have been resumed.");
	     }
	  
      }
      catch (Exception e)
	  {
		  e.printStackTrace();
	  }
}
 


	   private void parseConfigFile()
	   {
		   	// want to be able to take an absolute file name from GUI, but
		   	// also need to be able to do the initial thing with a default file
		   	// which comes from the archive.  Soln'n is to copy the default file
		   	// to /tmp if there is not an existing file of the same name.
		   	
		   if (propertyFile != null)
		   {
		        java.util.Properties testProps = new java.util.Properties();
		        try {
		        	if (!(new File(propertyFile).exists())) {
		        		// the file doesn't exist, so create it in tmp from
		        		// the version in the archive
		        		propertyFile = "/tmp/testParams.prop";
		        		System.err.println("Using property file " +	propertyFile);
		        		if (!(new File(propertyFile).exists())) {
			        		System.err.println("creating " + propertyFile);
			        		BufferedInputStream is = new BufferedInputStream(this.getClass().getResourceAsStream("/testParams.prop"));
			        		OutputStream os = new FileOutputStream(propertyFile);
			        		byte buf[] = new byte[100];
			        		for (int n = is.read(buf); n > 0; n = is.read(buf)) {
			        			os.write(buf, 0, n);
			        		}
			        		os.close();
		        		}		        		
		        	}
					testProps.load(new FileInputStream(propertyFile));
				} catch (IOException e) {
					// TODO Auto-generated catch block
					System.err.println ("Bad Test Properties File: " + propertyFile);
					return;
				}

			    java.util.Enumeration keys = testProps.keys();
				while ( keys.hasMoreElements()  )
				{
					String key = (String)keys.nextElement();
					if ( key.equals("a") )
						action = testProps.getProperty("a");
					else if ( key.equals("r"))
					    rate = Integer.parseInt(testProps.getProperty("r"));
					else if (key.equals ("f"))
					    flowID = testProps.getProperty("f");
					else if (key.equals ("p"))
					    policy = testProps.getProperty("p");
					else if (key.equals ("s"))
					    srcIP = testProps.getProperty("s");
					else if (key.equals ("d"))
					    destIP = testProps.getProperty("d");
					else if (key.equals ("q"))
					    qos = testProps.getProperty("q");
					
				}
				
				System.err.println ("Action is " + action);				
				System.err.println ("Rate is " + rate);
				System.err.println ("FlowID is " + flowID);
				System.err.println ("Policy is " + policy);
				System.err.println ("SrcIP is " + srcIP);
				System.err.println ("DestIP is " + destIP);
				System.err.println ("Qos is " + qos);
				
		   }  // End if propertyFile !=null
	   }
	   
	   private QOSRequired findQoS (String qos)
	   {
	      if (qos.equals ("highPriority"))
	         return QOSRequired.highPriority;
	      else if (qos.equals ("mmVoice"))
	         return QOSRequired.mmVoice;
	      else if (qos.equals ("mmVideo"))
	         return QOSRequired.mmVideo;
	      else if (qos.equals ("highReliability"))
	         return QOSRequired.highReliability;
	      return QOSRequired.bestEffort;
	   }


} // end class BBTestClientComponentImpl
