// -*- C++ -*-
// $Id$

#include "tao/PortableServer/Acceptor_Filter_Factory.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Acceptor_Filter_Factory::~TAO_Acceptor_Filter_Factory(void)
{
}

TAO_Acceptor_Filter*
TAO_Acceptor_Filter_Factory::create_object (TAO_POA_Manager& )
{
  TAO_Acceptor_Filter *filter = 0;

  ACE_NEW_RETURN (filter,
                  TAO_Default_Acceptor_Filter (),
                  0);

  return filter;
}


int
TAO_Acceptor_Filter_Factory::initialize (void)
{
  return ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Acceptor_Filter_Factory);
}


ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_Acceptor_Filter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Acceptor_Filter_Factory,
                       ACE_TEXT ("TAO_Acceptor_Filter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Acceptor_Filter_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */


