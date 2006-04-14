// $Id$

#include "POA_Current_Factory.h"
#include "POA_Current.h"
#include "tao/ORB_Core.h"

TAO_POA_Current_Factory::TAO_POA_Current_Factory (void)
{
}

CORBA::Object_ptr
TAO_POA_Current_Factory::create_object (CORBA::ORB_ptr,
                                        int,
                                        ACE_TCHAR * []
                                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::Portable_Server::POA_Current* adapter = 0;
  ACE_NEW_RETURN (adapter,
                  TAO::Portable_Server::POA_Current (),
                  0);
  return adapter;
}

ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_POA_Current_Factory)
ACE_STATIC_SVC_DEFINE (TAO_POA_Current_Factory,
                       ACE_TEXT ("TAO_POA_Current_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_POA_Current_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

