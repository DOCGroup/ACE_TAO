// $Id$
#include "Profile_Transport_Resolver.h"
#include "Profile.h"
#include "Transport.h"
#include "Stub.h"
#include "Invocation_Endpoint_Selectors.h"
#include "ORB_Core.h"
#include "Endpoint_Selector_Factory.h"
#include "Codeset_Manager.h"
#include "Connector_Registry.h"
#include "Transport_Connector.h"
#include "Endpoint.h"

#if !defined (__ACE_INLINE__)
# include "Profile_Transport_Resolver.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Profile_Transport_Resolver,
           "$Id$")

namespace TAO
{

  Profile_Transport_Resolver::~Profile_Transport_Resolver (void)
  {
    if (this->profile_)
      {
        this->profile_->_decr_refcnt ();
      }

    if (this->transport_)
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
  Profile_Transport_Resolver::resolve (ACE_Time_Value *max_time_val,
                                       bool block
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_Countdown_Time countdown (max_time_val);

    TAO_Invocation_Endpoint_Selector *es =
      this->stub_->orb_core ()->endpoint_selector_factory ()->get_selector (
          ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

// here we select a connection
    es->select_endpoint (this,
                         max_time_val, block
                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->transport_ == 0)
      {
        ACE_THROW (CORBA::INTERNAL ());
      }

    const TAO_GIOP_Message_Version& version =
      this->profile_->version ();

    // Initialize the messaging object
    if (this->transport_->messaging_init (version.major, version.minor) == -1)
      {
        ACE_THROW (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO));
      }

    if (!this->transport_->is_tcs_set ())
      {
        TAO_Codeset_Manager *tcm =
          this->stub_->orb_core ()->codeset_manager ();
        tcm->set_tcs (*this->profile_, *this->transport_);
      }
  }


// need a bool here
  bool
  Profile_Transport_Resolver::try_connect (
      TAO_Transport_Descriptor_Interface *desc,
       ACE_Time_Value *max_time_value,
       bool block
       ACE_ENV_ARG_DECL
     )
  {
    TAO_Connector_Registry *conn_reg =
      this->stub_->orb_core ()->connector_registry (
                                    ACE_ENV_SINGLE_ARG_PARAMETER
                                  );
    ACE_CHECK_RETURN (false);

    if (conn_reg == 0)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
              TAO_DEFAULT_MINOR_CODE,
              EINVAL),
            CORBA::COMPLETED_NO),
                          0);
      }

    ACE_Time_Value connection_timeout;

    const bool is_conn_timeout =
      this->get_connection_timeout (connection_timeout);


    ACE_Time_Value *max_wait_time = 0;

    if (is_conn_timeout == true)
      {
        max_wait_time = &connection_timeout;
      }
    else
      {
        max_wait_time = max_time_value;
      }


// here to pass a boolean or not, so
//      bool block = true;
    // Obtain a connection.
    this->transport_ =
      conn_reg->get_connector (desc->endpoint ()->tag ())->connect (
        this,
        desc,
        max_wait_time,
        block
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (false);

    // A timeout error occurred
    // If the user has set a roundtrip timeout policy, then throw a
    // timeout exception, else just fall through and return false to
    // look at the next endpoint
    if (this->transport_ == 0 &&
        errno == ETIME &&
        is_conn_timeout == false)
      {
        ACE_THROW_RETURN (CORBA::TIMEOUT (
            CORBA::SystemException::_tao_minor_code (
                 TAO_TIMEOUT_CONNECT_MINOR_CODE,
                 errno),
            CORBA::COMPLETED_NO),
            false);
      }
    else if (this->transport_ == 0)
      {
        return false;
      }

    return true;
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
  Profile_Transport_Resolver::init_inconsistent_policies (
    ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_NEW_THROW_EX (this->inconsistent_policies_,
                      CORBA::PolicyList (0),
                      CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  }
}
