/* -*- C++ -*- */
// $Id$

// ATM_Connector.i

ACE_INLINE
ACE_ATM_Connector::ACE_ATM_Connector (ACE_ATM_Stream &new_stream, 
																      const ACE_ATM_Addr &remote_sap, 
                                      ACE_ATM_Params params,
                                      ACE_ATM_QoS options,
																      ACE_Time_Value *timeout,
																			const ACE_ATM_Addr &local_sap, 
																			int reuse_addr, 
																			int flags,
																			int perms)
{
  ACE_TRACE ("ACE_ATM_Connector::ACE_ATM_Connector");
  if ((ACE_HANDLE)this->connect (new_stream,
                                 remote_sap,
                                 params,
                                 options,
                                 timeout,
                                 local_sap,
                                 reuse_addr, 
		                             flags,
                                 perms) == ACE_INVALID_HANDLE
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
               ASYS_TEXT ("ACE_ATM_Stream::ACE_ATM_Stream")));
}

// Connect the <new_stream> to the <remote_sap>, waiting up to
// <timeout> amount of time if necessary.

//ACE_INLINE
//int
//ACE_ATM_Connector::connect (ACE_ATM_Stream &new_stream, 
//												    const ACE_ATM_Addr &remote_sap, 
//                            ACE_ATM_Params params,
//                            ACE_ATM_QoS options,
//														ACE_Time_Value *timeout,
//														const ACE_ATM_Addr &local_sap, 
//														int reuse_addr,
//														int flags,
//														int perms)
//{
//  ACE_TRACE ("ACE_ATM_Connector::connect");
//#if defined (ACE_HAS_FORE_ATM_XTI)
//  return connector_.connect(new_stream.get_stream(),
//                            remote_sap,
//                            timeout,
//                            local_sap,
//                            reuse_addr,
//                            flags,
//                            perms,
//                            params.get_device(),
//                            params.get_info(),
//                            params.get_rw_flag(),
//                            params.get_user_data(),
//                            &options.get_qos());
//#elif defined (ACE_HAS_FORE_ATM_WS2)
//  ACE_OS::printf( "ATM_Connector(connect): to connect\n" );
//
//  return connector_.connect(new_stream.get_stream(),
//                            remote_sap,
//                            options.get_option_params(),
//                            timeout,
//                            local_sap,
//                            0,
//                            0,
//                            flags,
//                            reuse_addr,
//                            perms,
//                            params.get_protocol_family(),
//                            params.get_protocol());
//#else
//  ACE_UNUSED_ARG (new_stream);
//  ACE_UNUSED_ARG (remote_sap);
//  ACE_UNUSED_ARG (params);
//  ACE_UNUSED_ARG (options);
//  ACE_UNUSED_ARG (timeout);
//  ACE_UNUSED_ARG (local_sap);
//  ACE_UNUSED_ARG (reuse_addr);
//  ACE_UNUSED_ARG (flags);
//  ACE_UNUSED_ARG (perms);
//  return 0;
//#endif /* ACE_HAS_FORE_ATM_XTI/ACE_HAS_FORE_ATM_WS2 */
//}

// Try to complete a non-blocking connection.

ACE_INLINE
int
ACE_ATM_Connector::complete (ACE_ATM_Stream &new_stream, 
			     ACE_ATM_Addr *remote_sap,
			     ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_ATM_Connector::complete");
#if defined (ACE_HAS_ATM)
  return connector_.complete(new_stream.get_stream(),
                             remote_sap,
                             tv);
#else
  ACE_UNUSED_ARG(new_stream);
  ACE_UNUSED_ARG(remote_sap);
  ACE_UNUSED_ARG(tv);
  return 0;
#endif
}

ACE_INLINE
int
ACE_ATM_Connector::add_leaf (ACE_ATM_Stream &current_stream,
                             const ACE_Addr &remote_sap,
                             ACE_INT32 leaf_id,
                             ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_ATM_Connector::add_leaf");
#if defined (ACE_HAS_FORE_ATM_XTI)
  return connector_.add_leaf(current_stream.get_stream(),
                             remote_sap,
                             leaf_id,
                             timeout);
#elif defined (ACE_HAS_FORE_ATM_WS2)
  ACE_UNUSED_ARG(leaf_id);
  ACE_UNUSED_ARG(timeout);

  SOCKET ns;
  int len = sizeof( struct sockaddr_atm );
  struct sockaddr_atm *laddr;
  laddr = ( struct sockaddr_atm *)remote_sap.get_addr();

  ns = ::WSAJoinLeaf(( SOCKET )current_stream.get_handle(),
                     ( struct sockaddr FAR *)laddr,
                     //( struct sockaddr *)remote_sap.get_addr(),
                     len, 
                     NULL,  // caller data
                     NULL,  // callee data
                     NULL,  // SQos
                     NULL,  // GQos
                     JL_SENDER_ONLY );

  if ( ns == INVALID_SOCKET ) {
    ACE_OS::printf( "Connector(add_leaf): WSAJoinLeaf %d\n", 
                    ::WSAGetLastError());
    return -1;
  }

//  ACE_HANDLE nah;

//  nah = ACE_OS::join_leaf( current_stream.get_handle(),
//                           ( struct sockaddr *)remote_sap.get_addr(),
//                           NULL );
              
  return 0;
#else
  ACE_UNUSED_ARG(current_stream);
  ACE_UNUSED_ARG(remote_sap);
  ACE_UNUSED_ARG(leaf_id);
  ACE_UNUSED_ARG(timeout);
  return 0;
#endif
}

ACE_INLINE
int
ACE_ATM_Connector::reset_new_handle (ACE_HANDLE handle)
{
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  // Reset the event association 
  return ::WSAEventSelect ((SOCKET) handle,
                           NULL,
                           0);    
#else /* !defined ACE_HAS_WINSOCK2 */
  ACE_UNUSED_ARG (handle);
  return 0;
#endif /* ACE_WIN32 */
}

