/* -*- C++ -*- */
// $Id$

// ATM_Acceptor.i


ACE_INLINE
ACE_HANDLE
ACE_ATM_Acceptor::open (const ACE_Addr &remote_sap,
                        int backlog,
                        ACE_ATM_Params params)
{
  ACE_TRACE ("ACE_ATM_Acceptor::open");
#if defined (ACE_HAS_FORE_ATM_XTI)
  ACE_HANDLE handle = acceptor_.open (remote_sap,
                                      params.get_reuse_addr(),
                                      params.get_oflag(),
                                      params.get_info(),
                                      backlog,
                                      params.get_device());
  return (handle == ACE_INVALID_HANDLE ? -1 : 0);
#elif defined (ACE_HAS_FORE_ATM_WS2)
  return (acceptor_.open (remote_sap,
                          params.get_reuse_addr(),
                          params.get_protocol_family(),
                          backlog,
                          params.get_protocol()));
#else
  ACE_UNUSED_ARG (remote_sap);
  ACE_UNUSED_ARG (backlog);
  ACE_UNUSED_ARG (params);
#endif /* ACE_HAS_FORE_ATM_XTI/ACE_HAS_FORE_ATM_WS2 */
}

ACE_INLINE
ACE_ATM_Acceptor::ACE_ATM_Acceptor (const ACE_Addr &remote_sap,
                                    int backlog,
                                    ACE_ATM_Params params)
{
  ACE_TRACE ("ACE_ATM_Acceptor::ACE_ATM_Acceptor");
  open (remote_sap,
        backlog,
        params);
}

ACE_INLINE
int
ACE_ATM_Acceptor::close (void)
{
#if defined (ACE_HAS_FORE_ATM_XTI) || defined (ACE_HAS_FORE_ATM_WS2)
  return (acceptor_.close());
#else
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI || ACE_HAS_FORE_ATM_WS2 */
}

ACE_INLINE
int
ACE_ATM_Acceptor::accept (ACE_ATM_Stream &new_sap,
                          ACE_Addr *remote_addr,
                          ACE_Time_Value *timeout,
                          int restart,
                          int reset_new_handle,
                          ACE_ATM_Params params,
                          ACE_ATM_QoS qos)
{
  ACE_TRACE ("ACE_ATM_Acceptor::accept");
#if defined (ACE_HAS_FORE_ATM_XTI)
  ATM_QoS optbuf = qos.get_qos();
                                        
  return (acceptor_.accept(new_sap.get_stream(),
                           remote_addr,
                           timeout,
                           restart,
                           reset_new_handle,
                           params.get_rw_flag(),
                           params.get_user_data(),
                           &optbuf));
#elif defined (ACE_HAS_FORE_ATM_WS2)
  return (acceptor_.accept(new_sap.get_stream(),
                           remote_addr,
                           timeoutm
                           restart,
                           reset_new_handle));
#else
  ACE_UNUSED_ARG(new_sap);
  ACE_UNUSED_ARG(remote_addr);
  ACE_UNUSED_ARG(timeout);
  ACE_UNUSED_ARG(restart);
  ACE_UNUSED_ARG(reset_new_handle);
  ACE_UNUSED_ARG(params);
  ACE_UNUSED_ARG(qos);
  return (0);
#endif /* ACE_HAS_FORE_ATM_XTI */
}
