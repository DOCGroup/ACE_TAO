// $Id$

#include "Request_Context_Repository.h"
#include "../Utils/resolve_init.h"
#include "../Utils/UUID.h"

ACE_RCSID (EventChannel,
           Request_Context_Repository,
           "$Id$")

namespace {
PortableInterceptor::SlotId object_id_slot;
PortableInterceptor::SlotId cached_result_slot;
PortableInterceptor::SlotId seq_num_slot;
PortableInterceptor::SlotId ft_request_service_context_slot;
PortableInterceptor::SlotId transaction_depth_slot;
CORBA::ORB_ptr orb;
}


void
Request_Context_Repository::allocate_slots(PortableInterceptor::ORBInitInfo_ptr info
            ACE_ENV_ARG_DECL)
{
  object_id_slot = info->allocate_slot_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  cached_result_slot = info->allocate_slot_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  seq_num_slot = info->allocate_slot_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ft_request_service_context_slot = info->allocate_slot_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  transaction_depth_slot = info->allocate_slot_id(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Request_Context_Repository::init(CORBA::ORB_ptr the_orb)
{
  orb = the_orb;
}

void Request_Context_Repository::generate_object_id(
  FtRtecEventChannelAdmin::ObjectId& oid
  ACE_ENV_ARG_DECL)
{
  oid.length(sizeof(UUID));
  UUID::create(oid.get_buffer());
  set_object_id(oid
                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Request_Context_Repository::set_object_id(
  const FtRtecEventChannelAdmin::ObjectId& object_id
  ACE_ENV_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Any a;
  a <<= object_id;
  pic->set_slot(object_id_slot, a
                ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

FtRtecEventChannelAdmin::ObjectId_var
get_object_id(CORBA::Any_var a
              ACE_ENV_ARG_DECL)
{
  FtRtecEventChannelAdmin::ObjectId* object_id;
  FtRtecEventChannelAdmin::ObjectId_var result;

  if ((*a >>= object_id) ==0)
    ACE_THROW_RETURN(CORBA::NO_MEMORY(), result);


  ACE_NEW_THROW_EX(result,
                   FtRtecEventChannelAdmin::ObjectId(*object_id),
                   CORBA::NO_MEMORY());
  return result;
}


FtRtecEventChannelAdmin::ObjectId_var
Request_Context_Repository::get_object_id(ACE_ENV_SINGLE_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(FtRtecEventChannelAdmin::ObjectId_var());

  CORBA::Any_var a = pic->get_slot(object_id_slot
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(FtRtecEventChannelAdmin::ObjectId_var());

  return ::get_object_id(a
                         ACE_ENV_ARG_PARAMETER);
}

FtRtecEventChannelAdmin::ObjectId_var
Request_Context_Repository::get_object_id(
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
{
  CORBA::Any_var a = ri->get_slot(object_id_slot
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(FtRtecEventChannelAdmin::ObjectId_var());

  return ::get_object_id(a
                         ACE_ENV_ARG_PARAMETER);

}

void
Request_Context_Repository::set_cached_result(
  PortableInterceptor::ServerRequestInfo_ptr ri,
  const CORBA::Any& result
  ACE_ENV_ARG_DECL)
{
  ri->set_slot(cached_result_slot,
               result ACE_ENV_ARG_PARAMETER);
}

CORBA::Any_ptr
Request_Context_Repository::get_cached_result(ACE_ENV_SINGLE_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  CORBA::Any_var a = pic->get_slot(cached_result_slot
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  return a._retn();
}

bool Request_Context_Repository::is_executed_request()
{
  ACE_TRY_NEW_ENV {
    CORBA::Any_var any = get_cached_result(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    CORBA::TypeCode_var type = any->type();
    return type->kind() != CORBA::tk_null;
  }
  ACE_CATCHALL {
  }
  ACE_ENDTRY;
  return false;
}

void
Request_Context_Repository::set_sequence_number(
  PortableInterceptor::ServerRequestInfo_ptr ri,
  FTRT::SequenceNumber seq_num
  ACE_ENV_ARG_DECL)
{
  CORBA::Any a;

  a <<= seq_num;

  ri->set_slot(seq_num_slot, a ACE_ENV_ARG_PARAMETER);
}

void
Request_Context_Repository::set_sequence_number(
  FTRT::SequenceNumber seq_num
  ACE_ENV_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Any a;

  a <<= seq_num;

  pic->set_slot(seq_num_slot, a ACE_ENV_ARG_PARAMETER);
}


FTRT::SequenceNumber
Request_Context_Repository::get_sequence_number(ACE_ENV_SINGLE_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  CORBA::Any_var a = pic->get_slot(seq_num_slot ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  FTRT::SequenceNumber result  = 0;
  *a >>= result;
  return result;
}

FTRT::SequenceNumber
Request_Context_Repository::get_sequence_number(
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
{
  CORBA::Any_var a = ri->get_slot(seq_num_slot ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  FTRT::SequenceNumber result  = 0;
  *a >>= result;
  return result;
}

void
Request_Context_Repository::set_ft_request_service_context(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    IOP::ServiceContext_var service_context
    ACE_ENV_ARG_DECL)
{
  CORBA::Any a;
  a <<= service_context.in();
  ri->set_slot(ft_request_service_context_slot,a
               ACE_ENV_ARG_PARAMETER);
}

CORBA::Any_var
Request_Context_Repository::get_ft_request_service_context(
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
{
  return ri->get_slot(ft_request_service_context_slot
                   ACE_ENV_ARG_PARAMETER);
}

void
Request_Context_Repository::set_transaction_depth(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    FTRT::TransactionDepth depth
    ACE_ENV_ARG_DECL)
{
  CORBA::Any a;
  a <<= depth;
  ri->set_slot(transaction_depth_slot,a
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Request_Context_Repository::set_transaction_depth(
    FTRT::TransactionDepth depth
    ACE_ENV_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
      resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Any a;
  a <<= depth;
  pic->set_slot(transaction_depth_slot,a
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


FTRT::TransactionDepth
Request_Context_Repository::get_transaction_depth(
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
{
  CORBA::Any_var a = ri->get_slot(transaction_depth_slot
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  FTRT::TransactionDepth result=0;
  a >>= result;
  return result;

}

FTRT::TransactionDepth
Request_Context_Repository::get_transaction_depth(
    ACE_ENV_SINGLE_ARG_DECL)
{
  PortableInterceptor::Current_var pic =
      resolve_init<PortableInterceptor::Current>(orb, "PICurrent"
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  CORBA::Any_var a = pic->get_slot(transaction_depth_slot
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  FTRT::TransactionDepth result=0;
  a >>= result;
  return result;

}
