ACE_INLINE TAO_OA_Parameters *
TAO_OA_Connection_Handler::params (void)
{
  return params_;
}

ACE_INLINE void
TAO_OA_Connection_Handler::params (TAO_OA_Parameters *p)
{
  params_ = p;
}

ACE_INLINE
TAO_Client_Connection_Handler::TAO_Client_Connection_Handler (ACE_Thread_Manager *)
  : in_use_ (0)
{}

ACE_INLINE CORBA::Boolean
TAO_Client_Connection_Handler::in_use (void)
{
  return in_use_;
}

ACE_INLINE void
TAO_Client_Connection_Handler::in_use (CORBA::Boolean flag)
{
  in_use_ = flag;
}

// @@ Chris, shouldn't we protect this stuff with some type of #ifdef
// for platforms that lack template specialization?  
// Template specializations that allow the cached connection manager
// to work better.

ACE_INLINE size_t
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}

ACE_INLINE int
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::compare_i (const ACE_INET_Addr &a1, 
                                                                        const ACE_INET_Addr &a2) const
{
  return a1 != a2;
}

