// $Id$

#include "FT_ClientRequest_Interceptor.h"
#include "orbsvcs/orbsvcs/FT_CORBA_ORBC.h"
#include "tao/CORBA_String.h"
#include "ace/UUID.h"

ACE_RCSID (FaultTolerance,
           FT_ORBInitializer,
           "$Id$")

TAO_FT_ClientRequest_Interceptor::TAO_FT_ClientRequest_Interceptor (void)
  : name_ ("TAO_FT_ClientRequest_Interceptor")
    , uuid_ (ACE_Utils::UUID_GENERATOR::instance ()->generateUUID ())
{
}

TAO_FT_ClientRequest_Interceptor::~TAO_FT_ClientRequest_Interceptor (void)
{
  delete this->uuid_;
}

char *
TAO_FT_ClientRequest_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_);
}


void
TAO_FT_ClientRequest_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_FT_ClientRequest_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
TAO_FT_ClientRequest_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (TAO_debug_level > 3)
    {
      CORBA::String_var op =
        ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "TAO_FT (%P|%t): %s called for %s \n",
                  this->name_,
                  op.in ()));
    }

  this->group_version_context (ri
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->request_service_context (ri
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_FT_ClientRequest_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Do Nothing
}

void
TAO_FT_ClientRequest_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

void
TAO_FT_ClientRequest_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
#if 0
  // @@ Will be used later.
  // Do a check for policy in which this can be done..
  PortableInterceptor::ReplyStatus rs =
    ri->reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (rs != PortableInterceptor::SYSTEM_EXCEPTION)
    {
      return;
    }

  CORBA::Any_var ex =
    ri->received_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_OutputCDR cdr;

  if (!ex->impl ()->marshal_value (cdr))
    return;

  // Place for optimization..
  TAO_InputCDR icdr (cdr);

  CORBA::String_var rep_id;

  CORBA::ULong min, cs = 0;

  if (!(icdr.read_string (rep_id.out ()) &&
        icdr.read_ulong (min) &&
        icdr.read_ulong (cs)))
    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                               CORBA::COMPLETED_MAYBE));

  cs = CORBA::CompletionStatus (cs);

  // ORB would take care of it
  if (cs != CORBA::COMPLETED_MAYBE)
    return;

  // If it is COMPLETED_MAYBE, then we will throw a COMPLETED_NO,
  // which the ORB should understand
  if (ACE_OS_String::strcmp (rep_id.in (),
                             "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)
    ACE_THROW (CORBA::TRANSIENT (min,
                                 CORBA::COMPLETED_NO));
  else if (ACE_OS_String::strcmp (rep_id.in (),
                                  "IDL:omg.org/CORBA/COMM_FAILURE:1.0") == 0)
    ACE_THROW (CORBA::COMM_FAILURE (min,
                                    CORBA::COMPLETED_NO));
  else if (ACE_OS_String::strcmp (rep_id.in (),
                                  "IDL:omg.org/CORBA/NO_REPONSE:1.0") == 0)
    ACE_THROW (CORBA::NO_RESPONSE (min,
                                   CORBA::COMPLETED_NO));

  else if (ACE_OS_String::strcmp (rep_id.in (),
                                  "IDL:omg.org/CORBA/OBJ_ADAPTER:1.0") == 0)
    ACE_THROW (CORBA::OBJ_ADAPTER (min,
                                   CORBA::COMPLETED_NO));
  cout << "Didnt throw exception " << endl;
#endif /*if 0*/
  return;
}


void
TAO_FT_ClientRequest_Interceptor::group_version_context (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If the service context is already there dont add new service
  // context information..
  ACE_TRY
    {
      IOP::ServiceContext_var sc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      IOP::TaggedComponent_var tp =
        ri->get_effective_component (IOP::TAG_FT_GROUP
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Grab the object group version
      // @@ NOTE: This involves an allocation and a dellocation. This is
      // really bad.
      TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                              tp->component_data.get_buffer ()
                                              ),
                        tp->component_data.length ());
      CORBA::Boolean byte_order;

      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return;

      cdr.reset_byte_order (ACE_static_cast (int,byte_order));

      FT::TagFTGroupTaggedComponent fgtc;

      if ((cdr >> fgtc) == 0)
        ACE_THROW (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28,
                                     CORBA::COMPLETED_NO));

      IOP::ServiceContext sc;
      sc.context_id = IOP::FT_GROUP_VERSION;

      TAO_OutputCDR ocdr;
      if (!(ocdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
        ACE_THROW (CORBA::MARSHAL ());

      if (!(ocdr << fgtc))
        ACE_THROW (CORBA::MARSHAL ());

      CORBA::ULong length =
        ACE_static_cast (CORBA::ULong, ocdr.total_length ());
      sc.context_data.length (length);
      CORBA::Octet *buf = sc.context_data.get_buffer ();

      for (const ACE_Message_Block *i = ocdr.begin ();
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
          buf += i->length ();
        }

      // Add this context to the service context list.
      ri->add_request_service_context (sc,
                                       0
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

    }
  ACE_CATCHANY
    {
      // Not much can be done anyway. Just throw
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  return;
}

void
TAO_FT_ClientRequest_Interceptor::request_service_context (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If the service context is already there dont add new service
  // context information..
  ACE_TRY
    {
      IOP::ServiceContext_var sc =
        ri->get_request_service_context (IOP::FT_REQUEST
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      IOP::ServiceContext sc;
      sc.context_id = IOP::FT_REQUEST;

      CORBA::Policy_var policy =
        ri->get_request_policy (FT::REQUEST_DURATION_POLICY
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // @@ NOTE: This needs fixing if something useful needs to
      // happen.
      FT::FTRequestServiceContext ftrsc;
      ftrsc.client_id = CORBA::string_dup (this->uuid_->to_string ()->c_str ());
      ftrsc.expiration_time =
        this->request_expiration_time (policy.in ()
                                       ACE_ENV_ARG_PARAMETER);

      ftrsc.retention_id =
        ri->request_id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // @@ NOTE we need to generate a UUID here

      TAO_OutputCDR ocdr;
      if (!(ocdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
        ACE_THROW (CORBA::MARSHAL ());

      if (!(ocdr << ftrsc))
        ACE_THROW (CORBA::MARSHAL ());

      // Make a *copy* of the CDR stream...
      CORBA::ULong length =
        ACE_static_cast (CORBA::ULong, ocdr.total_length ());
      sc.context_data.length (length);
      CORBA::Octet *buf = sc.context_data.get_buffer ();

      for (const ACE_Message_Block *i = ocdr.begin ();
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
          buf += i->length ();
        }

      // Add this context to the service context list.
      ri->add_request_service_context (sc,
                                       0
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
  return;
}


TimeBase::TimeT
TAO_FT_ClientRequest_Interceptor::request_expiration_time (
    CORBA::Policy *policy
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  FT::RequestDurationPolicy_var p;

  if (policy != 0)
    {
      p =  FT::RequestDurationPolicy::_narrow (policy
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  TimeBase::TimeT t = 0;

  if (p.in ())
    {
      t =
        p->request_duration_policy_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) TAO_FT - Using default value",
                      " of 15 seconds \n"));
        }
      t = 15 * 1000000;
    }

  // Calculaton of the expiration time

  // Grab the localtime on the machine where this is running
  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  TimeBase::TimeT sec_part  = time_val.sec () * 10000000;
  TimeBase::TimeT usec_part = time_val.usec ()* 10;

  // Now we have the total time
  t += (sec_part + usec_part);

  return t;
}
