// $Id$

#include "IdAssignmentStrategyFactoryImpl.h"
#include "IdAssignmentStrategy.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdAssignmentStrategyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentStrategyFactoryImpl::~IdAssignmentStrategyFactoryImpl (void)
    {
    }
    IdAssignmentStrategy*
    IdAssignmentStrategyFactoryImpl::create (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      IdAssignmentStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::SYSTEM_ID :
        {
          strategy =
            ACE_Dynamic_Service<IdAssignmentStrategy>::instance ("IdAssignmentStrategySystem");

          if (strategy == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic IdAssignmentStrategy Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_IdAssignmentStrategySystem()"));

              strategy =
                ACE_Dynamic_Service<IdAssignmentStrategy>::instance ("IdAssignmentStrategySystem");
            }
          break;
        }
        case ::PortableServer::USER_ID :
        {
          strategy =
            ACE_Dynamic_Service<IdAssignmentStrategy>::instance ("IdAssignmentStrategyUser");

          if (strategy == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic IdAssignmentStrategy Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_IdAssignmentStrategyUser()"));

              strategy =
                ACE_Dynamic_Service<IdAssignmentStrategy>::instance ("IdAssignmentStrategyUser");
            }
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentStrategyFactoryImpl,
        ACE_TEXT ("IdAssignmentStrategyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentStrategyFactoryImpl)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<IdAssignmentStrategyFactoryImpl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<IdAssignmentStrategyFactoryImpl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
