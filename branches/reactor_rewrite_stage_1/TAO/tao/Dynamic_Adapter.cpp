// @(#) $Id$

#include "tao/Dynamic_Adapter.h"
#include "tao/ORB_Core.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Dynamic_Adapter, "$Id$")

TAO_Dynamic_Adapter::~TAO_Dynamic_Adapter (void)
{
}

// Methods to be overridden.

void
TAO_Dynamic_Adapter::create_request (
    CORBA::Object_ptr /* obj */,
    CORBA::ORB_ptr /* orb */,
    const char * /* operation */,
    CORBA::NVList_ptr /* arg_list */,
    CORBA::NamedValue_ptr /* result */,
    CORBA::ExceptionList_ptr /* exceptions */,
    CORBA::Request_ptr & /* request */,
    CORBA::Flags /* req_flags */
    TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */
  )
{
}

CORBA::Request_ptr
TAO_Dynamic_Adapter::request (CORBA::Object_ptr /* obj */,
                              CORBA::ORB_ptr /* orb */,
                              const char * /* op */
                              TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */)
{
  return 0;
}

CORBA::Boolean
TAO_Dynamic_Adapter::context_is_nil (CORBA::Context_ptr)
{
  return 0;
}

CORBA::Boolean
TAO_Dynamic_Adapter::request_is_nil (CORBA::Request_ptr)
{
  return 0;
}

CORBA::Boolean
TAO_Dynamic_Adapter::server_request_is_nil (CORBA::ServerRequest_ptr)
{
  return 0;
}

void
TAO_Dynamic_Adapter::context_release (CORBA::Context_ptr)
{
}

void
TAO_Dynamic_Adapter::request_release (CORBA::Request_ptr)
{
}

void
TAO_Dynamic_Adapter::server_request_release (CORBA::ServerRequest_ptr)
{
}

void
TAO_Dynamic_Adapter::create_exception_list (CORBA::ExceptionList_ptr &
                                            TAO_ENV_ARG_DECL_NOT_USED)
{
}

// ***********************************************************************

// Implementation of the CORBA::is_nil and CORBA::release methods for
// Context, Request, and ServerRequest.

CORBA::Boolean
CORBA::is_nil (CORBA::Context_ptr ctx)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
              TAO_ORB_Core::dynamic_adapter_name ()
      );

  return dynamic_adapter->context_is_nil (ctx);
}

CORBA::Boolean
CORBA::is_nil (CORBA::Request_ptr req)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  return dynamic_adapter->request_is_nil (req);
}

CORBA::Boolean
CORBA::is_nil (CORBA::ServerRequest_ptr req)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  return dynamic_adapter->server_request_is_nil (req);
}

void
CORBA::release (CORBA::Context_ptr ctx)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->context_release (ctx);
}

void
CORBA::release (CORBA::Request_ptr req)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->request_release (req);
}

void
CORBA::release (CORBA::ServerRequest_ptr req)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->server_request_release (req);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Dynamic_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Dynamic_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


