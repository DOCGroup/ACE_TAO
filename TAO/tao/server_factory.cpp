// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     server_factory.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if !defined(__ACE_INLINE__)
#  include "server_factory.i"
#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Creation_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Creation_Strategy<TAO_OA_Connection_Handler>
#pragma instantiate ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
