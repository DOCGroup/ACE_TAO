#include "LB_ObjectReferenceFactory.h"

/**
 * @todo Change this to "ace/Tokenizer.h" once "ace/Tokenizer.h" is
 *       committed.
 */
#include "ace/SString.h"

ACE_RCSID (LoadBalancing,
           LB_ObjectReferenceFactory,
           "$Id$")


TAO_LB_ObjectReferenceFactory::TAO_LB_ObjectReferenceFactory (
  PortableInterceptor::ObjectReferenceFactory * old_orf,
  const char * repository_ids)
  : old_orf_ (old_orf),
    repository_ids_ (repository_ids)
{
  // Claim ownership of the old ObjectReferenceFactory.
  CORBA::add_ref (old_orf);
}

TAO_LB_ObjectReferenceFactory::~TAO_LB_ObjectReferenceFactory (void)
{
  // No need to call CORBA::remove_ref() on this->old_orf_.  It is a
  // "_var" object, meaning that will be done automatically.
}

CORBA::Object_ptr
TAO_LB_ObjectReferenceFactory::make_object (
    const char * repository_id,
    const PortableInterceptor::ObjectId & id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj =
    this->old_orf_->make_object (repository_id,
                                 id
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableGroup::ObjectGroup_var object_group;
  if (this->find_object_group (repository_id, object_group.out ()))
    {
      ACE_TRY
        {
          this->lm_->add_member (object_group.in (),
                                 this->location_,
                                 obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Return the object group reference instead.
          return object_group._retn ();
        }
      ACE_CATCH (PortableGroup::ObjectGroupNotFound, ex)
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ex,
                                 "TAO_LB_ObjectReferenceFactory::make_object"
                                 " - LoadManager::add_member()\n");

          ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                            CORBA::Object::_nil ());
        }
      ACE_CATCH (PortableGroup::MemberAlreadyPresent, ex)
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ex,
                                 "TAO_LB_ObjectReferenceFactory::make_object"
                                 " - LoadManager::add_member()\n");

          ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (),
                            CORBA::Object::_nil ());

        }
      ACE_CATCH (PortableGroup::ObjectNotAdded, ex)
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ex,
                                 "TAO_LB_ObjectReferenceFactory::make_object"
                                 " - LoadManager::add_member()\n");

          ACE_THROW_RETURN (CORBA::UNKNOWN (),
                            CORBA::Object::_nil ());
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }

  // Not a load managed object.  Simply return the object's actual
  // object reference.
  return obj._retn ();
}

CORBA::Boolean
TAO_LB_ObjectReferenceFactory::find_object_group (
  const char * repository_id,
  PortableGroup::ObjectGroup_out object_group)
{
  this->load_managed_object (repository_id)
  PortableGroup::ObjectGroup_var;
  if (this->load_balanced_object (repository_id)
      && this->table_.find (repository_id, object_group) == 0)
    return 1;
  else
    return 0;
}

CORBA::Boolean
TAO_LB_ObjectReferenceFactory::load_managed_object (const char * repository_id)
{
  CORBA::String_var ids = CORBA::string_dup (this->repository_ids_);

  if (str == this->repository_ids_
      && str + len
}
