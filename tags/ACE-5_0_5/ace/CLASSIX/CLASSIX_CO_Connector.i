/* -*- C++ -*- */
// $Id$
//
// CLASSIX_CO_Connector.i
/* ------------------------------------------------------------------------- */

ACE_INLINE
ACE_CLASSIX_CO_Connector::ACE_CLASSIX_CO_Connector (
    ACE_CLASSIX_Stream &new_stream, 
    const ACE_Addr &remote_sap, 
    ACE_Time_Value *timeout,
    const ACE_Addr &local_sap,
    int reuse_addr,
    int flags,
    int perms,
    int protocol_family,
    int protocol)
    : ACE_CLASSIX_CLD_Connector(new_stream, remote_sap, timeout, local_sap,
				reuse_addr, flags, perms, 
				protocol_family, protocol)
{
  ACE_TRACE ("ACE_CLASSIX_CO_Connector::ACE_CLASSIX_CO_Connector");
}

// Do-nothing constructor...

ACE_INLINE
ACE_CLASSIX_CO_Connector::ACE_CLASSIX_CO_Connector (void)
    : ACE_CLASSIX_CLD_Connector()
{
  ACE_TRACE ("ACE_CLASSIX_CO_Connector::ACE_CLASSIX_CO_Connector");
}

/* ------------------------------------------------------------------------- */


