// $Id$

#define ACE_BUILD_DLL

#include "ace/ACE_QoS.h"

ACE_RCSID( ace, "$Id$" )

ACE_ALLOC_HOOK_DEFINE( ACE_QoS )

class Ace_Export ACE_QoS
{
	// = TITLE
	//	Define the basic QoS parameters

public:
	// = Initializattion and termination methods.
	// Default
	ACE_QoS::ACE_QoS( void )
	{
		ACE_TRACE( "ACE_QoS::ACE_QoS" );
	}

	// set CBR rate
	int ACE_QoS::set_cbr_rate( int rate )
	{
		ACE_TRACE( "ACE_QoS::set_cbr_rate" );

#if defined (ACE_HAS_FORE_ATM_WS2)
		AAL_PARAMETERS_IE					ie_aalparams;
		ATM_TRAFFIC_DESCRIPTOR_IE			ie_td;
		ATM_BROADBAND_BEARER_CAPBILITY_IE	ie_bbc;
		ATM_QOS_CLASS_IE					ie_qos;
		Q2931_IE							*ie_ptr;
		int size;

		ie_aalparams.AALType = AALTYPE_5;
		ie_aalparams.AALSpecificParameters.AAL5Parameters.ForwardMaxCPCSSDUSize = 1516; 
		ie_aalparams.AALSpecificParameters.AAL5Parameters.BackwardMaxCPCSSDUSize = 1516; 
		ie_aalparams.AALSpecificParameters.AAL5Parameters.Mode = AAL5_MODE_MESSAGE;
		ie_aalparams.AALSpecificParameters.AAL5Parameters.SSCSType = AAL5_SSCS_NULL;

		size = sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(AAL_PARAMETERS_IE);

		ie_td.Forward.PeakCellRate_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Forward.PeakCellRate_CLP01 = cellRate;
    	ie_td.Forward.SustainableCellRate_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Forward.SustainableCellRate_CLP01 = SAP_FIELD_ABSENT;
		ie_td.Forward.MaxBurstSize_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Forward.MaxBurstSize_CLP01 = SAP_FIELD_ABSENT;
		ie_td.Forward.Tagging = SAP_FIELD_ABSENT;

		ie_td.Backward.PeakCellRate_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Backward.PeakCellRate_CLP01 = cellRate;
		ie_td.Backward.SustainableCellRate_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Backward.SustainableCellRate_CLP01 = SAP_FIELD_ABSENT;
		ie_td.Backward.MaxBurstSize_CLP0 = SAP_FIELD_ABSENT;
		ie_td.Backward.MaxBurstSize_CLP01 = SAP_FIELD_ABSENT;
		ie_td.Backward.Tagging = SAP_FIELD_ABSENT;

		ie_td.BestEffort = 0; // Note: this must be set to zero for CBR

		size += sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(ATM_TRAFFIC_DESCRIPTOR_IE);

		ie_bbc.BearerClass = BCOB_X;
		ie_bbc.TrafficType = TT_CBR;
		ie_bbc.TimingRequirements = TR_END_TO_END;
		ie_bbc.ClippingSusceptability = CLIP_NOT;
		ie_bbc.UserPlaneConnectionConfig = UP_P2P;

		size += sizeof(Q2931_IE_TYPE) + sizeof(ULONG) 
			+ sizeof(ATM_BROADBAND_BEARER_CAPABILITY_IE);

		ie_qos.QOSClassForward = QOS_CLASS1;
		ie_qos.QOSClassBackward = QOS_CLASS1; 

		size += sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(ATM_QOS_CLASS_IE);
	
		qos_.ProviderSpecific.buf = (char *) malloc(size);
		qos_.ProviderSpecific.len = size;
		memset(qos_.ProviderSpecific.buf, 0, size);

		ie_ptr = (Q2931_IE *) qos_.ProviderSpecific.buf;
		ie_ptr->IEType = IE_AALParameters;
		ie_ptr->IELength = sizeof(Q2931_IE_TYPE) + sizeof(ULONG) 
						+ sizeof(AAL_PARAMETERS_IE);
    	memcpy(ie_ptr->IE, &ie_aalparams, sizeof(AAL_PARAMETERS_IE));

		ie_ptr = (Q2931_IE *) ((char *)ie_ptr + ie_ptr->IELength);
		ie_ptr->IEType = IE_TrafficDescriptor;
		ie_ptr->IELength = sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(ATM_TRAFFIC_DESCRIPTOR_IE);
		memcpy(ie_ptr->IE, &ie_td, sizeof(ATM_TRAFFIC_DESCRIPTOR_IE));

		ie_ptr = (Q2931_IE *) ((char *)ie_ptr + ie_ptr->IELength);
		ie_ptr->IEType = IE_BroadbandBearerCapability;
		ie_ptr->IELength = sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(ATM_BROADBAND_BEARER_CAPABILITY_IE);
		memcpy(ie_ptr->IE, &ie_bbc, sizeof(ATM_BROADBAND_BEARER_CAPABILITY_IE));
	
		ie_ptr = (Q2931_IE *) ((char *)ie_ptr + ie_ptr->IELength);
		ie_ptr->IEType = IE_QOSClass;
		ie_ptr->IELength = sizeof(Q2931_IE_TYPE) + sizeof(ULONG) + sizeof(ATM_QOS_CLASS_IE);
		memcpy(ie_ptr->IE, &ie_qos, sizeof(ATM_QOS_CLASS_IE));

		qos_.SendingFlowspec.TokenRate = -1;
		qos_.SendingFlowspec.TokenBucketSize = -1;
		qos_.SendingFlowspec.PeakBandwidth = -1;
		qos_.SendingFlowspec.Latency = -1;
		qos_.SendingFlowspec.DelayVariation = -1;
		qos_.SendingFlowspec.LevelOfGuarantee = BestEffortService ; 
		// will most probably be ignored by the service provider
		qos_.SendingFlowspec.CostOfCall = 0;

		qos_.ReceivingFlowspec.TokenRate = -1;
		qos_.ReceivingFlowspec.TokenBucketSize = -1;
		qos_.ReceivingFlowspec.PeakBandwidth = -1;
		qos_.ReceivingFlowspec.Latency = -1;
		qos_.ReceivingFlowspec.DelayVariation = -1;
		qos_.ReceivingFlowspec.LevelOfGuarantee = BestEffortService; 
		// will most probably be ignored by the service provider
		qos_.ReceivingFlowspec.CostOfCall = 0;
#elif defined (ACE_HAS_FORE_ATM_XTI)
		// XTI part
#endif // ACE_HAS_FORE_ATM_WS2 && ACE_HAS_FORE_ATM_XTI
	}

	QOS ACE_QoS::get_qos( void )
	{
		return qos_;
	}
}
