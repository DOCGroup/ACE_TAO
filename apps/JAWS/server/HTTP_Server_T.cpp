#if !defined (HTTP_SERVER_T_I)
#define HTTP_SERVER_T_I

template <class LOCK> int
LOCK_SOCK_Acceptor<LOCK>::accept (ACE_SOCK_Stream &ns,
                                  ACE_Addr *ra,
                                  ACE_Time_Value *to,
                                  int r) const
{
  ACE_Guard<LOCK> m ((LOCK &)this->lock_);

  return ACE_SOCK_Acceptor::accept (ns, ra, to, r);
}

#endif /* HTTP_SERVER_T_I */
