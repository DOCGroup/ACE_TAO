// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CONNECTION_HANDLER_H
#define TAO_SSLIOP_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Current.h"
#include "orbsvcs/SSLIOP/SSLIOP_Transport.h"

#include "tao/Connection_Handler.h"
#include "tao/IIOPC.h"

#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IIOP_Properties;

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @class Connection_Handler
     *
     * @brief  Handles requests on a single connection.
     *
     * The Connection handler which is common for the Acceptor and
     * the Connector
     */
    class Connection_Handler
      : public SVC_HANDLER,
        public TAO_Connection_Handler
    {
    public:

      Connection_Handler (ACE_Thread_Manager* t = 0);

      /// Constructor.
      Connection_Handler (TAO_ORB_Core *orb_core);

      /// Destructor.
      ~Connection_Handler (void);

      /**
       * @name Connection Handler overloads
       *
       * Connection Handler overloads.
       */
      //@{
      virtual int open_handler (void *);
      //@}

      /// Close called by the Acceptor or Connector when connection
      /// establishment fails.
      int close (u_long = 0);

      //@{
      /** @name Event Handler overloads
       */
      virtual int resume_handler (void);
      virtual int close_connection (void);
      virtual int handle_input (ACE_HANDLE);
      virtual int handle_output (ACE_HANDLE);
      virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
      virtual int handle_timeout (const ACE_Time_Value &current_time,
                                  const void *act = 0);
      virtual int open (void *);
      //@}

      /// Add ourselves to cache.
      int add_transport_to_cache (void);

      /// Process the @a listen_list.
      int process_listen_point_list (IIOP::ListenPointList &listen_list);

      /// Make the SSL session state available to the SSLIOP::Current
      /// object.
      int setup_ssl_state (TAO::SSLIOP::Current_Impl *&previous_current_impl,
                           TAO::SSLIOP::Current_Impl *new_current_impl,
                           bool &setup_done);

      /// Teardown the SSL session state.
      void teardown_ssl_state (
        TAO::SSLIOP::Current_Impl *previous_current_impl,
        bool &setup_done);

    protected:

      //@{
      /**
       * @name TAO_Connection Handler overloads
       */
      virtual int release_os_resources (void);
      virtual void pos_io_hook (int & return_value);
      virtual int handle_write_ready (const ACE_Time_Value *timeout);
      //@}

    protected:

      /// Reference to the SSLIOP::Current object (downcast to gain
      /// access to the low-level management methods).
      TAO::SSLIOP::Current_var current_;

    private:

      /// TCP configuration for this connection.
      TAO_IIOP_Properties *tcp_properties_;
    };

    // ****************************************************************

    /**
     * @class State_Guard
     *
     * @brief This Class that sets up TSS SSL states upon
     *        instantiation, and tears down the TSS SSL state when
     *        that instance goes out of scope.
     *
     * This guard is used to make TSS SSL state configuration and
     * deconfiguration during an upcall exception safe.  Exceptions
     * are not supposed to be propagated up to the scope this guard is
     * used in, so this guard may be unnecessary.  However, since
     * proper TSS SSL state configuration/deconfiguration is critical
     * to proper security support, this guard is used to ensure that
     * configuration/deconfiguration is exception safe.
     */
    class State_Guard
    {
    public:

      /// Constructor that sets up the TSS SSL state.
      State_Guard (TAO::SSLIOP::Connection_Handler *handler,
                   int &result);

      /// Destructor that tears down the TSS SSL state.
      ~State_Guard (void);

    private:

      /// Pointer to the connection handler currently handling the
      /// request/upcall.
      Connection_Handler *handler_;

      /// The SSLIOP::Current implementation that was previously
      /// associated with the current thread and invocation.
      /**
       * It is stored here until the invocation completes, after which it
       * placed back into TSS.
       */
      Current_Impl *previous_current_impl_;

      /// The SSLIOP::Current implementation to be associated with the
      /// current invocation.
      Current_Impl current_impl_;

      /// Flag that specifies whether or not setup of the SSLIOP::Current
      /// object completed for the current thread and invocation.
      bool setup_done_;
    };
  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (__ACE_INLINE__)
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif /* TAO_SSLIOP_CONNECTION_HANDLER_H */
