// $Id$

#include "orbsvcs/PortableGroup/PG_Group_Guard.h"

#include "orbsvcs/PortableGroup/PG_GenericFactory.h"
#include "orbsvcs/PortableGroup/PG_ObjectGroupManager.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_Group_Guard::TAO_PG_Group_Guard (
  TAO_PG_GenericFactory & generic_factory,
  TAO_PG_Factory_Set & factory_set,
  TAO_PG_ObjectGroupManager & group_manager,
  const PortableServer::ObjectId & oid)
  : generic_factory_ (generic_factory),
    factory_set_ (factory_set),
    group_manager_ (group_manager),
    oid_ (oid),
    released_ (false)
{
}

TAO_PG_Group_Guard::~TAO_PG_Group_Guard (void)
{
  if (!this->released_)
    {
      try
        {
          this->generic_factory_.delete_object_i (this->factory_set_,
                                                  1  // Ignore exceptions
                                                  );

          // This should never throw an exception if this Guard is
          // used properly.
          this->group_manager_.destroy_object_group (this->oid_);
        }
      catch (const CORBA::Exception&)
        {
          // Ignore all exceptions.
        }
    }
}

void
TAO_PG_Group_Guard::release (void)
{
  this->released_ = true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
