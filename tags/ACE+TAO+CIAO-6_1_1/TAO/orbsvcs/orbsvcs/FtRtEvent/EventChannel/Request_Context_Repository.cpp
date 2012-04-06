// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "../Utils/resolve_init.h"
#include "../Utils/UUID.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/AnyTypeCode/IOPA.h"
#include "ace/TSS_T.h"

namespace {
PortableInterceptor::SlotId object_id_slot;
PortableInterceptor::SlotId cached_result_slot;
PortableInterceptor::SlotId seq_num_slot;
PortableInterceptor::SlotId ft_request_service_context_slot;
PortableInterceptor::SlotId transaction_depth_slot;
CORBA::ORB_ptr orb;
ACE_TSS<FtRtecEventChannelAdmin::ObjectId> oid;
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
Request_Context_Repository::allocate_slots(PortableInterceptor::ORBInitInfo_ptr info)
{
  object_id_slot = info->allocate_slot_id();
  cached_result_slot = info->allocate_slot_id();
  seq_num_slot = info->allocate_slot_id();
  ft_request_service_context_slot = info->allocate_slot_id();
  transaction_depth_slot = info->allocate_slot_id();
}

void
Request_Context_Repository::init(CORBA::ORB_ptr the_orb)
{
  orb = the_orb;
}

void Request_Context_Repository::generate_object_id(
  FtRtecEventChannelAdmin::ObjectId& oid)
{
  oid.length(sizeof(TAO_FtRt::UUID));
  TAO_FtRt::UUID::create(oid.get_buffer());
  set_object_id(oid);
}

void
Request_Context_Repository::set_object_id(
  const FtRtecEventChannelAdmin::ObjectId& object_id)
{
  /*
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any a;
  a <<= object_id;
  pic->set_slot(object_id_slot, a);

  */
  *oid = object_id;
}

FtRtecEventChannelAdmin::ObjectId_var
get_object_id(CORBA::Any_var a)
{
  FtRtecEventChannelAdmin::ObjectId *object_id, *r;
  FtRtecEventChannelAdmin::ObjectId_var result;

  if ((a.in() >>= object_id) ==0)
    throw CORBA::NO_MEMORY();

  ACE_NEW_THROW_EX(r,
                   FtRtecEventChannelAdmin::ObjectId(*object_id),
                   CORBA::NO_MEMORY());

  result = r;
  return result;
}


FtRtecEventChannelAdmin::ObjectId_var
Request_Context_Repository::get_object_id(void)
{
  /*
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any_var a = pic->get_slot(object_id_slot);

  return ::get_object_id(a);
  */
  FtRtecEventChannelAdmin::ObjectId *object_id;
  ACE_NEW_THROW_EX(object_id,
                   FtRtecEventChannelAdmin::ObjectId(*oid),
                   CORBA::NO_MEMORY());
  return FtRtecEventChannelAdmin::ObjectId_var(*object_id);
}

FtRtecEventChannelAdmin::ObjectId_var
Request_Context_Repository::get_object_id(
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::Any_var a = ri->get_slot(object_id_slot);

  return ::get_object_id(a);

}

void
Request_Context_Repository::set_cached_result(
  PortableInterceptor::ServerRequestInfo_ptr ri,
  const CORBA::Any& result)
{
  ri->set_slot(cached_result_slot,
               result);
}

CORBA::Any_ptr
Request_Context_Repository::get_cached_result(void)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any_var a = pic->get_slot(cached_result_slot);
  return a._retn();
}

bool Request_Context_Repository::is_executed_request()
{
  try{
    CORBA::Any_var any = get_cached_result();
    CORBA::TypeCode_var type = any->type();
    CORBA::TCKind const kind = type->kind();
    return kind != CORBA::tk_null;
  }
  catch (...){
  }
  return false;
}

void
Request_Context_Repository::set_sequence_number(
  PortableInterceptor::ServerRequestInfo_ptr ri,
  FTRT::SequenceNumber seq_num)
{
  CORBA::Any a;

  a <<= seq_num;

  ri->set_slot(seq_num_slot, a);
}

void
Request_Context_Repository::set_sequence_number(
  FTRT::SequenceNumber seq_num)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any a;

  a <<= seq_num;

  pic->set_slot(seq_num_slot, a);
}


FTRT::SequenceNumber
Request_Context_Repository::get_sequence_number(void)
{
  PortableInterceptor::Current_var pic =
    resolve_init<PortableInterceptor::Current>(orb, "PICurrent");
  CORBA::Any_var a = pic->get_slot(seq_num_slot);
  FTRT::SequenceNumber result  = 0;
  a >>= result;
  return result;
}

FTRT::SequenceNumber
Request_Context_Repository::get_sequence_number(
      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var a = ri->get_slot(seq_num_slot);
  FTRT::SequenceNumber result  = 0;
  a >>= result;
  return result;
}

void
Request_Context_Repository::set_ft_request_service_context(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    IOP::ServiceContext_var service_context)
{
  CORBA::Any a;
  a <<= service_context.in();
  ri->set_slot(ft_request_service_context_slot,a);
}

CORBA::Any_var
Request_Context_Repository::get_ft_request_service_context(
      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  return ri->get_slot(ft_request_service_context_slot);
}

void
Request_Context_Repository::set_transaction_depth(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    FTRT::TransactionDepth depth)
{
  CORBA::Any a;
  a <<= depth;
  ri->set_slot(transaction_depth_slot,a);
}

void
Request_Context_Repository::set_transaction_depth(
    FTRT::TransactionDepth depth)
{
  PortableInterceptor::Current_var pic =
      resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any a;
  a <<= depth;
  pic->set_slot(transaction_depth_slot,a);
}


FTRT::TransactionDepth
Request_Context_Repository::get_transaction_depth(
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var a = ri->get_slot(transaction_depth_slot);
  FTRT::TransactionDepth result=0;
  a >>= result;
  return result;

}

FTRT::TransactionDepth
Request_Context_Repository::get_transaction_depth()
{
  PortableInterceptor::Current_var pic =
      resolve_init<PortableInterceptor::Current>(orb, "PICurrent");

  CORBA::Any_var a = pic->get_slot(transaction_depth_slot);

  FTRT::TransactionDepth result=0;
  a >>= result;
  return result;

}

TAO_END_VERSIONED_NAMESPACE_DECL
