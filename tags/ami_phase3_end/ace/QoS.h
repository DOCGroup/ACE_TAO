// $Id$

// QoS class definition (refer to WinSock2 QoS)

#ifndef QOS_H
#define QOS_H

#include 

#if !defined(ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Ace_Export ACE_QoS
{
	// = TITLE
	//	Define the basic QoS parameters

public:
	// = Initializattion and termination methods.
	ACE_QoS( void );
	// Default

	~ACE_QoS( void );
	// Default dtor

	// set CBR rate
	int set_cbr_rate( int );

	// get QoS struct
	QOS get_qos( void );

private:
#if defined (ACE_HAS_FORE_ATM_WS2)
	// just map to WS2 GQOS struct
	QOS	qos_;
#elif defined (ACE_HAS_FOR_ATM_XTI)
typedef enum {
	BEST_EFFORT,
	CONTROLLED_LOAD,
	GUARANTEED
} SERVICETYPE;

typedef struct _flowspec {
	int	TokeRate;
	int	TokenBucketSize;
	int	PeakBandwidth;
	int	Latency;
	int	DelayVariation;
	SERVICETYPE	ServiceType;
	int	MaxSduSize;
	int	MinimumPolicedSize;
} FLOWSPEC;

typedef struct _BUF {
	ulong	len;
	char	*buf;
} BUF;

typedef struct _QualityOfService {
	FLOWSPEC	SendingFlowspec;
	FLOWSPEC	ReceivingFlowspec;
	BUF			ProviderSpecific;
} QOS;

	QOS	qos_;
#endif // ACE_HAS_FORE_ATM_WS2 && ACE_HAS_FOREATM_XTI
}

#endif /* QOS_H */
