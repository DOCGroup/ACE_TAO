#include "PG_Group_Guard.h"

#include "PG_GenericFactory.h"
#include "PG_ObjectGroupManager.h"


ACE_RCSID (PortableGroup,
           PG_Group_Guard,
           "$Id$")


TAO_PG_Group_Guard::TAO_PG_Group_Guard (
  TAO_PG_GenericFactory & generic_factory,
  TAO_PG_Factory_Set & factory_set,
  TAO_PG_ObjectGroupManager & group_manager,
  const PortableServer::ObjectId & oid)
  : generic_factory_ (generic_factory),
    factory_set_ (factory_set),
    group_manager_ (group_manager),
    oid_ (oid),
    released_ (0)
{
}

TAO_PG_Group_Guard::~TAO_PG_Group_Guard (void)
{
  if (!this->released_)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          this->generic_factory_.delete_object_i (this->factory_set_,
                                                  1  // Ignore exceptions
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // This should never throw an exception if this Guard is
          // used properly.
          this->group_manager_.destroy_object_group (this->oid_
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore all exceptions.
        }
      ACE_ENDTRY;
    }
}

void
TAO_PG_Group_Guard::release (void)
{
  this->released_ = 1;
}

