/* -*- C++ -*- */
// $Id$

// TLI_Connector.i

inline
ACE_TLI_Connector::ACE_TLI_Connector (ACE_TLI_Stream &new_stream, 
				      const ACE_Addr &remote_sap, 
				      ACE_Time_Value *timeout,
				      const ACE_Addr &local_sap, 
				      int reuse_addr, 
				      int flags,
				      int perms,
				      const char device[], 
				      struct t_info *info,
				      int rwf,
				      netbuf *udata,
				      netbuf *opt)
{
  ACE_TRACE ("ACE_TLI_Connector::ACE_TLI_Connector");
  if (this->connect (new_stream, remote_sap, timeout, local_sap, reuse_addr, 
		     flags, perms, device, 
		     info, rwf,
		     udata, opt) == ACE_INVALID_HANDLE
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIMEDOUT))
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_TLI_Stream::ACE_TLI_Stream"));
}
