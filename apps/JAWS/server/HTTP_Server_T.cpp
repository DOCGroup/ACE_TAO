// $Id$

#if !defined (HTTP_SERVER_T_I)
#define HTTP_SERVER_T_I

template <class ACE_LOCK> int
LOCK_SOCK_Acceptor<ACE_LOCK>::accept (ACE_SOCK_Stream &ns,
				      ACE_Addr *ra,
				      ACE_Time_Value *to,
				      int r) const
{
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);

  return ACE_SOCK_Acceptor::accept (ns, ra, to, r);
}

#endif /* HTTP_SERVER_T_I */
