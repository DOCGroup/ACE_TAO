// $Id$

#include "tao/Default_Collocation_Resolver.h"

ACE_RCSID (tao,
           Default_Collocation_Resolver,
           "$Id$")

#include "tao/Object.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_Default_Collocation_Resolver::is_collocated (CORBA::Object_ptr object
                                                 ACE_ENV_ARG_DECL_NOT_USED) const
{
  return object->_is_collocated ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_Default_Collocation_Resolver,
                       ACE_TEXT ("Default_Collocation_Resolver"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Collocation_Resolver),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Collocation_Resolver)
