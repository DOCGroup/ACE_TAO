#include "LB_ObjectReferenceFactory.h"


ACE_RCSID (LoadBalancing,
           LB_ObjectReferenceFactory,
           "$Id$")


#include "tao/debug.h"


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

  const CORBA::ULong len = repository_ids.length ();
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

  ACE_DECLARE_NEW_CORBA_ENV;

  if (!CORBA::is_nil (this->lm_.in ()))
    {
      const CORBA::ULong len = this->fcids_.size ();
      for (CORBA::ULong i = 0; i < len; ++i)
        {
          ACE_TRY
            {
              // Clean up all object groups we created.
              this->lm_->delete_object (this->fcids_[i].in ()
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

  // @todo De-register LoadAlert objects.
  // @todo De-register object group members.

  delete [] this->registered_members_;
}

CORBA::Object_ptr
TAO_LB_ObjectReferenceFactory::make_object (
    const char * repository_id,
    const PortableInterceptor::ObjectId & id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (repository_id == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), CORBA::Object::_nil ());

  CORBA::Object_var obj =
    this->old_orf_->make_object (repository_id,
                                 id
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableGroup::ObjectGroup_var object_group;

  CORBA::ULong index = 0;

  const CORBA::Boolean found_group =
    this->find_object_group (repository_id,
                             index,
                             object_group.out ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (found_group)
    {
      // Be careful not to attempt duplicate registrations on
      // subsequent object reference creation calls.
      if (!this->registered_members_[index])
        {
          ACE_TRY
            {
              object_group =
                this->lm_->add_member (object_group.in (),
                                       this->location_,
                                       obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCH (PortableGroup::ObjectGroupNotFound, ex)
            {
              if (TAO_debug_level > 0)
                ACE_PRINT_EXCEPTION (ex,
                                     "TAO_LB_ObjectReferenceFactory::"
                                     "make_object");

              ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                                CORBA::Object::_nil ());
            }
          ACE_CATCH (PortableGroup::MemberAlreadyPresent, ex)
            {
              if (TAO_debug_level > 0)
                ACE_PRINT_EXCEPTION (ex,
                                     "TAO_LB_ObjectReferenceFactory::"
                                     "make_object");

              ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (),
                                CORBA::Object::_nil ());

            }
          ACE_CATCH (PortableGroup::ObjectNotAdded, ex)
            {
              if (TAO_debug_level > 0)
                ACE_PRINT_EXCEPTION (ex,
                                     "TAO_LB_ObjectReferenceFactory::"
                                     "make_object");

              ACE_THROW_RETURN (CORBA::UNKNOWN (),
                                CORBA::Object::_nil ());
            }
          ACE_ENDTRY;
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

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
  PortableGroup::ObjectGroup_out object_group
  ACE_ENV_ARG_DECL)
{
  if (this->load_managed_object (repository_id, index))
    {
      PortableGroup::ObjectGroup_var group;
      if (this->table_.find (repository_id, group) != 0)
        {
          if (ACE_OS::strcasecmp (this->object_groups_[index].in (),
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
                                          fcid.out ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (0);

              const CORBA::ULong len = this->fcids_.size ();
              this->fcids_.size (len + 1); // Incremental growth.  Yuck!
              this->fcids_[len] = fcid;
            }
          else
            {
              group =
                this->orb_->string_to_object (this->object_groups_[index].in ()
                                              ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (0);
            }

          if (this->table_.bind (repository_id, group) != 0)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO_LB_ObjectReferenceFactory::"
                            "find_object_group - "
                            "Couldn't bind object group reference.\n"));

              ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
            }

         object_group = group.out ();
       }

      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
TAO_LB_ObjectReferenceFactory::load_managed_object (const char * repository_id,
                                                    CORBA::ULong & i)
{
  // @todo Make this more efficient.

  const CORBA::ULong len = this->repository_ids_.length ();
  for (i = 0; i < len; ++i)
    if (ACE_OS::strcmp (this->repository_ids_[i].in (), repository_id) == 0)
      return 1;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, PortableGroup::ObjectGroup_var>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Array_Base<PortableGroup::GenericFactory::FactoryCreationId_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, PortableGroup::ObjectGroup_var>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, PortableGroup::ObjectGroup_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Array_Base<PortableGroup::GenericFactory::FactoryCreationId_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
