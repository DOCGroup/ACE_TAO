// $Id$

#if !defined (HTTP_SERVER_T_I)
#define HTTP_SERVER_T_I

template <class ACE_LOCK> int
LOCK_SOCK_Acceptor<ACE_LOCK>::accept (ACE_SOCK_Stream &stream,
				      ACE_Addr *remote_address,
				      ACE_Time_Value *timeout,
				      int restart,
                                      int reset_new_handle) const
{
  ACE_Guard<ACE_LOCK> m ((ACE_LOCK &) this->lock_);

  return ACE_SOCK_Acceptor::accept (stream, 
                                    remote_adrress, 
                                    timeout, 
                                    restart, 
                                    reset_new_handle);
}

#endif /* HTTP_SERVER_T_I */
