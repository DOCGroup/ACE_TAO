//$Id$
#include "Connection_Resolver.h"


namespace TAO
{
  Profile_Connection_Resolver::Profile_Connection_Resolver (TAO_Stub *stub)
    : stub_ (stub)
      , transport_ (0)
  {
  }

  Profile_Connection_Resolver::~Profile_Connection_Resolver (void)
  {
    if (this->profile_)
      this->profile_->_decr_refcnt ();

    TAO_Transport::release (this->transport_);
  }

  TAO_Stub *
  Profile_Connection_Resolver::stub (void)
  {
    return this->stub_;
  }

  void
  Profile_Connection_Resolver::profile (TAO_Profile *p)
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
  Profile_Connection_Resolver::profile (void) const
  {
    return this->profile_;
  }


  TAO_Transport *
  Profile_Connection_Resolver::resolve (TAO_Invocation_Endpoint_Selector *es
                                        ACE_ENV_ARG_DECL)
  {
    es->select_endpoint (this
                         ACE_ENV_ARG_DECL);
    ACE_CHECK;

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

    return this->transport_;
  }

  int
  Profile_Connection_Resolver::try_connect (TAO_Endpoint *ep
                                            ACE_ENV_ARG_PARAMETER)
  {


    TAO_Connector_Registry *conn_reg =
      this->stub_->orb_core ()->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

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
     conn_reg->get_connector (desc.endpoint ())->connect (this,
                                                          &desc,
                                                          max_wait_time
                                                          ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   // A timeout error occurred
   if (result == -1 && errno == ETIME)
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
              1);
        }
    }

   // Return that we'd like to try another endpoint.
   return 0;
  }


}
