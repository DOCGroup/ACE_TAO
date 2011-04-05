// $Id$

#include "tao/PortableServer/IdUniquenessStrategyUnique.h"
#include "tao/PortableServer/Root_POA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategyUnique::IdUniquenessStrategyUnique (void) :
      poa_ (0)
    {
    }

    void
    IdUniquenessStrategyUnique::strategy_init (
      TAO_Root_POA *poa
      )
    {
      poa_ = poa;
    }

    void
    IdUniquenessStrategyUnique::strategy_cleanup (
      void)
    {
      poa_ = 0;
    }

    bool
    IdUniquenessStrategyUnique::is_servant_activation_allowed (
      PortableServer::Servant servant,
      bool &wait_occurred_restart_call)
    {
      // If the specified servant is already in the Active Object Map, the
      // ServantAlreadyActive exception is raised.
      int const result =
        this->poa_->is_servant_active (servant,
                                       wait_occurred_restart_call);

      return (!(result || wait_occurred_restart_call));
    }

    bool
    IdUniquenessStrategyUnique::allow_multiple_activations (void) const
    {
      return false;
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessStrategyUnique::type() const
    {
      return ::PortableServer::UNIQUE_ID;
    }

  }
}



ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  IdUniquenessStrategyUnique,
  TAO::Portable_Server::IdUniquenessStrategyUnique)

ACE_STATIC_SVC_DEFINE (
  IdUniquenessStrategyUnique,
  ACE_TEXT ("IdUniquenessStrategyUnique"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (IdUniquenessStrategyUnique),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL

