// $Id$

#include "orbsvcs/LoadBalancing/LB_ObjectReferenceFactory.h"
#include "tao/debug.h"
#include "ace/SString.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// The number of different object groups to support.
#ifndef TAO_LB_ORF_GROUP_TABLE_SIZE
const size_t TAO_LB_ORF_GROUP_TABLE_SIZE = 16;
#endif  /* TAO_LB_ORF_GROUP_TABLE_SIZE */

TAO_LB_ObjectReferenceFactory::TAO_LB_ObjectReferenceFactory (
  PortableInterceptor::ObjectReferenceFactory * old_orf,
  const CORBA::StringSeq & object_groups,
  const CORBA::StringSeq & repository_ids,
  const char * location,
  CORBA::ORB_ptr orb,
  CosLoadBalancing::LoadManager_ptr lm)
  : old_orf_ (old_orf),
    object_groups_ (object_groups),
    repository_ids_ (repository_ids),
    location_ (1),
    table_ (TAO_LB_ORF_GROUP_TABLE_SIZE),
    fcids_ (),
    orb_ (CORBA::ORB::_duplicate (orb)),
    lm_ (CosLoadBalancing::LoadManager::_duplicate (lm)),
    registered_members_ (0)
{
  // Claim ownership of the old ObjectReferenceFactory.
  CORBA::add_ref (old_orf);

  this->location_.length (1);
  this->location_[0].id = CORBA::string_dup (location);

  CORBA::ULong const len = repository_ids.length ();
  ACE_NEW (this->registered_members_,
           CORBA::Boolean[len]);

  ACE_ASSERT (this->registered_members_ != 0);
  ACE_OS::memset (this->registered_members_,
                  0,
                  len * sizeof (CORBA::Boolean));
}

TAO_LB_ObjectReferenceFactory::~TAO_LB_ObjectReferenceFactory (void)
{
  // No need to call CORBA::remove_ref() on this->old_orf_.  It is a
  // "_var" object, meaning that will be done automatically.


  if (!CORBA::is_nil (this->lm_.in ()))
    {
      const CORBA::ULong len = this->fcids_.size ();
      for (CORBA::ULong i = 0; i < len; ++i)
        {
          try
            {
              // Clean up all object groups we created.
              this->lm_->delete_object (this->fcids_[i].in ());
            }
          catch (const CORBA::Exception&)
            {
              // Ignore all exceptions.
            }
        }
    }

  // @todo De-register LoadAlert objects.
  // @todo De-register object group members.

  delete [] this->registered_members_;
}

CORBA::Object_ptr
TAO_LB_ObjectReferenceFactory::make_object (
    const char * repository_id,
    const PortableInterceptor::ObjectId & id)
{
  if (repository_id == 0)
    throw CORBA::BAD_PARAM ();

  CORBA::Object_var obj =
    this->old_orf_->make_object (repository_id,
                                 id);

  PortableGroup::ObjectGroup_var object_group;

  CORBA::ULong index = 0;

  CORBA::Boolean const found_group =
    this->find_object_group (repository_id,
                             index,
                             object_group.out ());

  if (found_group)
    {
      // Be careful not to attempt duplicate registrations on
      // subsequent object reference creation calls.
      if (!this->registered_members_[index])
        {
          try
            {
              object_group =
                this->lm_->add_member (object_group.in (),
                                       this->location_,
                                       obj.in ());
            }
          catch (const PortableGroup::ObjectGroupNotFound& ex)
            {
              if (TAO_debug_level > 0)
                ex._tao_print_exception (
                  "TAO_LB_ObjectReferenceFactory::""make_object");

              throw CORBA::BAD_PARAM ();
            }
          catch (const PortableGroup::MemberAlreadyPresent& ex)
            {
              if (TAO_debug_level > 0)
                ex._tao_print_exception (
                  "TAO_LB_ObjectReferenceFactory::""make_object");

              throw CORBA::BAD_INV_ORDER ();

            }
          catch (const PortableGroup::ObjectNotAdded& ex)
            {
              if (TAO_debug_level > 0)
                ex._tao_print_exception (
                  "TAO_LB_ObjectReferenceFactory::""make_object");

              throw CORBA::UNKNOWN ();
            }

          this->registered_members_[index] = 1;
        }

      // Return the object group reference instead.
      return object_group._retn ();
    }

  // Not a load managed object.  Simply return the object's actual
  // object reference.
  return obj._retn ();
}

CORBA::Boolean
TAO_LB_ObjectReferenceFactory::find_object_group (
  const char * repository_id,
  CORBA::ULong & index,
  PortableGroup::ObjectGroup_out object_group)
{
  if (!this->load_managed_object (repository_id, index))
    return false;

  PortableGroup::ObjectGroup_var group;
  if (this->table_.find (repository_id, group) != 0)
    {
      if (ACE_OS::strcasecmp (this->object_groups_[index],
                              "CREATE") == 0)
        {
          PortableGroup::Criteria criteria (1);
          criteria.length (1);

          PortableGroup::Property & property = criteria[0];
          property.nam.length (1);

          property.nam[0].id =
            CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

          // Configure for application-controlled membership.
          PortableGroup::MembershipStyleValue msv =
            PortableGroup::MEMB_APP_CTRL;
          property.val <<= msv;

          PortableGroup::GenericFactory::FactoryCreationId_var fcid;

          group =
            this->lm_->create_object (repository_id,
                                      criteria,
                                      fcid.out ());

          CORBA::ULong const len = this->fcids_.size ();
          this->fcids_.size (len + 1); // Incremental growth.  Yuck!
          this->fcids_[len] = fcid;
        }
      else
        {
          group =
            this->orb_->string_to_object (this->object_groups_[index]);
        }

      if (this->table_.bind (repository_id, group) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO_LB_ObjectReferenceFactory::"
                        "find_object_group - "
                        "Couldn't bind object group reference.\n"));

          throw CORBA::INTERNAL ();
        }

      object_group = group._retn ();
    }

  return 1;
}

CORBA::Boolean
TAO_LB_ObjectReferenceFactory::load_managed_object (const char * repository_id,
                                                    CORBA::ULong & i)
{
  // @todo Make this more efficient.

  CORBA::ULong const len = this->repository_ids_.length ();
  for (i = 0; i < len; ++i)
    if (ACE_OS::strcmp (this->repository_ids_[i], repository_id) == 0)
      return true;

  return false;
}


TAO_END_VERSIONED_NAMESPACE_DECL
