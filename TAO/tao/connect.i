ACE_INLINE TAO_OA_Parameters *
TAO_Server_Connection_Handler::params (void)
{
  return params_;
}

ACE_INLINE void
TAO_Server_Connection_Handler::params (TAO_OA_Parameters *p)
{
  params_ = p;
}

ACE_INLINE
TAO_Client_Connection_Handler::TAO_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SVC_HANDLER (t, 0, 0)
{
}

/*
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
*/

