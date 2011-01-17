// $Id$

#include "orbsvcs/Log/LogActivator.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
#include "orbsvcs/Log/LogMgr_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LogActivator::TAO_LogActivator (TAO_LogMgr_i &logmgr_i)
  : logmgr_i_ (logmgr_i)
{
}

TAO_LogActivator::~TAO_LogActivator ()
{
}

PortableServer::Servant
TAO_LogActivator::incarnate (const PortableServer::ObjectId& oid,
                             PortableServer::POA_ptr)
{
  CORBA::String_var poa_id = PortableServer::ObjectId_to_string (oid);

  DsLogAdmin::LogId id = ACE_OS::strtoul(poa_id.in (), 0, 0);

  if (!logmgr_i_.exists(id))
    throw CORBA::OBJECT_NOT_EXIST ();

  return logmgr_i_.create_log_servant (id);
}


void
TAO_LogActivator::etherealize (const PortableServer::ObjectId&,
                               PortableServer::POA_ptr,
                               PortableServer::Servant servant,
                               CORBA::Boolean,
                               CORBA::Boolean remaining_activations)
{
  if (!remaining_activations)
   {
     delete servant;
   }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
