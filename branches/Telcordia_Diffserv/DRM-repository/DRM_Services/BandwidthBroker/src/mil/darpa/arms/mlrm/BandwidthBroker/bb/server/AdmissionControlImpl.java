//$Id: AdmissionControlImpl.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// AdmissionControlImpl.java

// Author : Kirthika Parmeswaran 
// This class is the CORBA servant implementation of the BandwidthBroker.idl

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;


import java.io.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import mil.darpa.arms.mlrm.BandwidthBroker.L2Disc.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;

public class AdmissionControlImpl extends AdmissionControlPOA
{

    BBDriver bbDriver = null;
    boolean standByMode = false;

    public AdmissionControlImpl (String bbConfigFile, String [] args)

	throws AdmissionControlException
    {
	try
	{
	   bbDriver = new BBDriver (bbConfigFile);
	   if (bbDriver.configureNw == 1)
	   {
	      bbDriver.initFlowProvisioner (args);
	   }

	   // RV: Initialize the L2 Path manager
	   // Always as loopback for now
	   L2PathManager.init(true);
	}
	catch (Exception ce)
	    {
		throw new AdmissionControlException (ce.toString());
	    }
    }

    public AdmissionControlResult flowRequest (FlowInfo theFlowInfo, 
					      QOSRequired qos, 
					      SchedulingAction schAction, 
					      StringHolder flowID,
                                              IntHolder dscp) 
	throws AdmissionControlException
    {
       return flowRequest_ft (theFlowInfo, qos, schAction, flowID, dscp, "");
    }

    public AdmissionControlResult flowRequest_ft (FlowInfo theFlowInfo, 
					      QOSRequired qos, 
					      SchedulingAction schAction, 
					      StringHolder flowID,
                                              IntHolder dscp, 
					      String reference)
	throws AdmissionControlException

    {
       //default priority = 1
       return flowRequest_ext (theFlowInfo, qos, schAction, flowID, dscp, 1, reference);
    }

    public AdmissionControlResult flowRequest_ext (FlowInfo theFlowInfo, 
					      QOSRequired qos, 
					      SchedulingAction schAction, 
					      StringHolder flowID,
                                              IntHolder dscp, 
					      int priority,
					      String reference)
	throws AdmissionControlException
    {

        System.out.println ("Received a flow Request with reference " + reference);
        flowID.value = "not alloted";
	dscp.value = 0;
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	AdmissionControlResult result;

	String srcIP = null;
	String srcMask = null;
	String destIP = null;
	String destMask = null;
	String srcPort = null;
	String destPort = null;
	String srcPortHigh = null;
	String destPortHigh = null;
	String proto = null;
	int fwdRate = 0;
	int fwdRateExtra = 0;
	int revRate = 0;
	int revRateExtra = 0;
	int flowDuration = 0;
	String svcClass = null;

	// flow ID is unique, returned from BBDRiver,
	String fID = new String ("0");	
	flowID.value = fID;
			    			   
	
	try
	    {
	      svcClass = getServiceClass (qos);

	      if (theFlowInfo.biDirectional)
		BBServer.bblogger.info ("Bi-directional flow"
				    +"request ..");
	      else
		BBServer.bblogger.info ("Uni-directional flow"
				    +"request ..");
	      
	      FlowInfo fi = theFlowInfo;
	      srcIP = fi.srcIP.dottedDecimal;
	      srcMask = fi.srcIP.subnetMask;
	      destIP = fi.destIP.dottedDecimal;
	      destMask = fi.destIP.subnetMask;
	      srcPort = new Integer (fi.srcPort.low).toString();
	      srcPortHigh = new Integer (fi.srcPort.high).toString();
	      destPort = new Integer (fi.destPort.low).toString();
	      destPortHigh = new Integer (fi.destPort.high).toString();
	      fwdRate = fi.fwdRate.requiredBW;
	      fwdRateExtra = fi.fwdRate.extraBW;
	      if (!fi.biDirectional)
	      {
		  revRate = fi.revRate.requiredBW;
		  revRateExtra = fi.revRate.extraBW;
	      }
	      proto = getProtocol(fi.protocol);
	      flowDuration = fi.flowDuration;
	      
	      BBServer.bblogger.info ("srcIP="+srcIP+","+srcMask);
	      BBServer.bblogger.info ("destIP="+destIP+","+destMask);
	      BBServer.bblogger.info ("srcPort="+srcPort+"-"+srcPortHigh);
	      BBServer.bblogger.info (" destPort="+destPort+"-"+destPortHigh);
	      BBServer.bblogger.info (" fwdRate="+ new Integer (fwdRate).toString()+
				  "+"+ new Integer (fwdRateExtra).toString());
	      BBServer.bblogger.info (" revRate="+ new Integer (revRate).toString()+
				  "+"+ new Integer (revRateExtra).toString());
	      BBServer.bblogger.info ("protocol="+proto);
	      BBServer.bblogger.info (" flowDuration="+new

		Integer(flowDuration).toString());
	      
              // Allow for no mask information
              if (srcMask.equals (""))
                 srcMask = "255.255.255.255";
              if (destMask.equals (""))
                 destMask = "255.255.255.255";

	      //@@todo: check for scheduling action default,
		// all assumed to be reserve_and_commit
		// - Kirthika
		String retval="";
		switch (schAction.value())
		    {
		    case SchedulingAction._reserve:
			{
			    BBServer.bblogger.info ("Flow request to BB:"+
						" flowID="+fID+
						" srcIP="+srcIP+
						" destIP="+destIP+
						" srcPort="+srcPort+
						" destPort="+destPort+
						" fwdRate="+new
						    Integer(fwdRate).toString()+
						" revRate="+new
						    Integer(revRate).toString()+
						" flowDuration="+new
						    Integer(flowDuration).toString()+
						" svcClass="+svcClass+
						" protocol="+proto);
			    
			    // flowID is returned, not all parameters are used as of now
			     BBDriver.FlowReference fref =
                                bbDriver.processFlowSetupRequest (srcIP, 
					srcMask,
					destIP,destMask,
					srcPort,srcPortHigh,
					destPort,destPortHigh,
					proto,
					svcClass,
					fwdRate,
					fwdRateExtra,
					false, 
					priority,
					reference);
			    

			    if (fref == null)
				result = AdmissionControlResult.DECISION_DENY;
			    else
				{
				    flowID.value = fref.flowID;
				    dscp.value = fref.dscp;
				    result = AdmissionControlResult.DECISION_ADMIT;
				}
			    break;
			}
		    default:
		    case SchedulingAction._reserve_and_commit:
			{
			    BBServer.bblogger.info ("Flow request to BB:"+
						" flowID="+fID+
						" srcIP="+srcIP+
						" destIP="+destIP+
						" srcPort="+srcPort+
						" destPort="+destPort+
						" fwdRate="+new
						    Integer(fwdRate).toString()+
						" revRate="+new
						    Integer(revRate).toString()+
						" flowDuration="+new
						    Integer(flowDuration).toString()+
						" svcClass="+svcClass+
						" protocol="+proto);
			    
			  
			    flowID.value = fID;
			    // flowID is returned, not all parameters are used as of now
			    BBDriver.FlowReference fRef = 
                                  bbDriver.processFlowSetupRequest (srcIP, 
				  	srcMask,
					destIP,destMask,
					srcPort,srcPortHigh,
					destPort,destPortHigh,
					proto,
					svcClass,
					fwdRate,
					fwdRateExtra,
					true, 
					priority,
					reference);
			    if (fRef == null)
				result = AdmissionControlResult.DECISION_DENY;
			    else
				{
				    flowID.value = fRef.flowID;
                                    dscp.value = fRef.dscp;
				    result = AdmissionControlResult.DECISION_ADMIT;
                                    System.out.println ("Flow Admitted successfully; flowID =" + fRef.flowID);
				}
			    break;
			}
		    }
		
	    }catch (Exception op)
		{
		     result = AdmissionControlResult.DECISION_ERROR;
		    throw new AdmissionControlException ("Exception "+
							 op.toString()+
		       " in processing flow request for flowID: "+flowID.value);
		   
		}

	BBServer.bblogger.info ("Admission control status at server:"+
			    new Integer (result.value()).toString());
	return result;
    }
	
     public AdmissionControlResult flowRequestCommit(String flowID)
	throws AdmissionControlException
    {
	BBServer.bblogger.info ("in AdmissionControlImpl::flowRequestCommit for flowID:"+flowID);
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	if (bbDriver.processFlowRequestCommit (flowID))
	  return AdmissionControlResult.DECISION_ADMIT;
	else
	    return AdmissionControlResult.NOFLOWID;
    }
	
     public AdmissionControlResult flowDelete(String flowID) 
	throws AdmissionControlException
    {
	BBServer.bblogger.info ("in AdmissionControlImpl::flowRequestDelete");

        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	if (bbDriver.processFlowDeleteRequest (flowID))
	  return  AdmissionControlResult.DECISION_DELETE;
        else
	    return  AdmissionControlResult.NOFLOWID;
    }
	
     public AdmissionControlResult unidirectionalFlowUpdate(String flowID, 
						    int oldfwdRate, 
						    int newfwdRate) 
	throws AdmissionControlException
    {
        try
        {
	BBServer.bblogger.info ("in AdmissionControlImpl::unidirectionalFlowUpdate");
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	if (bbDriver.processFlowUpdateRequest (flowID, newfwdRate))
	   return  AdmissionControlResult.DECISION_ADMIT;
        else
	   return  AdmissionControlResult.DECISION_DENY;
        }
        catch (Exception e)
        {
           e.printStackTrace ();
	   return  AdmissionControlResult.DECISION_DENY;
        }

    
    }
	
     public AdmissionControlResult bidirectionalFlowUpdate(String flowID, 
						   int oldfwdRate, 
						   int newfwdRate, 
						   int revRate, 
						   int newRevRate) 
	throws AdmissionControlException
    {
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	return  AdmissionControlResult.DECISION_ADMIT;
    }

     public AdmissionControlResult flowQosUpdate(String flowID, 
					 QOSRequired oldQos, 
					 QOSRequired newQos) 
	throws AdmissionControlException
    {
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	return  AdmissionControlResult.DECISION_ADMIT;
    }

    public AdmissionControlResult allocatedFlowBandwidthQuery (String flowToken, 
							       org.omg.CORBA.IntHolder fwdBW, 
							       org.omg.CORBA.IntHolder revBW) 
	throws mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlException
    {
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	return  AdmissionControlResult.DECISION_ADMIT;
    }


    boolean isQueryForProvisioned (BWQueryType type)
    {
       if (type.value () == BWQueryType._provisioned)
          return true;
       else
          return false;
    }

    public AdmissionControlResult subnetbandwidthQuery (BWBetnSubnetsSeqHolder 
							  subnetBandwidthInfo,
							BWQueryType type) 
	throws mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.AdmissionControlException
    {

        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

        try
        {
	   boolean qType = isQueryForProvisioned (type);

           BWBetnSubnets bbs [] = subnetBandwidthInfo.value;
           bbDriver.processBWQueries (bbs, qType);
           subnetBandwidthInfo.value = bbs;
        }
        catch (Exception e)
        {
           e.printStackTrace ();
	   return  AdmissionControlResult.BADDATA;

        }
	return  AdmissionControlResult.DECISION_ADMIT;
    }
    
    public AdmissionControlResult poolbandwidthQuery (BWBetnPoolsSeqHolder 
						      poolBandwidthInfo,
						      BWQueryType type) 
	throws AdmissionControlException
    {
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

        try
        {
	   boolean qType = isQueryForProvisioned (type);
           BWBetnPools bbs [] = poolBandwidthInfo.value;
           bbDriver.processBWQueriesByPools (bbs, qType);
           poolBandwidthInfo.value = bbs;
        }
        catch (Exception e)
        {
           e.printStackTrace ();
	   return  AdmissionControlResult.BADDATA;

        }
	return  AdmissionControlResult.DECISION_ADMIT;
    }
    
    public AdmissionControlResult allocateBandwidthPercentPerQoS(BWPercentPerQoS[] 
								   bwPercentageSeq) 
	throws AdmissionControlException
    {
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	return  AdmissionControlResult.DECISION_ADMIT;
    }

    public AdmissionControlResult changeBandwidthAllocationPolicyTo(BandwidthPolicy bwPolicy) 
	throws AdmissionControlException
    {
	BBServer.bblogger.info ("in AdmissionControlImpl::changeBandwidthAllocationPolicyTo:"+
				new Integer (bwPolicy.value ()).toString ());
        if (standByMode == true)
           return AdmissionControlResult.DECISION_DENY;

	if (bbDriver.changeBandwidthAllocationPolicyRequest (bwPolicy.value()))
	    return  AdmissionControlResult.DECISION_ADMIT;
	else
	    return  AdmissionControlResult.DECISION_ERROR;
	
    }

    public AdmissionControlResult standby () 
                                     throws AdmissionControlException
    {
       standByMode = true;
       try
       {
          bbDriver.processReleaseAllFlows ();
          return AdmissionControlResult.DECISION_ADMIT;
       }
       catch (Exception e)
       {
          e.printStackTrace ();
       }
       return  AdmissionControlResult.DECISION_ERROR;
    }

    public AdmissionControlResult resume () 
                                     throws AdmissionControlException
    {
       standByMode = false;
       return AdmissionControlResult.DECISION_ADMIT;
    }

    public String getProtocol (BBProtocolType p)
    {
	String protocol = new String ("");
	switch (p.value())
	    {
	    case BBProtocolType._tcp:
		protocol = new String ("tcp");
		break;
	    case BBProtocolType._udp:
		protocol = new String ("udp");
		break;
	    case BBProtocolType._sctp:
		protocol = new String ("sctp");
		break;
	    case BBProtocolType._notSpecified:
		protocol = new String ("notSpecified");
		break;
	    }
	return protocol;
    }
    
    public String getServiceClass (QOSRequired q)
    {
	String svcClass = new String ("bestEffort");
	switch (q.value())
	  {
	  case QOSRequired._highReliability :
	    {
	      // With higher WFQ size
	      svcClass = "highReliability";
	      break;
	    }
	  case QOSRequired._mmVoice :
	    {
	      // class for voice traffic
	      svcClass = "mmVoice";
	      break;
	    }
	  case QOSRequired._mmVideo :
	    {
	      // class for video traffic
	      svcClass = "mmVideo";
	      break;
	    }
	  case QOSRequired._highPriority :
	    {
	      // low latency, low priority queue size
	      svcClass = "highPriority";
	      break;
	    }
	  case QOSRequired._raw :
	    {
	      // raw physical bw class
	      svcClass = "raw";
	      break;
	    }
	  default:
	    svcClass = new String ("bestEffort");
	  }
	return svcClass;
    }

    public AdmissionControlResult processNetworkEvent (switchInfo f)
    {
       bbDriver.processNetworkEvent (f);
       return AdmissionControlResult.DECISION_ADMIT;
    }

    public AdmissionControlResult processL3NetworkEvent (String [] l3i)
    {
       bbDriver.processL3NetworkEvent (l3i);
       return AdmissionControlResult.DECISION_ADMIT;
    }
   
}


