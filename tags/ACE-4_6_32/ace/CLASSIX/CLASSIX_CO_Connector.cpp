// $Id$
/*
// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_CO_Connector.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================
*/
#define ACE_BUILD_DLL
#include "ace/CLASSIX/CLASSIX_CO_Connector.h"
#include "ace/CLASSIX/CLASSIX_CO_Acceptor.h"

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_CO_Connector)

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_CO_Connector.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */

// Set up remote port information
    
int
ACE_CLASSIX_CO_Connector::connect (ACE_CLASSIX_Stream &new_stream, 
				   const ACE_Addr & theAcceptor,/* acceptor */
				   ACE_Time_Value * theTimeout, /* timeout */
				   const ACE_Addr & theLocal,
				   int theReuse,
				   int theFlag,
				   int thePerm,
				   int theProtocolFamily, 
				   int theProtocol)

{
    //
    // set up local SAP and connector info in <new_stream>
    // 
    if (this->ACE_CLASSIX_CLD_Connector::
	connect(new_stream,
		theAcceptor,
		theTimeout,
		theLocal,
		theReuse,
		theFlag & (~ACE_CLASSIX_CLD_Connector::ENABLE),
		thePerm,
		theProtocolFamily,
		theProtocol) != 0)
	return -1;

    //
    // make a connection
    // 
    // Tell the connector my own SAP
    ACE_CLASSIX_CO_Acceptor::Peer_Union peer;
    size_t size = sizeof (ACE_CLASSIX_CO_Acceptor::Peer_Union);
    peer.type = ACE_CLASSIX_Addr::PORT;
    peer.choose.port.peer = new_stream.local_sap().get_addr().get_id();

    if (new_stream.send((void*) &peer, size) != size)
	ACE_ERROR_RETURN((LM_ERROR, "%t|%P|%p\n",
			  "ACE_CLASSIX_Connector::connect()::send:"), -1);
    // Wait for peer SAP information from the acceptor
    if (new_stream.ipcRecv((void*) &peer, size, 0, theTimeout) != size)
	ACE_ERROR_RETURN((LM_ERROR, "ACE_CLASSIX_Connector::connect()::"
			  "unable to receive connect response\n"), -1);
	
    int error = 0;
    switch (peer.type)
    {
    case ACE_CLASSIX_Addr::PORT:
	error = new_stream.set_peer_sap(peer.choose.port.peer);
	break;

    case ACE_CLASSIX_Addr::GROUP:
	error = new_stream.set_peer_sap(peer.choose.group.peer);
	break;

    case ACE_CLASSIX_Addr::STAMP:
	error = new_stream.set_peer_sap(peer.choose.stamp.peer);
	break;

    case ACE_CLASSIX_Addr::DYNAMIC:
	error = new_stream.set_peer_sap(peer.choose.group.peer);
	break;

    default:
	ACE_ERROR_RETURN((LM_ERROR, "ACE_CLASSIX_Connect::connect()::"
			  "Invalid peer address\n"), -1);
    }

    if (error != 0)
	ACE_ERROR_RETURN((LM_ERROR, "ACE_CLASSIX_Connector::connect()::"
			  "unable to receive connect response\n"), -1);


    //
    // Make the local SAP "selectable"
    // 
    if (theFlag & ACE_CLASSIX_CO_Connector::ENABLE &&
	new_stream.selectable() != 0)
	return -1;

    return 0;
}

void
ACE_CLASSIX_CO_Connector::dump (void) const
{
    ACE_TRACE ("ACE_CLASSIX_CO_Connector::dump");
}

/* ------------------------------------------------------------------------- */
