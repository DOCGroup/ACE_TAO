#if !defined (HTTP_SERVER_T_C)
#define HTTP_SERVER_T_C

template <class LOCK> int
LOCK_SOCK_Acceptor<LOCK>::accept (ACE_SOCK_Stream &ns,
                                  ACE_Addr *ra,
                                  ACE_Time_Value *to,
                                  int r) const
{
  ACE_Guard<LOCK> m ((LOCK &)this->lock_);

  return ACE_SOCK_Acceptor::accept (ns, ra, to, r);
}

// This is necessary for gcc to work with templates
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class LOCK_SOCK_Acceptor<ACE_Thread_Mutex>;
template class ACE_Oneshot_Acceptor<HTTP_Handler, ONESHOT_SOCK_ACCEPTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* HTTP_SERVER_T_C */
