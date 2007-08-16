// $Id$

#ifndef HTIOP_ACCEPTOR_IMPL_CPP
#define HTIOP_ACCEPTOR_IMPL_CPP

#include "orbsvcs/HTIOP/HTIOP_Acceptor_Impl.h"
#include "orbsvcs/HTIOP/HTIOP_Completion_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Session.h"

#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Connector_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#include "ace/Object_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

////////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO::HTIOP::Creation_Strategy<SVC_HANDLER>::Creation_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO::HTIOP::Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Make Svc Handler\n"));
      ACE_NEW_RETURN (sh,
                      SVC_HANDLER (this->orb_core_),
                      -1);
    }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::Accept_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (
                          const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                          int restart)
{
  if (this->peer_acceptor_.open (local_addr, restart) == -1)
    return -1;

  this->peer_acceptor_.enable (ACE_NONBLOCK);

  return 0;
}


/// Returns the underlying PEER_ACCEPTOR object
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_PEER_ACCEPTOR &
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

/// Returns the underlying PEER_ACCEPTOR object
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{
  return this->peer_acceptor_.get_handle ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  ACE_DEBUG ((LM_DEBUG, "accept_svc_handler\n"));
  if (this->peer_acceptor_.accept (svc_handler->peer (),
                                   0,
                                   0,
                                   1,
                                   0 /* reset_new_handle */) == -1)
    {
      svc_handler->close (0);

      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "Accept_svc_handler, skipping first_recv\n"));
  return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::
Strategy_Acceptor (const ACE_TCHAR service_name[],
                   const ACE_TCHAR service_description[],
                   int use_select,
                   int reuse_addr)

{
  ACE_UNUSED_ARG (service_name);
  ACE_UNUSED_ARG (service_description);
  ACE_UNUSED_ARG (use_select);
  ACE_UNUSED_ARG (reuse_addr);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::shared_open (
    const ACE_PEER_ACCEPTOR_ADDR &local_addr,
    int protocol_family,
    int backlog)
{
  int error = 0;
#if defined (ACE_HAS_IPV6)
  ACE_ASSERT (protocol_family == PF_INET || protocol_family == PF_INET6);

  if (protocol_family == PF_INET6)
    {
      sockaddr_in6 local_inet6_addr;
      ACE_OS::memset (reinterpret_cast<void *> (&local_inet6_addr),
                      0,
                      sizeof local_inet6_addr);

      if (local_addr == ACE_Addr::sap_any)
        {
          local_inet6_addr.sin6_family = AF_INET6;
          local_inet6_addr.sin6_port = 0;
          local_inet6_addr.sin6_addr = in6addr_any;
        }
      else
        local_inet6_addr = *reinterpret_cast<sockaddr_in6 *> (local_addr.get_addr ());

      // We probably don't need a bind_port written here.
      // There are currently no supported OS's that define
      // ACE_LACKS_WILDCARD_BIND.
      if (ACE_OS::bind (this->get_handle (),
                        reinterpret_cast<sockaddr *> (&local_inet6_addr),
                        sizeof local_inet6_addr) == -1)
        error = 1;
    }
  else
#endif
    if (protocol_family == PF_INET)
      {
        sockaddr_in local_inet_addr;
        ACE_OS::memset (reinterpret_cast<void *> (&local_inet_addr),
                        0,
                        sizeof local_inet_addr);

        if (local_addr == ACE_Addr::sap_any)
          {
            local_inet_addr.sin_port = 0;
          }
        else
          local_inet_addr = *reinterpret_cast<sockaddr_in *> (local_addr.get_addr ());
        if (local_inet_addr.sin_port == 0)
          {
            if (ACE::bind_port (this->get_handle ()) == -1)
              error = 1;
          }
        else if (ACE_OS::bind (this->get_handle (),
                               reinterpret_cast<sockaddr *> (&local_inet_addr),
                               sizeof local_inet_addr) == -1)
          error = 1;
      }
    else if (ACE_OS::bind (this->get_handle (),
                           (sockaddr *) local_addr.get_addr (),
                           local_addr.get_size ()) == -1)
      error = 1;

  if (error != 0
      || ACE_OS::listen (this->get_handle (),
                         backlog) == -1)
    {
      error = 1;
      this->close ();
    }

  return error ? -1 : 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open(
   const ACE_PEER_ACCEPTOR_ADDR &local_addr,
   ACE_Reactor *reactor,
   TAO::HTIOP::Creation_Strategy<SVC_HANDLER> * cre_s,
   TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> *acc_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   int reuse_addr)
{
  this->reactor (reactor);

  this->accept_strategy_ = acc_s;
  this->concurrency_strategy_ = con_s;
  this->creation_strategy_ = cre_s;

  if (this->accept_strategy_->open (local_addr, reuse_addr) == -1)
    return -1;

  if (this->accept_strategy_->acceptor ().enable (ACE_NONBLOCK) != 0)
    return -1;

  int result =
    reactor->register_handler (this,
                               ACE_Event_Handler::ACCEPT_MASK);


  return result;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::close ()
{
  ACE_TRACE ("TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::close");

  // Guard against multiple closes.
  if (this->reactor () != 0)
    {
      ACE_HANDLE handle = this->get_handle ();

      this->reactor ()->remove_handler
        (handle,
         // We must pass the DONT_CALL flag here to avoid infinite
         // recursion.
         ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);

      // Shut down the listen socket to recycle the handles.
      if (this->accept_strategy_->acceptor ().close () == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("close\n")));
      // Set the Reactor to 0 so that we don't try to close down
      // again.
      this->reactor (0);
    }
  return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle () const
{
  return this->accept_strategy_->get_handle ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_input (ACE_HANDLE listener)
{
  ACE_Handle_Set conn_handle;

  ACE_Time_Value timeout;

  #  if defined (ACE_WIN64)
  // This arg is ignored on Windows and causes pointer truncation
  // warnings on 64-bit compiles.
  int select_width = 0;
#  else
  int select_width = int (listener) + 1;
#  endif /* ACE_WIN64 */

  // TAO_HTIOP connections cannot be attached to sessions or anything else until
  // after the HTTP request header is read. So the acceptor here just creates
  // completion handler that gets the header data and then creates the
  // association between the connection handler and the session, or
  // re-establishes an existing session.
  do
    {
      TAO::HTIOP::Completion_Handler *sh = 0;
      if (this->make_svc_handler(sh) == -1 ||
          this->accept_svc_handler (sh) == -1 ||
          this->activate_svc_handler (sh) == -1)
        return -1;
    }

  // Now, check to see if there is another connection pending and
  // break out of the loop if there is none.

  //int use_select = 1;
  while (1
         && ACE_OS::select (select_width,
                            conn_handle,
                            0,
                            0,
                            &timeout) == 1);
  return 0;
}


template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler (SVC_HANDLER *svc_handler)
{
  return this->concurrency_strategy_->activate_svc_handler (svc_handler,
                                                            (void *) this);
}


template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  return this->accept_strategy_->accept_svc_handler (svc_handler);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::make_svc_handler (SVC_HANDLER *&sh)
{
  return this->creation_strategy_->make_svc_handler (sh);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* HTIOP_ACCEPTOR_IMPL_CPP */
