/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector (ACE_CLASSIX_Stream &new_stream, 
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

// Do-nothing constructor...

ACE_INLINE
ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector (void)
{
  ACE_TRACE ("ACE_CLASSIX_CLD_Connector::ACE_CLASSIX_CLD_Connector");
}

ACE_INLINE
int 
ACE_CLASSIX_CLD_Connector::reset_new_handle (ACE_HANDLE /*  handle */)
{
  return 0;
}
