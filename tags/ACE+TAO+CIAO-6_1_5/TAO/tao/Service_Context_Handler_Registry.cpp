// $Id$

#include "tao/Service_Context_Handler_Registry.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Service_Context_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Service_Context_Registry::process_service_contexts (
  IOP::ServiceContextList &sc,
  TAO_Transport& transport,
  TAO_ServerRequest *request)
{
  for (CORBA::ULong index = 0;
       index != sc.length ();
       ++index)
  {
    IOP::ServiceContext const & context = sc[index];

    iterator handler_iter = this->registry_.find (context.context_id);

    if (handler_iter != registry_.end ())
      {
        return handler_iter->second->process_service_context (transport, context, request);
      }
  }
  return 0;
}

int
TAO_Service_Context_Registry::generate_service_context (
  TAO_Stub *stub,
  TAO_Transport& transport,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &spec,
  TAO_OutputCDR &msg)
{
  int result = 0;
  for (Table::iterator x = this->registry_.begin ();
       x != this->registry_.end ();
       ++x)
    {
      if ((*x).second->generate_service_context (
        stub, transport, opdetails, spec, msg) == -1)
        {
          result = -1;
          }
    }

  return result;
}

TAO_Service_Context_Registry::~TAO_Service_Context_Registry (void)
{
  for (Table::iterator x = this->registry_.begin ();
       x != this->registry_.end ();
       ++x)
    {
      delete (*x).second;
    }
}

int
TAO_Service_Context_Registry::bind (
  IOP::ServiceId id,
  TAO_Service_Context_Handler* handler)
{
  value_type const value =
    std::make_pair (key_type (id), data_type (handler));

  std::pair<iterator, bool> result = this->registry_.insert (value);

  return (result.second ? 0 : 1);
}

TAO_Service_Context_Handler*
TAO_Service_Context_Registry::operator[] (IOP::ServiceId id)
{
  return registry_[id];
}

TAO_END_VERSIONED_NAMESPACE_DECL
