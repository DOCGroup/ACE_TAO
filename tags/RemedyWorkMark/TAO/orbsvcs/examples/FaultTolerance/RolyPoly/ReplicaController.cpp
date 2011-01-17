// file      : RolyPoly/ReplicaController.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PI/PI.h"

#include "orbsvcs/FT_CORBA_ORBC.h"

#include "ace/UUID.h"
#include "ace/Thread_Manager.h"
#include "ace/TMCast/Group.hpp"

#include "CrashPoint.h"
#include "StateUpdate.h"
#include "ReplicaController.h"


// State slot.
//
//

namespace
{
  PortableInterceptor::SlotId state_slot_id_;
}

PortableInterceptor::SlotId
state_slot_id ()
{
  return state_slot_id_;
}

void
state_slot_id (PortableInterceptor::SlotId slot_id)
{
  state_slot_id_ = slot_id;
}

Checkpointable::
~Checkpointable ()
{
}

CORBA::Any* Checkpointable::
get_state ()
{
  return 0;
}

void Checkpointable::
associate_state (CORBA::ORB_ptr orb, CORBA::Any const& state)
{
  try
  {
    CORBA::Object_var pic_obj =
      orb->resolve_initial_references ("PICurrent");


    PortableInterceptor::Current_var pic =
      PortableInterceptor::Current::_narrow (
        pic_obj.in ());


    pic->set_slot (state_slot_id (), state);

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Caught exception:");
  }
}

// ReplyLogger
//
//

ReplicaController::
~ReplicaController ()
{
}

ReplicaController::
ReplicaController (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
  try
  {
    CORBA::Object_var poa_object =
      orb_->resolve_initial_references ("RootPOA");

    root_poa_ = PortableServer::POA::_narrow (
      poa_object.in ());
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Caught exception:");
    ACE_OS::abort ();
  }

  // Generate member id
  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->init ();
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  ACE_INET_Addr address (10000, "239.255.0.1");

  ACE_DEBUG ((LM_DEBUG, "Becoming a member with id %s\n",
              uuid.to_string ()->c_str ()));

  ACE_auto_ptr_reset (group_, new ACE_TMCast::Group (address, uuid.to_string ()->c_str ()));

  int r = ACE_Thread_Manager::instance ()->spawn (
    &ReplicaController::listener_thunk, this);

  if (r < 0)
  {
    orb_->shutdown (0);
  }
}

void ReplicaController::
listener ()
{
  try
  {
    for (char buffer[1024];;)
    {
      size_t n = group_->recv (buffer, sizeof (buffer));

      ACE_HEX_DUMP ((LM_DEBUG, buffer, n));

      TAO_InputCDR cdr (buffer, n);

      CORBA::OctetSeq object_id;
      PortableInterceptor::AdapterName adapter_name;
      CORBA::String_var client_id;
      CORBA::Long retention_id;
      CORBA::OctetSeq reply;
      CORBA::Any state;

      cdr >> object_id;
      cdr >> adapter_name;
      cdr >> client_id.out ();
      cdr >> retention_id;
      cdr >> reply;
      cdr >> state;

      if (!cdr.good_bit ())
      {
        ACE_DEBUG ((LM_DEBUG, "CDR failed\n"));
        //@@ what to do?
      }

      ACE_DEBUG ((LM_DEBUG,
                  "Received log for %s with rid %i\n",
                  client_id.in (),
                  retention_id));


      RecordId rid (client_id.in (), retention_id);

      CORBA::OctetSeq_var tmp (new CORBA::OctetSeq (reply));
      log_.insert (rid, tmp);

      // Update state.
      CORBA::TypeCode_var tc = state.type ();

      if (tc->kind () != CORBA::tk_null)
      {
        PortableServer::POA_var poa = resolve_poa (adapter_name);

        PortableServer::ServantBase_var servant =
          poa->id_to_servant (object_id);

        Checkpointable* target =
          dynamic_cast<Checkpointable*> (servant.in ());

        if (target) target->set_state (state);
      }
    }
  }
  catch (ACE_TMCast::Group::Failed const&)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Group failure. Perhaps, I am alone in the group.\n"));
  }
  catch (ACE_TMCast::Group::InsufficienSpace const&)
  {
    ACE_DEBUG ((LM_DEBUG, "Group::InsufficienSpace\n"));
  }

  orb_->shutdown (0);
}

PortableServer::POA_ptr ReplicaController::
resolve_poa (PortableInterceptor::AdapterName const&)
{
  //@@ Assume for now it's a root poa.
  return PortableServer::POA::_duplicate (root_poa_.in ());
}


ACE_THR_FUNC_RETURN ReplicaController::
listener_thunk (void* p)
{
  ReplicaController* obj = reinterpret_cast<ReplicaController*> (p);
  obj->listener();
  return 0;
}

namespace
{
  FT::FTRequestServiceContext*
  extract_context (
    PortableInterceptor::ServerRequestInfo_ptr ri);
}

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
void
ReplicaController::tao_ft_interception_point (
  PortableInterceptor::ServerRequestInfo_ptr ri,
  CORBA::OctetSeq_out ocs)
{
  FT::FTRequestServiceContext_var ftr (
    extract_context (ri));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Received request from %s with rid %i\n",
              ftr->client_id.in (),
              ftr->retention_id));

  // Check if this request is eligible for replay.

  RecordId rid (ftr->client_id.in (), ftr->retention_id);

  if (log_.contains (rid))
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Replaying reply for %s with rid %i\n",
                ftr->client_id.in (),
                ftr->retention_id));

    CORBA::OctetSeq_var copy (log_.lookup (rid)); // make a copy

    ocs = copy._retn ();
  }

  return;
}

#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

void
ReplicaController::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  FT::FTRequestServiceContext_var ftr (
    extract_context (ri));


  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Sending reply for %s with rid %i\n",
              ftr->client_id.in (),
              ftr->retention_id));


  // Prepare reply for logging.

  CORBA::Any_var result =
    ri->result ();

  TAO_OutputCDR cdr;
  result->impl ()->marshal_value (cdr);

  Dynamic::ParameterList_var pl =
    ri->arguments ();

  CORBA::ULong len = pl->length ();

  for (CORBA::ULong index = 0; index != len ; ++index)
  {
    //@@ No chance for PARAM_OUT
    if ((*pl)[index].mode == CORBA::PARAM_INOUT)
    {
      (*pl)[index].argument.impl ()->marshal_value (cdr);
    }
  }

  CORBA::OctetSeq_var reply;

  ACE_NEW (reply.out (), CORBA::OctetSeq (cdr.total_length ()));

  reply->length (cdr.total_length ());

  CORBA::Octet* buf = reply->get_buffer ();

  // @@ What if this throws an exception??  We don't have any way to
  // check whether this succeeded
  for (ACE_Message_Block const* mb = cdr.begin ();
       mb != 0;
       mb = mb->cont ())
  {
    ACE_OS::memcpy (buf, mb->rd_ptr (), mb->length ());
    buf += mb->length ();
  }

  // Logging the reply and state update.
  //

  // First send message to members.
  //
  {
    // Extract state update.

    CORBA::OctetSeq_var oid = ri->object_id ();
    PortableInterceptor::AdapterName_var an = ri->adapter_name ();

    CORBA::Any_var state = ri->get_slot (state_slot_id ());

    CORBA::TypeCode_var tc = state->type ();

    if (tc->kind () == CORBA::tk_null)
    {
      ACE_DEBUG ((LM_DEBUG, "Slot update is void\n"));

      PortableServer::POA_var poa = resolve_poa (an.in ());

      PortableServer::ServantBase_var servant =
        poa->id_to_servant (oid.in ());

      Checkpointable* target =
        dynamic_cast<Checkpointable*> (servant.in ());

      if (target)
      {
        CORBA::Any_var tmp = target->get_state ();

        if (tmp.ptr () != 0) state = tmp._retn ();
      }
    }

    TAO_OutputCDR cdr;

    cdr << oid.in ();
    cdr << an.in ();
    cdr << ftr->client_id.in ();
    cdr << ftr->retention_id;
    cdr << reply.in ();
    cdr << state.in ();

    size_t size = cdr.total_length ();

    CORBA::OctetSeq_var msg;

    ACE_NEW (msg.out (), CORBA::OctetSeq (size));

    msg->length (size);

    {
      CORBA::Octet* buf = msg->get_buffer ();

      for (ACE_Message_Block const* mb = cdr.begin ();
           mb != 0;
           mb = mb->cont ())
      {
        ACE_OS::memcpy (buf, mb->rd_ptr (), mb->length ());
        buf += mb->length ();
      }
    }

    CORBA::Octet* buf = msg->get_buffer ();

    // Crash point 1.
    //
    if (crash_point == 1 && ftr->retention_id > 2) ACE_OS::exit (1);

    try
    {
      while (true)
      {
        try
        {
          group_->send (buf, size);
          ACE_DEBUG ((LM_DEBUG, "Sent log record of length %i\n", size));
          break;
        }
        catch (ACE_TMCast::Group::Aborted const&)
        {
          ACE_DEBUG ((LM_DEBUG, "Retrying to send log record.\n"));
        }
      }
    }
    catch (ACE_TMCast::Group::Failed const&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Group failure. Perhaps, I am alone in the group.\n"));
    }
  }


  // Now perform local logging.
  //
  RecordId rid (ftr->client_id.in (), ftr->retention_id);

  // This is slow but eh-safe ;-).
  //
  log_.insert (rid, reply);


  // Crash point 2.
  //
  if (crash_point == 2 && ftr->retention_id > 2) ACE_OS::exit (1);
}


namespace
{
  FT::FTRequestServiceContext*
  extract_context (PortableInterceptor::ServerRequestInfo_ptr ri)
  {
    IOP::ServiceContext_var svc =
      ri->get_request_service_context (IOP::FT_REQUEST);

    TAO_InputCDR cdr (reinterpret_cast<const char*> (svc->context_data.get_buffer ()),
                      svc->context_data.length ());

    CORBA::Boolean byte_order;

    if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    {
      //@@ what to throw?
      throw CORBA::BAD_CONTEXT ();
    }

    cdr.reset_byte_order (static_cast<int> (byte_order));

    // Funny, the following two lines should normally translate
    // just to one ctor call. But because we have to use this
    // ACE_NEW macro hackery we have a default ctor call plus
    // assignment operator call. Yet another example how the
    // majority is being penalized by some broken platforms.
    //
    FT::FTRequestServiceContext_var req;

    //@@ completed status maybe wrong
    //
    ACE_NEW_THROW_EX (req,
                      FT::FTRequestServiceContext,
                      CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                          TAO::VMCID,
                          ENOMEM),
                        CORBA::COMPLETED_NO));

    cdr >> *req;

    if (!cdr.good_bit ())
    {
      //@@ what to throw?
      throw CORBA::UNKNOWN ();
    }

    return req._retn ();
  }
}


char*
ReplicaController::name (void)
{
  return CORBA::string_dup ("ReplicaController");
}

void
ReplicaController::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ReplicaController::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ReplicaController::destroy (void)
{
}

void
ReplicaController::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{

}

void
ReplicaController::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
