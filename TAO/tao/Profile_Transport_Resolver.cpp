//$Id$
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

ACE_RCSID (tao,
           Profile_Transport_Resolver,
           "$Id$")

namespace TAO
{
  Profile_Transport_Resolver::Profile_Transport_Resolver (TAO_Stub *stub)
    : stub_ (stub)
      , transport_ (0)
      , profile_ (0)
  {
  }

  Profile_Transport_Resolver::~Profile_Transport_Resolver (void)
  {
    if (this->profile_)
      this->profile_->_decr_refcnt ();

    this->transport_->make_idle ();
    TAO_Transport::release (this->transport_);
  }

  TAO_Stub *
  Profile_Transport_Resolver::stub (void) const
  {
    return this->stub_;
  }

  void
  Profile_Transport_Resolver::profile (TAO_Profile *p)
  {
    // @@ NOTE:This is just a workaround for a more serious problem with
    // profile management. This would cover some potential issues that
    // Ossama is working on.
    // Ossama, please remove them when you are done.
    TAO_Profile *tmp = this->profile_;

    // Dont do anything if the incoming profile is null
    if (p)
      {
        (void) p->_incr_refcnt ();
        this->profile_ = p;

        if (tmp)
          (void) tmp->_decr_refcnt ();
    }
  }

  TAO_Profile *
  Profile_Transport_Resolver::profile (void) const
  {
    return this->profile_;
  }

  TAO_Transport *
  Profile_Transport_Resolver::transport (void) const
  {
    return this->transport_;
  }


  void
  Profile_Transport_Resolver::resolve (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    TAO_Invocation_Endpoint_Selector *es =
      this->stub_->orb_core ()->endpoint_selector_factory ()->get_selector (
        ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    es->select_endpoint (this
                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->transport_ == 0)
      ACE_THROW (CORBA::INTERNAL ());

    const TAO_GIOP_Message_Version& version =
      this->profile_->version ();

    // Initialize the messaging object
    if (this->transport_->messaging_init (version.major,
                                          version.major) == -1)
      ACE_THROW (CORBA::INTERNAL (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EINVAL),
                        CORBA::COMPLETED_NO));

    if (!this->transport_->is_tcs_set())
      this->stub_->orb_core ()->codeset_manager()->
        set_tcs (*this->profile_,*this->transport_);
  }

  bool
  Profile_Transport_Resolver::try_connect (TAO_Endpoint *ep
                                           ACE_ENV_ARG_DECL)
  {
    TAO_Connector_Registry *conn_reg =
      this->stub_->orb_core ()->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
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

    // Get the max_wait_time
    ACE_Time_Value *max_wait_time = 0;
    ACE_Time_Value connection_timeout;
    int is_conn_timeout = 0;
#if 0
  // Check for the connection timout policy in the ORB
  this->orb_core ()->connection_timeout (this->stub (),
                                         is_conn_timeout,
                                         connection_timeout);

  // If a connection timeout policy is set, use that as the timeout
  // value.
   if (!is_conn_timeout)
     max_wait_time =
       this->max_wait_time ();
   else
     max_wait_time = &connection_timeout;
#endif /*fi 0*/
   // Obtain a connection.
   this->transport_ =
     conn_reg->get_connector (ep)->connect (this,
                                            ep,
                                            max_wait_time
                                            ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (false);

   // A timeout error occurred
   if (this->transport_ == 0 && errno == ETIME)
     {
       // If the user has set a roundtrip timeout policy, then throw a
       // timeout exception, else just fall through and return 0 to
       // look at the next endpoint
       if (!is_conn_timeout)
         {
          ACE_THROW_RETURN (CORBA::TIMEOUT (
              CORBA::SystemException::_tao_minor_code (
                TAO_TIMEOUT_CONNECT_MINOR_CODE,
                errno),
              CORBA::COMPLETED_NO),
              false);
         }
     }
   else if (this->transport_ == 0)
     return false;

   return true;
  }


}
