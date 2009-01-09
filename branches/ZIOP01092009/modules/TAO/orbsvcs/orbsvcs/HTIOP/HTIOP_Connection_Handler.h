// -*- C++ -*-

// ===================================================================
/**
 *  @file   HTIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
// ===================================================================

#ifndef HTIOP_CONNECTION_HANDLER_H
#define HTIOP_CONNECTION_HANDLER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Transport.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {

    /**
     * @class HTIOP_Connection_Handler
     *
     * @brief  Handles requests on a single connection.
     *
     * The Connection handler which is common for the Acceptor and
     * the Connector
     */


    class HTIOP_Export Connection_Handler : public SVC_HANDLER,
                                            public TAO_Connection_Handler
    {

    public:

      Connection_Handler (ACE_Thread_Manager* t = 0);

      /// Constructor. <arg> parameter is used by the Acceptor to pass the
      /// protocol configuration properties for this connection.
      Connection_Handler (TAO_ORB_Core *orb_core);

      /// Destructor.
      ~Connection_Handler (void);

      /// Called by the <Strategy_Acceptor> when the handler is completely
      /// connected.  Argument is unused.
      virtual int open (void *);

      virtual int open_handler (void *);

#if 0  // for now, an active version of HTIOP is not defined, but the
       // methods are kept here as a place-holder.

      /// = Active object activation method.
      virtual int activate (long flags = THR_NEW_LWP,
                            int n_threads = 1,
                            int force_active = 0,
                            long priority = ACE_DEFAULT_THREAD_PRIORITY,
                            int grp_id = -1,
                            ACE_Task_Base *task = 0,
                            ACE_hthread_t thread_handles[] = 0,
                            void *stack[] = 0,
                            size_t stack_size[] = 0,
                            ACE_thread_t  thread_names[] = 0);


      /// Only used when the handler is turned into an active object by
      /// calling <activate>.  This serves as the event loop in such cases.
      virtual int svc (void);
#endif /* 0 */

      //@{
      /** @name Event Handler overloads
       */
      virtual int resume_handler (void);
      virtual int close_connection (void);
      virtual int handle_input (ACE_HANDLE);
      virtual int handle_output (ACE_HANDLE);
      virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
      virtual int close (u_long = 0);
      //@}

      /// Add ourselves to Cache.
      int add_transport_to_cache (void);

      /// Process the <listen_list>
      int process_listen_point_list (::HTIOP::ListenPointList &listen_list);

      ///Set the Diff-Serv codepoint if the Policy dicates the setting of
      ///Network Priority This method is a no-op, but implemented to
      ///satisfy the base class interface.
      int set_dscp_codepoint (CORBA::Boolean set_network_priority);
      int set_dscp_codepoint (CORBA::Long dscp);


      //@{
      /**
       * @name TAO_Connection Handler overloads
       */
      virtual int release_os_resources (void);
      //@}

    private:
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* HTIOP_CONNECTION_HANDLER_H */
