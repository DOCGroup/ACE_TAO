// -*- C++ -*-
//
// $Id$

#include "orbsvcs/LoadBalancingS.h"


class HashReplicaFactory
  : public virtual POA_TAO_LoadBalancing::GenericFactory
{
public:  

  virtual CORBA::Object_ptr create_object (
      const char *type_id,
      const TAO_LoadBalancing::Criteria &the_criteria,
      TAO_LoadBalancing::GenericFactory::FactoryCreationId_out
        factory_creation_id)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::NoFactory,
                     TAO_LoadBalancing::ObjectNotCreated,
                     TAO_LoadBalancing::InvalidCriteria,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::CannotMeetCriteria));

  virtual void delete_object (
     const TAO_LoadBalancing::GenericFactory::FactoryCreationId
       &factory_creation_id)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectNotFound));

};
