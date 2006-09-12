
package  mil.darpa.arms.mlrm.BandwidthBroker.cif;
import  mil.darpa.arms.mlrm.BandwidthBroker.*;
import  mil.darpa.arms.mlrm.*;

import java.io.*;
import org.omg.CORBA.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

public class ClientImpl extends  mil.darpa.arms.mlrm.BandwidthBroker.ClientSessionComposition.ComponentImpl
{
    // ==================================================================
    //
    // Internal state.
    //
    // ==================================================================

    /** The name of the component. */
    private String name_;
    private javax.swing.JFrame frame_;

    /** To refer to the GUI output area. */
    private javax.swing.JTextArea textArea_;


    public
    ClientImpl()
    {
	System.out.println("Client Instance");
    }

    // ==================================================================
    //
    // Methods for OMG IDL Components::EnterpriseComponent
    //
    // ==================================================================

    static boolean once = true;
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
       try
       {
          initORB ();
	  if (once == true)
	  {
             makeInitialBWRequest ();
	     once = false;
	  }
       }
       catch (Exception e)
       {
          System.out.println("Initial BB request failed");
       }

	System.out.println("CLient Configuration COmplete");
        // Checks if the configuration is completed.
        if(name_ == null)
            throw new org.omg.Components.InvalidConfiguration();

        // Check if the connection to the server is set.
/*
        if(get_context().get_connection_ac_server() == null)
            throw new org.omg.Components.InvalidConfiguration();
	        if(get_context().get_connection_fp_server() == null)
            throw new org.omg.Components.InvalidConfiguration();

*/
      frame_ = new javax.swing.JFrame(name_ + "'s Consumer GUI");
        // Sets its size.
        frame_.setSize(400, 300);

        // Creates a text area for displaying inputs.
        textArea_ = new javax.swing.JTextArea(40, 20);
        textArea_.setEditable(false);

        // Constructs and shows the GUI.
        javax.swing.JPanel panel = new javax.swing.JPanel(
                                       new java.awt.BorderLayout()
                                   );
        frame_.getContentPane().add(panel);
        panel.add(new javax.swing.JScrollPane(textArea_),
                  java.awt.BorderLayout.CENTER);
        frame_.pack();
        frame_.show();


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
      frame_.dispose();
        frame_ = null;

    }

    // ==================================================================
    //
    // Methods for the OMG IDL demo1::CCM_Client_Executor
    //
    // ==================================================================

    /**
     * The mutator method for the attribute name.
     *
     * @param n The name.
     */
    public void
    the_name(String n)
    {
        name_ = n;
        if (textArea_ != null)
           textArea_.append("call received on the_name");        
	System.out.println("Setting the name to:" + n);
    }

    /**
     * The accessor method for the attribute name.
     *
     * @return The name.
     */
    public String
    the_name()
    {
        return name_;
    }

    static int count = 0;
    public void push(Condition event)
    {
       System.out.println("Received event");
       textArea_.append(event.description);
       try
       {
          if (count > 3)
	  {
             makeModifyRequest ();
	     count = 0;
	  }
          else
             count ++;
       }
       catch (Exception e)
       {
          System.out.println("Modify request failed");
       }

    }
/*
    public void push(BWNotification event)
    {
       textArea_.append("Event Received");        

    }
*/

    AdmissionControl adc;
    void initORB () throws Exception
    {
       String bbName = "mil/darpa/arms/mlrm/BandwidthBroker/AdmissionControl";
       String args [] = { "-ORBInitRef", "NameService=corbaloc::localhost:5001/NameService"};
       org.omg.CORBA.Object obj;
       ORB orb = ORB.init (args, null);
       try
       {
	  NamingContextExt rootContext;
          obj = orb.resolve_initial_references ("NameService");
	  rootContext = NamingContextExtHelper.narrow (obj);
          System.out.println ("NameService resolved successfully");

	  obj = rootContext.resolve_str (bbName);
	  adc = AdmissionControlHelper.narrow (obj);
          System.out.println ("AdmissionControl obtained successfully");

       }
       catch (Exception e)
       {
          System.out.println ("Error locating BandwidthBroker");
	  e.printStackTrace ();
	  
       }
    }

    void makeInitialBWRequest () throws Exception
    {
       System.out.println ("Making initial request");
       String sIP = "mako"; 
       String dIP = "javelin"; 
       String smask = ""; 
       String dmask = "";
                                                                                
       short pn1 = -1;
       short pn2 = -1;
       PortInfo sPort = new PortInfo (pn1,pn1); //low,high same
       PortInfo dPort = new PortInfo (pn2,pn2);
       BBProtocolType protocol = BBProtocolType.notSpecified; //tcp
       int fwdRate = 1000;
       int flowDuration = 1000;
       AdmissionControlResult r =null;


       System.out.println ("Filling flow details");
       FlowInfo uniFI = new FlowInfo (new IPAddress (sIP,smask),
                                      sPort,
                                      new IPAddress (dIP,dmask),
                                      dPort,
                                      protocol,
                                      new BandwidthRate (fwdRate,10),
                                      new BandwidthRate (0,0),
                                      false,
                                      flowDuration);
                                                                                
       QOSRequired qos = QOSRequired.highReliability;
       SchedulingAction schAction
            = SchedulingAction.reserve_and_commit; //reserve and actiavte the flow
       StringHolder fID = new StringHolder();
       IntHolder dscp = new IntHolder ();
                                                                                
       System.out.println ("Filled flow details. About to make a call");
       // Create new flow, flowID is obtained
       r = adc.flowRequest (uniFI, qos, schAction, fID, dscp);

       if (r == AdmissionControlResult.DECISION_ADMIT)
       {
          System.out.println (" Client flow request for flowID:"+fID.value
                                +" result= flow request admitted with dscp = " +                                dscp.value);
          _fid = fID.value;
          _rate = fwdRate;
       }
       else
          System.out.println (" Client flow request for flowID:"+fID.value
            +" result= flow request denied");
       
    }

    String _fid; int _rate;

    void makeModifyRequest () throws Exception
    {
       AdmissionControlResult r = null;
       int newRate = _rate + 100;

       // Now modify the flow using the same flowID
       r = adc.unidirectionalFlowUpdate(_fid, 0, newRate + 100);
       if (r == AdmissionControlResult.DECISION_ADMIT)
       {
       
          System.out.println ("Client flow modify request for flowID: "+
                               _fid + " new rate: " + newRate +
                               " result= flow modified");
          _rate = newRate;
       }
       else
          System.out.println ("Client flow delete request for flowID:" +
                               _fid +" new rate: " + newRate +
                               " result= modify denied" );

    }

}
