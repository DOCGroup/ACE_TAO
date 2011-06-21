// $Id$
#include "ace/OS_NS_string.h"
#include "ace/SString.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "orbsvcs/FtRtEvent/EventChannel/FtEventServiceInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "../Utils/Safe_InputCDR.h"
#include "../Utils/Log.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
CachedRequestTable::is_new_request(const ACE_CString& client_id, CORBA::Long retention_id)
{
  TableImpl::ENTRY* entry = 0;
  return table_.find(client_id, entry) !=0 || entry->int_id_.retention_id != retention_id;
}

int
CachedRequestTable::update(const ACE_CString& client_id,
                           CORBA::Long retention_id,
                           const CORBA::Any& result)
{
  CachedRequestInfo info;
  info.retention_id = retention_id;
  TableImpl::ENTRY* entry=0;
  if (table_.bind(client_id, info, entry))
  {
    entry->int_id_.result = result;
    return 0;
  }
  return -1;
}



CORBA::Any
CachedRequestTable::get_result(const ACE_CString& client_id)
{
  TableImpl::ENTRY* entry = 0;
  if (table_.find(client_id, entry)) {
    return entry->int_id_.result;
  }
  return CORBA::Any();
}


void
CachedRequestTable::get_state(FtRtecEventChannelAdmin::CachedOptionResults& state)
{
  state.length(table_.current_size());
  TableImpl::iterator last = table_.end();
  int i = 0;

  for (TableImpl::iterator first = table_.begin();
    first != last;
    ++first)
  {
    TableImpl::ENTRY& entry = *first;
    state[i].client_id = ACE_OS::strdup(entry.ext_id_.c_str());
    state[i].cached_result = entry.int_id_;
    ++i;
  }

}

void
CachedRequestTable::set_state(const FtRtecEventChannelAdmin::CachedOptionResults& state)
{
  for (size_t i = 0; i < state.length(); ++i)
  {
    table_.bind(state[i].client_id.in(), state[i].cached_result);
  }
}

void
retrieve_ft_request_context(
                            PortableInterceptor::ServerRequestInfo_ptr ri,
                            IOP::ServiceContext_var& service_context,
                            FT::FTRequestServiceContext& ft_request_service_context)
{
  service_context = ri->get_request_service_context(IOP::FT_REQUEST);

  const char * buf =
    reinterpret_cast<const char *> (service_context->context_data.get_buffer ());

  Safe_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    throw CORBA::BAD_PARAM ();

  cdr.reset_byte_order (static_cast<int> (byte_order));


  if ((cdr >> ft_request_service_context) == 0)
    throw CORBA::BAD_PARAM ();

}

FTRT::TransactionDepth
get_transaction_depth_context(
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  IOP::ServiceContext_var service_context;
  try{
    service_context = ri->get_request_service_context(FTRT::FT_TRANSACTION_DEPTH);
  }
  catch (const CORBA::BAD_PARAM&)
  {
    ACE_DEBUG((LM_DEBUG, "Received request without transaction depth context\n"));
    return -1;
  }

  const char * buf =
    reinterpret_cast<const char *> (service_context->context_data.get_buffer ());
  Safe_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    throw CORBA::BAD_PARAM ();

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FTRT::TransactionDepth result;
  if ((cdr >> result) == 0)
    throw CORBA::BAD_PARAM ();

  return result;
}

FTRT::SequenceNumber
get_sequence_number_context(
                            PortableInterceptor::ServerRequestInfo_ptr ri)
{
  FTRT::SequenceNumber result;
  IOP::ServiceContext_var service_context;
  service_context = ri->get_request_service_context(FTRT::FT_SEQUENCE_NUMBER);

  const char * buf =
    reinterpret_cast<const char *> (service_context->context_data.get_buffer ());
  Safe_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    throw CORBA::BAD_PARAM ();

  cdr.reset_byte_order (static_cast<int> (byte_order));

  if ((cdr >> result) == 0)
    throw CORBA::BAD_PARAM ();

  return result;
}


namespace {
FtEventServiceInterceptor* interceptor;
}

FtEventServiceInterceptor::FtEventServiceInterceptor ()
{
  interceptor = this;
}

FtEventServiceInterceptor::~FtEventServiceInterceptor ()
{
}

FtEventServiceInterceptor*
FtEventServiceInterceptor::instance()
{
  return interceptor;
}

char *
FtEventServiceInterceptor::name (void)
{
  return CORBA::string_dup ("FtEventServiceInterceptor");
}

void
FtEventServiceInterceptor::destroy (void)
{
}

void
FtEventServiceInterceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}


void
FtEventServiceInterceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();
  if (ACE_OS::strcmp(operation.in(), "push") == 0) {
    TAO_FTRTEC::Log(3, ACE_TEXT ("Received push command\n"));
    return;
  }

  try{
    FT::FTRequestServiceContext ft_request_service_context;
    IOP::ServiceContext_var service_context;
    retrieve_ft_request_context(ri,
      service_context,
      ft_request_service_context);

    bool is_new_request = request_table_.is_new_request(
      ft_request_service_context.client_id.in(),
      ft_request_service_context.retention_id);

    CORBA::Any cached_result;
    if (!is_new_request)  {
      cached_result =
        request_table_.get_result(ft_request_service_context.client_id.in());
    }

    Request_Context_Repository().set_cached_result(ri, cached_result);
    Request_Context_Repository().set_ft_request_service_context(ri, service_context);

    try{

      FTRT::TransactionDepth transaction_depth =
        get_transaction_depth_context(ri);

      Request_Context_Repository().set_transaction_depth(ri, transaction_depth);
    }
    catch (const CORBA::BAD_PARAM&){
    }

    FTRT::SequenceNumber sequence_no =
      get_sequence_number_context(ri);

    Request_Context_Repository().set_sequence_number(ri, sequence_no);
  }
  catch (const CORBA::BAD_PARAM&){
  }
}

void
FtEventServiceInterceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  FT::FTRequestServiceContext ft_request_service_context;
  IOP::ServiceContext_var service_context;

  try{
    retrieve_ft_request_context(ri,
      service_context,
      ft_request_service_context);
  }
  catch (const CORBA::BAD_PARAM&){
    return;
  }

  request_table_.update(ft_request_service_context.client_id.in(),
    ft_request_service_context.retention_id,
    *(ri->result()) );
}

void
FtEventServiceInterceptor::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
FtEventServiceInterceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr)
{
}


void
FtEventServiceInterceptor::get_state(FtRtecEventChannelAdmin::CachedOptionResults& state)
{
  request_table_.get_state(state);
}

void
FtEventServiceInterceptor::set_state(const FtRtecEventChannelAdmin::CachedOptionResults& state)
{
  request_table_.set_state(state);
}

TAO_END_VERSIONED_NAMESPACE_DECL
