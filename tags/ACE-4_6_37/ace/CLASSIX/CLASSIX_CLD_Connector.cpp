// $Id$
/*
// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_CLD_Connector.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
//
// ============================================================================
*/
#define ACE_BUILD_DLL
#include "ace/CLASSIX/CLASSIX_CLD_Connector.h"
#include "ace/CLASSIX/CLASSIX_Addr.h"

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_CLD_Connector)

#if !defined (__ACE_INLINE__)
#include "CLASSIX_CLD_Connector.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */

ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector (
    ACE_CLASSIX_Stream &new_stream, 
    const ACE_Addr &remote_sap, 
    ACE_Time_Value *timeout,
    const ACE_Addr &local_sap,
    int reuse_addr,
    int flags,
    int perms,
    int protocol_family,
    int protocol)
{
  ACE_TRACE ("ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector");
  if (this->connect (new_stream, remote_sap, timeout, local_sap,
		     reuse_addr, flags, perms, 
		     protocol_family, protocol) == -1)
      ACE_ERROR ((LM_ERROR, 
		  ASYS_TEXT ("%p\n"),  ASYS_TEXT 
		  ("ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector")));
}

// Set up remote port information
int
ACE_CLASSIX_CLD_Connector::connect (ACE_CLASSIX_Stream &new_stream, 
				    const ACE_Addr & theRemote,/* remote_sap */
				    ACE_Time_Value *, /* timeout */
				    const ACE_Addr & theLocal,
                                    int, /* reuse_addr */
				    int theFlag, /* flags */
				    int, /* perms */
				    int theProtocolFamily, 
				    int /* protocol */)

{
    ACE_TRACE ("ACE_CLASSIX_CLD_Connector::connect");
    //
    // Set up peer SAP
    // 
    if (theRemote == ACE_Addr::sap_any ||
	new_stream.set_peer_sap(theRemote) != 0)
	ACE_ERROR_RETURN((LM_ERROR, "ACE_CLASSIX_CLD_Connector::connect()::"
			  "Invalid Connector SAP\n"), -1);

    //
    // Set up local SAP
    // 
    if (theLocal == ACE_Addr::sap_any)
    {
	// if local sap has not been configured
	// create a port and use it as the local SAP 
	new_stream.open();
    }
    else
    {
	// replace the local SAP information
	new_stream.close();
	new_stream.open(theLocal);
    }
    
    //
    // Make the local SAP "selectable", if requested via the flag
    // 
    if (new_stream.local_sap().is_configured() && 
	new_stream.peer_sap().is_configured() &&  /* remote sap info exist */
	theProtocolFamily == ACE_CLASSIX_Addr::AF_CLASSIX)
    {
	new_stream.control();
	// setup the default delivery mode, if applicable
	if (theFlag & ACE_CLASSIX_CLD_Connector::ENABLE)
	{
	    if (new_stream.selectable() == 0)
		return 0;
	    else
		return -1;
	}
	return 0;
    }
    else
    {
	return -1;
    }
}

void
ACE_CLASSIX_CLD_Connector::dump (void) const
{
  ACE_TRACE ("ACE_CLASSIX_CLD_Connector::dump");
}

/* ------------------------------------------------------------------------- */
