/* -*- C++ -*- */
// $Id$

// TLI_Connector.i

ACE_INLINE
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
  if (this->connect (new_stream,
                     remote_sap,
                     timeout,
                     local_sap,
                     reuse_addr, 
                     flags,
                     perms,
                     device, 
                     info,
                     rwf,
                     udata,
                     opt) == -1
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_TLI_Stream::ACE_TLI_Stream")));
}

ACE_INLINE
int
ACE_TLI_Connector::reset_new_handle (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);
  // Nothing to do here since the handle is not a socket
  return 0;
}

