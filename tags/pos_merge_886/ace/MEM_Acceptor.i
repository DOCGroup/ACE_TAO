/* -*- C++ -*- */
// $Id$

// MEM_Acceptor.i

ASYS_INLINE int
ACE_MEM_Acceptor::open (const ACE_Addr &local_sap,
                        ACE_Protocol_Info *protocolinfo,
                        ACE_SOCK_GROUP g,
                        u_long flags,
                        int reuse_addr,
                        int protocol_family,
                        int backlog,
                        int protocol)
{
  return this->ACE_SOCK_Acceptor::open
    (local_sap, protocolinfo, g, flags, reuse_addr, protocol_family,
     backlog, protocol);
}

ASYS_INLINE int
ACE_MEM_Acceptor::accept (ACE_SOCK_Stream &new_stream,
                          ACE_Addr *remote_addr,
                          ACE_Time_Value *timeout,
                          int restart,
                          int reset_new_handle) const
{
  return this->ACE_SOCK_Acceptor::accept
    (new_stream, remote_addr, timeout, restart, reset_new_handle);
}

ASYS_INLINE int
ACE_MEM_Acceptor::accept (ACE_SOCK_Stream &new_stream,
                          ACE_Accept_QoS_Params qos_params,
                          ACE_Addr *remote_addr,
                          ACE_Time_Value *timeout,
                          int restart,
                          int reset_new_handle) const
{
  return this->ACE_SOCK_Acceptor::accept
    (new_stream, qos_params, remote_addr, timeout, restart, reset_new_handle);
}

ASYS_INLINE int
ACE_MEM_Acceptor::get_local_addr (ACE_MEM_Addr &sap) const
{
  ACE_INET_Addr temp;

  this->ACE_SOCK_Acceptor::get_local_addr (temp);
  sap.set_port_number (temp.get_port_number ());
  return 0;
}

ASYS_INLINE const ACE_TCHAR *
ACE_MEM_Acceptor::mmap_prefix (void) const
{
  return this->mmap_prefix_;
}

ASYS_INLINE void
ACE_MEM_Acceptor::mmap_prefix (const ACE_TCHAR *prefix)
{
  if (prefix == 0)
    {
      this->mmap_prefix_ = 0;
    }
  else
    {
      this->mmap_prefix_ = ACE::strnew (prefix);
    }
}

ASYS_INLINE ACE_MEM_IO::Signal_Strategy
ACE_MEM_Acceptor::preferred_strategy (void) const
{
  return this->preferred_strategy_;
}

ASYS_INLINE void
ACE_MEM_Acceptor::preferred_strategy (ACE_MEM_IO::Signal_Strategy strategy)
{
  this->preferred_strategy_ = strategy;
}

ASYS_INLINE ACE_MEM_SAP::MALLOC_OPTIONS &
ACE_MEM_Acceptor::malloc_options (void)
{
  return this->malloc_options_;
}
