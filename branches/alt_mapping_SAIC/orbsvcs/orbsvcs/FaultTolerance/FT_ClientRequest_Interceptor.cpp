// $Id$

#include "orbsvcs/FaultTolerance/FT_ClientRequest_Interceptor.h"
#include "orbsvcs/FaultTolerance/FT_Service_Callbacks.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

#include "tao/CORBA_String.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "tao/PI/ClientRequestInfo.h"

#include "ace/UUID.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Lock.h"
#include "ace/Synch_Traits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FT_ClientRequest_Interceptor::FT_ClientRequest_Interceptor (void)
    : name_ ("TAO_FT_ClientRequest_Interceptor")
      , uuid_ (0)
      , lock_ (0)
      , retention_id_ (0)
      , ft_send_extended_sc_ (false)
  {
    ACE_Utils::UUID_GENERATOR::instance ()->init ();

    this->uuid_ =
      ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID ();

    // Would be nice to have runtime option.
    ACE_NEW (this->lock_,
             ACE_Lock_Adapter<TAO_SYNCH_MUTEX>);

  }

  FT_ClientRequest_Interceptor::~FT_ClientRequest_Interceptor (void)
  {
    delete this->uuid_;
    delete this->lock_;
  }

  char *
  FT_ClientRequest_Interceptor::name (void)
  {
    return CORBA::string_dup (this->name_);
  }

  void
  FT_ClientRequest_Interceptor::destroy (void)
  {
  }

  void
  FT_ClientRequest_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
  {
    // Do Nothing
  }

  void
  FT_ClientRequest_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    if (TAO_debug_level > 3)
      {
        CORBA::String_var op =
          ri->operation ();

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_FT (%P|%t) - %s called for %s\n"),
                    this->name_,
                    op.in ()));
      }

    IOP::TaggedComponent_var tp;
    try
      {
        tp = ri->get_effective_component (IOP::TAG_FT_GROUP);
      }
    catch (const CORBA::Exception&)
      {
        return;
      }

    this->group_version_context (ri, tp);

    this->request_service_context (ri);
  }

  void
  FT_ClientRequest_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
  {
  }

  void
  FT_ClientRequest_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    TAO_ClientRequestInfo* tao_ri = dynamic_cast<TAO_ClientRequestInfo*> (ri);

    if (!tao_ri)
      {
        throw CORBA::INTERNAL ();
      }

    TimeBase::TimeT expires = tao_ri->tao_ft_expiration_time ();

    if (!expires)
      {
        // Not an FT request
        return;
      }

    PortableInterceptor::ReplyStatus status = -1;

    try
    {
       status = ri->reply_status();
    }
    catch (const CORBA::Exception&)
    {
       // No reply status => Not a location forward.
       return;
    }

    if (status == PortableInterceptor::LOCATION_FORWARD)
      {
        // We are in an FT request and a location forward has been received.

        if (expires < TAO_FT_Service_Callbacks::now ())
          {
            // The request has already expired...

            if (TAO_debug_level > 3)
              {
                ACE_DEBUG ((LM_DEBUG,
                        "TAO_FT (%P|%t): FT_ClientRequest_Interceptor::receive_other - LOCATION_FORWARD received after request expiration.\n"));
              }

            // The spec says throw a SYSTEM_EXCEPTION, but doesn't specify which one.
            // I think a TRANSIENT is the most suitable.
            throw CORBA::TRANSIENT (
              CORBA::SystemException::_tao_minor_code (
                TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
                errno),
              CORBA::COMPLETED_NO);
          }
      }
  }

  void
  FT_ClientRequest_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
  {
#if 0
    // @@ Will be used later.
    // Do a check for policy in which this can be done..
    PortableInterceptor::ReplyStatus rs =
      ri->reply_status ();

    if (rs != PortableInterceptor::SYSTEM_EXCEPTION)
      {
        return;
      }

    CORBA::Any_var ex =
      ri->received_exception ();

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
      throw CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_MAYBE);

    cs = CORBA::CompletionStatus (cs);

    // ORB would take care of it
    if (cs != CORBA::COMPLETED_MAYBE)
      return;

    // If it is COMPLETED_MAYBE, then we will throw a COMPLETED_NO,
    // which the ORB should understand
    if (ACE_OS_String::strcmp (rep_id.in (),
                               "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)
      throw CORBA::TRANSIENT (min, CORBA::COMPLETED_NO);
    else if (ACE_OS_String::strcmp (rep_id.in (),
                                  "IDL:omg.org/CORBA/COMM_FAILURE:1.0") == 0)
      throw CORBA::COMM_FAILURE (min, CORBA::COMPLETED_NO);
    else if (ACE_OS_String::strcmp (rep_id.in (),
                                    "IDL:omg.org/CORBA/NO_REPONSE:1.0") == 0)
      throw CORBA::NO_RESPONSE (min, CORBA::COMPLETED_NO);

    else if (ACE_OS_String::strcmp (rep_id.in (),
                                    "IDL:omg.org/CORBA/OBJ_ADAPTER:1.0") == 0)
    throw CORBA::OBJ_ADAPTER (min, CORBA::COMPLETED_NO);
    cout << "Didnt throw exception " << endl;
#endif /*if 0*/
    return;
  }


  void
    FT_ClientRequest_Interceptor::group_version_context (
    PortableInterceptor::ClientRequestInfo_ptr ri,
    IOP::TaggedComponent* tp)
  {
    try
    {
      IOP::ServiceContext sc;
      sc.context_id = IOP::FT_GROUP_VERSION;

      if (this->ft_send_extended_sc_)
        {
          // We send the whole tagged component as a service context.
          sc.context_data.length (tp->component_data.length ());
          CORBA::Octet *sc_buf = sc.context_data.get_buffer ();
          ACE_OS::memcpy (sc_buf, tp->component_data.get_buffer (), tp->component_data.length ());
        }
      else
        {
          // Grab the object group version
          TAO_InputCDR cdr (reinterpret_cast<const char*> (tp->component_data.get_buffer ()),
            tp->component_data.length ());
          CORBA::Boolean byte_order;

          if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
            return;

          cdr.reset_byte_order (static_cast<int> (byte_order));

          FT::TagFTGroupTaggedComponent gtc;

          if (!(cdr >> gtc))
            throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

          TAO_OutputCDR ocdr;
          if (!(ocdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
            return;

          if (!(ocdr << gtc.object_group_ref_version))
            return;

          CORBA::ULong const length =
            static_cast<CORBA::ULong> (ocdr.total_length ());
          sc.context_data.length (length);
          CORBA::Octet *buf = sc.context_data.get_buffer ();

          for (const ACE_Message_Block *i = ocdr.begin ();
            i != 0;
            i = i->cont ())
          {
            ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
            buf += i->length ();
          }
        }
      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0);

    }
    catch (const CORBA::Exception&)
    {
      // Not much can be done anyway. Just keep quiet
    }

    return;
  }

  void
    FT_ClientRequest_Interceptor::request_service_context (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    try
    {
      IOP::ServiceContext sc;
      sc.context_id = IOP::FT_REQUEST;

      CORBA::Policy_var policy =
        ri->get_request_policy (FT::REQUEST_DURATION_POLICY);

      FT::FTRequestServiceContext ftrsc;
      ftrsc.client_id =
        CORBA::string_dup (this->uuid_->to_string ()->c_str ());

      TAO_ClientRequestInfo* tao_ri = dynamic_cast<TAO_ClientRequestInfo*> (ri);

      if (!tao_ri)
        {
          throw CORBA::INTERNAL ();
        }

      if (tao_ri->tao_ft_expiration_time ())
        {
          ftrsc.retention_id = tao_ri->tao_ft_retention_id ();
          ftrsc.expiration_time = tao_ri->tao_ft_expiration_time ();
        }
      else
        {
          ACE_GUARD  (ACE_Lock,
                      guard,
                      *this->lock_);

          ftrsc.retention_id = ++this->retention_id_;
          ftrsc.expiration_time =
              this->request_expiration_time (policy.in ());

          tao_ri->tao_ft_retention_id (ftrsc.retention_id);
          tao_ri->tao_ft_expiration_time (ftrsc.expiration_time);
        }

      TAO_OutputCDR ocdr;
      if (!(ocdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
        return;

      if (!(ocdr << ftrsc))
        return;

      // Make a *copy* of the CDR stream...
      CORBA::ULong length =
        static_cast<CORBA::ULong> (ocdr.total_length ());
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
      ri->add_request_service_context (sc, 0);
    }
    catch (const CORBA::Exception&)
    {
    }
  }

  TimeBase::TimeT
  FT_ClientRequest_Interceptor::request_expiration_time (
    CORBA::Policy *policy)
  {
    FT::RequestDurationPolicy_var p;

    if (policy != 0)
      {
        p =  FT::RequestDurationPolicy::_narrow (policy);
      }

    TimeBase::TimeT t = 0;

    if (p.in ())
      {
        t = p->request_duration_policy_value ();
      }
    else
      {
        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO_FT (%P|%t) - Using default value")
                        ACE_TEXT ("of 1.5 seconds\n")));
          }
        t = 15 * 1000000;
      }

    // Calculaton of the expiration time
    t += TAO_FT_Service_Callbacks::now ();

    return t;
  }

  void
  FT_ClientRequest_Interceptor::ft_send_extended_sc (CORBA::Boolean send)
  {
    this->ft_send_extended_sc_ = send;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
