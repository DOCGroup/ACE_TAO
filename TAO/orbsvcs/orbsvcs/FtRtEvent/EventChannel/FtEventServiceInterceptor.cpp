// $Id$

#include "orbsvcs/FT_CORBA_ORBC.h"
#include "FtEventServiceInterceptor.h"
#include "Request_Context_Repository.h"

ACE_RCSID (FTRT_EventChannel,
           FtEventServiceInterceptor,
           "$Id$")

bool
CachedRequestTable::is_new_request(const ACE_CString& client_id, CORBA::Long retention_id)
{
  TableImpl::ENTRY* entry;
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
  TableImpl::ENTRY* entry;
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
    state[i].client_id = strdup(entry.ext_id_.c_str());
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
                            FT::FTRequestServiceContext& ft_request_service_context
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  service_context = ri->get_request_service_context(IOP::FT_REQUEST ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char * buf =
    ACE_reinterpret_cast (const char *,
    service_context->context_data.get_buffer ());

  TAO_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  cdr.reset_byte_order (ACE_static_cast (int,byte_order));


  if ((cdr >> ft_request_service_context) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  ACE_DEBUG((LM_DEBUG, "client_id = %s, retention_id = %d \n",
    ft_request_service_context.client_id.in(),
    ft_request_service_context.retention_id));
}

FTRT::TransactionDepth
get_transaction_depth_context(
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
{
  IOP::ServiceContext_var service_context;
  ACE_TRY {
    service_context = ri->get_request_service_context(FTRT::FT_TRANSACTION_DEPTH
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(-1);
  }
  ACE_CATCH  (CORBA::BAD_PARAM, ex)
  {
    ACE_DEBUG((LM_DEBUG, "Received request without transaction depth context\n"));
    return -1;
  }
  ACE_ENDTRY;

  const char * buf =
    ACE_reinterpret_cast (const char *,
    service_context->context_data.get_buffer ());
  TAO_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  cdr.reset_byte_order (ACE_static_cast (int,byte_order));

  FTRT::TransactionDepth result;
  if ((cdr >> result) == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  ACE_DEBUG((LM_DEBUG, "transaction depth %d\n", result));
  return result;
}

FTRT::SequenceNumber
get_sequence_number_context(
                            PortableInterceptor::ServerRequestInfo_ptr ri
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  FTRT::SequenceNumber result;
  IOP::ServiceContext_var service_context;
  service_context = ri->get_request_service_context(FTRT::FT_SEQUENCE_NUMBER
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char * buf =
    ACE_reinterpret_cast (const char *,
    service_context->context_data.get_buffer ());
  TAO_InputCDR cdr (buf,
    service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  cdr.reset_byte_order (ACE_static_cast (int,byte_order));

  if ((cdr >> result) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  ACE_DEBUG((LM_DEBUG, "sequence number %d\n", result));
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
FtEventServiceInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("FtEventServiceInterceptor");
}

void
FtEventServiceInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FtEventServiceInterceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
  PortableInterceptor::ForwardRequest))
{
  ACE_TRY_EX(block1) {
    FT::FTRequestServiceContext ft_request_service_context;
    IOP::ServiceContext_var service_context;
    retrieve_ft_request_context(ri,
      service_context,
      ft_request_service_context
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);

    bool is_new_request = request_table_.is_new_request(
      ft_request_service_context.client_id.in(),
      ft_request_service_context.retention_id);

    ACE_DEBUG((LM_DEBUG,"is_new_request %d\n", is_new_request));

    CORBA::Any cached_result;
    if (!is_new_request)  {
      cached_result =
        request_table_.get_result(ft_request_service_context.client_id.in());
    }

    Request_Context_Repository().set_cached_result(ri, cached_result
                                           ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);
    Request_Context_Repository().set_ft_request_service_context(ri, service_context
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);

    ACE_TRY_EX(block2) {

      FTRT::TransactionDepth transaction_depth =
        get_transaction_depth_context(ri
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(block2);

      Request_Context_Repository().set_transaction_depth(ri, transaction_depth
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(block2);
    }
    ACE_CATCH  (CORBA::BAD_PARAM, ex)   {
    }
    ACE_ENDTRY;

    FTRT::SequenceNumber sequence_no =
      get_sequence_number_context(ri ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);

    Request_Context_Repository().set_sequence_number(ri, sequence_no
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);
  }
  ACE_CATCH  (CORBA::BAD_PARAM, ex)   {
  }
  ACE_ENDTRY;
  return;
}




void
FtEventServiceInterceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                            ACE_ENV_ARG_DECL_NOT_USED)
                                            ACE_THROW_SPEC ((CORBA::SystemException,
                                            PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG, "receive request %s\n", operation.in()));
}

void
FtEventServiceInterceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                                       ACE_ENV_ARG_DECL_NOT_USED)
                                       ACE_THROW_SPEC ((CORBA::SystemException))
{
  FT::FTRequestServiceContext ft_request_service_context;
  IOP::ServiceContext_var service_context;

  ACE_TRY {
    retrieve_ft_request_context(ri,
      service_context,
      ft_request_service_context
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
  ACE_CATCH (CORBA::BAD_PARAM, ex) {
    return;
  }
  ACE_ENDTRY;

  request_table_.update(ft_request_service_context.client_id.in(),
    ft_request_service_context.retention_id,
    *(ri->result()) );
}

void
FtEventServiceInterceptor::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
  PortableInterceptor::ForwardRequest))
{
#ifndef NDEBUG
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG, "%s return Exception\n", operation.in()));
#endif
}

void
FtEventServiceInterceptor::send_other (
                                       PortableInterceptor::ServerRequestInfo_ptr
                                       ACE_ENV_ARG_DECL_NOT_USED)
                                       ACE_THROW_SPEC ((CORBA::SystemException,
                                       PortableInterceptor::ForwardRequest))
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




