// $Id$

#define ACE_BUILD_DLL
#include "ace/ATM_Acceptor.h"

ACE_RCSID(ace, ATM_Acceptor, "$Id$")

#if defined (ACE_HAS_ATM)

#if !defined (__ACE_INLINE__)
#include "ace/ATM_Acceptor.i"
#endif /* __ACE_INLINE__ */

// Put the actual definitions of the ACE_ATM_Request and
// ACE_ATM_Request_Queue classes here to hide them from clients...

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Acceptor)

void
ACE_ATM_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Acceptor::dump");
}

ACE_ATM_Acceptor::ACE_ATM_Acceptor (void)
{
  ACE_TRACE ("ACE_ATM_Acceptor::ACE_ATM_Acceptor");
}

ACE_ATM_Acceptor::~ACE_ATM_Acceptor (void)
{
  ACE_TRACE ("ACE_ATM_Acceptor::~ACE_ATM_Acceptor");
}

int 
ACE_ATM_Acceptor::get_local_addr( ACE_ATM_Addr &local_addr )
{
  ACE_TRACE( "ACE_ATM_Acceptor::get_local_addr" );

#if defined (ACE_HAS_FORE_ATM_WS2)
  unsigned long ret = 0;
  DWORD deviceID = 0;
  ATM_ADDRESS addr;
  struct sockaddr_atm *laddr;

  if ( ::WSAIoctl(( int )(( ACE_SOCK_Acceptor *)this ) -> get_handle(), 
                SIO_GET_ATM_ADDRESS, 
                (LPVOID) &deviceID, 
                sizeof(DWORD), 
                ( LPVOID )&addr,
                sizeof( ATM_ADDRESS ),
                &ret,
                NULL,
                NULL ) == SOCKET_ERROR ) {
    ACE_OS::printf( "ATM_Acceptor(get_local_addr): WSIoctl: %d\n", 
                    ::WSAGetLastError());
    return -1;
  }

  laddr = ( struct sockaddr_atm *)local_addr.get_addr();
  ACE_OS::memcpy(( void *)&( laddr -> satm_number ), 
                 ( void *)&addr, 
                 ATM_ADDR_SIZE - 1 );

  return 0;
#elif defined (ACE_HAS_FORE_ATM_XTI)
  ACE_UNUSED_ARG( local_addr );
  
  return 0;
#else
  ACE_UNUSED_ARG( local_addr );

  return 0;
#endif /* ACE_HAS_FORE_ATM_WS2 && ACE_HAS_FORE_ATM_XTI */
}

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
  //Unable to use ACE_SOCK_Acceptor.open 
  //because of its assumption of using SOCK_STREAM
  //return (ACE_HANDLE)(acceptor_.open (remote_sap,       //ACE_Addr
  //                        protocol_info,                //ACE_Protocol_Info
  //                        0,                            //ACE_SOCK_Group
  //                        0,                            //u_long flags
  //                        params.get_reuse_addr(),      
  //                        params.get_protocol_family(),
  //                        backlog,
  //                        params.get_protocol()));

  struct sockaddr_atm local_atm_addr;
  ACE_HANDLE          ret;
  DWORD               flags = 0;

  /* Create a local endpoint of communication */

  // Only leaves can listen.
  flags = ACE_FLAG_MULTIPOINT_C_LEAF | ACE_FLAG_MULTIPOINT_D_LEAF;
          

  if ((ret = ACE_OS::socket (AF_ATM, 
                             SOCK_RAW, 
                             ATMPROTO_AAL5, 
                             NULL, 
                             0, 
                             flags )) 
    == ACE_INVALID_HANDLE) {
    ACE_OS::printf( "Acceptor(open): socket %d\n", ::WSAGetLastError()), 
    ACE_OS::exit (1);
  }

  ((ACE_SOCK_Acceptor *)this) -> set_handle( ret );

  /* Set up the address information to become a server */
  ACE_OS::memset ((void *) &local_atm_addr, 0, sizeof local_atm_addr);
  local_atm_addr.satm_family = AF_ATM;
  local_atm_addr.satm_number.AddressType = SAP_FIELD_ANY_AESA_REST;
  local_atm_addr.satm_number.Addr[ ATM_ADDR_SIZE - 1 ] 
    = (( ACE_ATM_Addr *)&remote_sap ) -> get_selector();
  local_atm_addr.satm_blli.Layer2Protocol = SAP_FIELD_ANY;
  local_atm_addr.satm_blli.Layer3Protocol = SAP_FIELD_ABSENT;
  local_atm_addr.satm_bhli.HighLayerInfoType = SAP_FIELD_ABSENT;

  /* Associate address with endpoint */
  if (ACE_OS::bind ((( ACE_SOCK_Acceptor *)this ) -> get_handle(),
                    ACE_reinterpret_cast(struct sockaddr *, &local_atm_addr),
                    sizeof local_atm_addr) == -1) {
    ACE_OS::printf( "Acceptor(open): bind %d\n", ::WSAGetLastError()); 
    ACE_OS::exit (1);
  }

  /* Make endpoint listen for service requests */
  if (ACE_OS::listen ((( ACE_SOCK_Acceptor *)this ) -> get_handle(), 
                      backlog) 
                    == -1) {
    ACE_OS::printf( "Acceptor(open): listen %d\n", ::WSAGetLastError());
    ACE_OS::exit (1);
  }

  return 0;
#else
  ACE_UNUSED_ARG (remote_sap);
  ACE_UNUSED_ARG (backlog);
  ACE_UNUSED_ARG (params);
#endif /* ACE_HAS_FORE_ATM_XTI/ACE_HAS_FORE_ATM_WS2 */
}

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
//  return (acceptor_.accept(new_sap.get_stream(),
//                           remote_addr,
//                           timeout,
//                           restart,
//                           reset_new_handle));
  ACE_HANDLE n_handle;
  ACE_HANDLE s_handle = (( ACE_SOCK_Acceptor *) this ) -> get_handle();
  struct sockaddr_atm *cli_addr 
    = ( struct sockaddr_atm *)remote_addr -> get_addr();
  int caddr_len = sizeof( struct sockaddr_atm ); 

  do {
    n_handle = ACE_OS::accept( s_handle, 
                              ACE_reinterpret_cast( struct sockaddr *, 
                                                    cli_addr ),
                              &caddr_len );
  } while ( n_handle == ACE_INVALID_HANDLE && errno == EINTR );

  (( ACE_ATM_Addr *)remote_addr ) -> set( cli_addr, 
                        (( ACE_ATM_Addr *)remote_addr ) -> get_selector());
  (( ACE_IPC_SAP *)&new_sap ) -> set_handle( n_handle );

  return 0;
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

#endif /* ACE_HAS_ATM */

