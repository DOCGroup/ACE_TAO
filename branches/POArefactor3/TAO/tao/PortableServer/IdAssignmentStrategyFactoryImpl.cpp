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
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("Unable to get ")
                        ACE_TEXT ("IdAssignmentStrategySystem")));
          break;
        }
        case ::PortableServer::USER_ID :
        {
          strategy =
            ACE_Dynamic_Service<IdAssignmentStrategy>::instance ("IdAssignmentStrategyUser");

          if (strategy == 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("Unable to get ")
                        ACE_TEXT ("IdAssignmentStrategyUser")));
          break;
        }
      }

      return strategy;
    }

    void
    IdAssignmentStrategyFactoryImpl::destroy (
      IdAssignmentStrategy * /*strategy*/
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      // Noop because both types are singletons
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
