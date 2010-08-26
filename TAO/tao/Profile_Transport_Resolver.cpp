// -*- C++ -*-
// $Id$

#include "tao/Profile_Transport_Resolver.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/Transport.h"
#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Transport_Descriptor_Interface.h"
#include "tao/Endpoint_Selector_Factory.h"
#include "tao/Codeset_Manager.h"
#include "tao/Connector_Registry.h"
#include "tao/Transport_Connector.h"
#include "tao/Endpoint.h"
#include "tao/SystemException.h"
#include "tao/Client_Strategy_Factory.h"

#include "ace/Countdown_Time.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Profile_Transport_Resolver.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  Profile_Transport_Resolver::~Profile_Transport_Resolver (void)
  {
    if (this->profile_)
      {
        this->profile_->_decr_refcnt ();
      }

    if (this->transport_.get ())
      {
        if (this->is_released_ == false)
          {
            this->transport_->make_idle ();
          }

        this->transport_->remove_reference ();
      }

    delete this->inconsistent_policies_;
  }

  void
  Profile_Transport_Resolver::profile (TAO_Profile *p)
  {
    // Dont do anything if the incoming profile is null
    if (p)
      {
        // @note This is just a workaround for a more serious problem
        // with profile management. This would cover some potential
        // issues that Ossama is working on.  Ossama, please remove
        // them when you are done.
        TAO_Profile *tmp = this->profile_;

        (void) p->_incr_refcnt ();
        this->profile_ = p;

        if (tmp)
          {
            (void) tmp->_decr_refcnt ();
          }
      }
  }


  void
  Profile_Transport_Resolver::resolve (ACE_Time_Value *max_time_val)
  {
    ACE_Countdown_Time countdown (max_time_val);

    TAO_Invocation_Endpoint_Selector *es =
      this->stub_->orb_core ()->endpoint_selector_factory ()->get_selector ();

    // Select the endpoint
    es->select_endpoint (this, max_time_val);

    if (this->transport_.get () == 0)
      {
        // No useable endpoint could be found. We will not
        // be able to send the message. Wait to throw an exception until
        // after the send_request interception point has been called.
        return;
      }

    TAO_GIOP_Message_Version const & version = this->profile_->version ();

    // Initialize the messaging object
    this->transport_->messaging_init (version);

    if (!this->transport_->is_tcs_set ())
      {
        TAO_Codeset_Manager * const tcm =
          this->stub_->orb_core ()->codeset_manager ();
        if (tcm)
          tcm->set_tcs (*this->profile_, *this->transport_);
      }
  }

  bool
  Profile_Transport_Resolver::try_connect (
      TAO_Transport_Descriptor_Interface *desc,
      ACE_Time_Value *timeout)
  {
    return this->try_connect_i (desc, timeout, false);
  }

  bool
  Profile_Transport_Resolver::try_parallel_connect (
       TAO_Transport_Descriptor_Interface *desc,
       ACE_Time_Value *timeout)
  {
    return this->try_connect_i (desc, timeout, true);
  }


  bool
  Profile_Transport_Resolver::try_connect_i (
       TAO_Transport_Descriptor_Interface *desc,
       ACE_Time_Value *timeout,
       bool parallel)
  {
    TAO_Connector_Registry *conn_reg =
      this->stub_->orb_core ()->connector_registry ();

    if (conn_reg == 0)
      {
        throw ::CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            0,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    ACE_Time_Value connection_timeout;
    bool has_con_timeout = this->get_connection_timeout (connection_timeout);

    if (has_con_timeout && !this->blocked_)
      {
        timeout = &connection_timeout;
      }
    else if (has_con_timeout)
      {
        if (timeout == 0 || connection_timeout < *timeout)
          timeout = &connection_timeout;
        else
          has_con_timeout = false;
      }
    else if (!this->blocked_)
      {
        timeout = 0;
      }

    TAO_Connector *con = conn_reg->get_connector (desc->endpoint ()->tag ());
    ACE_ASSERT(con != 0);
    if (parallel)
      {
        this->transport_.set (con->parallel_connect (this, desc, timeout));
      }
    else
      {
        this->transport_.set (con->connect (this, desc, timeout));
      }
    // A timeout error occurred.
    // If the user has set a roundtrip timeout policy, throw a timeout
    // exception.  Otherwise, just fall through and return false to
    // look at the next endpoint.
    if (this->transport_.get () == 0 &&
        has_con_timeout == false &&
        errno == ETIME)
      {
        throw ::CORBA::TIMEOUT (
          CORBA::SystemException::_tao_minor_code (
            TAO_TIMEOUT_CONNECT_MINOR_CODE,
            errno),
          CORBA::COMPLETED_NO);
      }
    else if (this->transport_.get () == 0)
      {
        return false;
      }
    else
      {
        // Determine the sync scope (if any)
        Messaging::SyncScope sync_scope;
        bool has_synchronization = false;
        this->stub_->orb_core ()->call_sync_scope_hook (this->stub_,
                                                        has_synchronization,
                                                        sync_scope);

        // If this stub has synchronization that's not "none", we need to
        // have the transport schedule output at the appropriate time.
        if (has_synchronization && sync_scope != Messaging::SYNC_NONE)
          {
            this->transport_->set_flush_in_post_open ();
          }
      }

    return true;
  }

  bool
  Profile_Transport_Resolver::use_parallel_connect (void) const
  {
    TAO_ORB_Core *oc = this->stub_->orb_core();
    return (oc->orb_params()->use_parallel_connects()
#if 0       // it was decided that even with blocked connects
            // parallel connects could be useful, at least for cache
            // processing.
            oc->client_factory()->connect_strategy() !=
            TAO_Client_Strategy_Factory::TAO_BLOCKED_CONNECT
#endif /* 0 */
            );
  }

  bool
  Profile_Transport_Resolver::get_connection_timeout (
    ACE_Time_Value &max_wait_time)
  {
    bool is_conn_timeout = false;

    // Check for the connection timout policy in the ORB
    this->stub_->orb_core ()->connection_timeout (this->stub_,
                                                  is_conn_timeout,
                                                  max_wait_time);

    return is_conn_timeout;
  }


  void
  Profile_Transport_Resolver::init_inconsistent_policies (void)
  {
    ACE_NEW_THROW_EX (this->inconsistent_policies_,
                      CORBA::PolicyList (0),
                      CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  }


  int
  Profile_Transport_Resolver::find_transport (TAO_Transport_Descriptor_Interface *desc)
  {
    TAO::Transport_Cache_Manager & cache =
      this->profile_->orb_core()->lane_resources ().transport_cache();

    // the cache increments the reference count on the transport if
    // the find is successful. We want to return a "boolean" of 0 for
    // failure, 1 for success.
    size_t busy_count;
    TAO_Transport* tmp = this->transport_.get ();
    if (cache.find_transport(desc, tmp, busy_count) !=
        Transport_Cache_Manager::CACHE_FOUND_AVAILABLE)
      return 0;

    this->transport_.set (tmp);
    return 1;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
