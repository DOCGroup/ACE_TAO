#include "PG_GenericFactory.h"
#include "PG_MemberInfo.h"
#include "PG_ObjectGroupManager.h"
#include "PG_PropertyManager.h"
#include "PG_Property_Utils.h"
#include "PG_conf.h"
#include "orbsvcs/PortableGroupC.h"
#include "PG_Properties_Encoder.h"
#include "PG_Properties_Decoder.h"


ACE_RCSID (PortableGroup,
           PG_GenericFactory,
           "$Id$")


TAO_PG_GenericFactory::TAO_PG_GenericFactory (
  TAO_PG_ObjectGroupManager & object_group_manager,
  TAO_PG_PropertyManager & property_manager)
  : poa_ (),
//    next_group_id_(0),
    object_group_manager_ (object_group_manager),
    property_manager_ (property_manager),
    factory_map_ (TAO_PG_MAX_OBJECT_GROUPS),
    next_fcid_ (0),
    lock_ ()
{
  this->object_group_manager_.generic_factory (this);
  this->domain_id_ = "default-domain";

}

TAO_PG_GenericFactory::~TAO_PG_GenericFactory (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  TAO_PG_Factory_Map::iterator end = this->factory_map_.end ();
  for (TAO_PG_Factory_Map::iterator i = this->factory_map_.begin ();
       i != end;
       ++i)
    {
      TAO_PG_Factory_Set & factory_set = (*i).int_id_;

      ACE_TRY
        {
          this->delete_object_i (factory_set,
                                 1 /* Ignore exceptions */
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore all exceptions.
        }
      ACE_ENDTRY;
    }

  (void) this->factory_map_.close ();
}

CORBA::Object_ptr
TAO_PG_GenericFactory::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{

  PortableGroup::ObjectGroup_var object_group = PortableGroup::ObjectGroup::_nil();


  PortableGroup::Properties_var properties =
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableGroup::MembershipStyleValue membership_style =
    TAO_PG_MEMBERSHIP_STYLE;
  PortableGroup::FactoriesValue factory_infos(0);

  PortableGroup::InitialNumberMembersValue initial_number_members =
    TAO_PG_INITIAL_NUMBER_MEMBERS;
  PortableGroup::MinimumNumberMembersValue minimum_number_members =
    TAO_PG_MINIMUM_NUMBER_MEMBERS;

  // Make sure the criteria for the object group being created are
  // valid.
  this->process_criteria (type_id,
                          the_criteria,
                          membership_style,
                          factory_infos,
                          minimum_number_members,
                          minimum_number_members
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  PortableGroup::ObjectGroupId group_id;
//  PortableGroup::ObjectGroup_var
  object_group =
    this->object_group_manager_.create_object_group (type_id,               // in
                                                     this->domain_id_,      // in
                                                     the_criteria,          // in
                                                     group_id               // out
                                                     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId_var factory_id = 0;
  ACE_NEW_THROW_EX (factory_id,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  *factory_id <<= group_id;


  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  TAO_PG_Factory_Set factory_set;

  const CORBA::ULong factory_infos_count = factory_infos.length ();

  ACE_TRY
    {
      if (factory_infos_count > 0
          && membership_style == PortableGroup::MEMB_INF_CTRL)
        {
          this->populate_object_group (object_group.in (),
                                       type_id,
                                       factory_infos,
                                       minimum_number_members,
                                       factory_set
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;



          CORBA::ULong fcid = 0;

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            CORBA::Object::_nil ());


          if (this->factory_map_.bind (group_id, factory_set) != 0)
             ACE_TRY_THROW (PortableGroup::ObjectNotCreated ());
        }
    }
  ACE_CATCHANY
    {
      this->delete_object_i (factory_set,
                             1 /* Ignore exceptions */
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_group_manager_.destroy_object_group (group_id
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = factory_id._retn();

  return object_group._retn ();
}

void
TAO_PG_GenericFactory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  PortableGroup::ObjectGroupId group_id;

  if (factory_creation_id >>= group_id) // Extract the actual FactoryCreationId.
    {
      // Successfully extracted the FactoryCreationId.  Now find the
      // TAO_PG_Factory_Set corresponding to it.

      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

      // If no entry exists in the factory map, infrastructure
      // controlled membership was not used.
      TAO_PG_Factory_Map::ENTRY *entry = 0;
      if (this->factory_map_.find (group_id, entry) == 0)
        {
          TAO_PG_Factory_Set & factory_set = entry->int_id_;


          this->delete_object_i (factory_set,
                                 0  /* Do not ignore exceptions */
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (this->factory_map_.unbind (group_id) != 0)
            ACE_THROW (CORBA::INTERNAL ());

          // Destroy the object group entry.
          this->object_group_manager_.destroy_object_group (
            group_id
            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
  else
    ACE_THROW (PortableGroup::ObjectNotFound ());  // @@
                                                   //    CORBA::BAD_PARAM
                                                   //    instead?
}

void
TAO_PG_GenericFactory::delete_object_i (TAO_PG_Factory_Set & factory_set,
                                        CORBA::Boolean ignore_exceptions
                                        ACE_ENV_ARG_DECL)
{
  const size_t len = factory_set.size ();

  size_t ilen = len;
  for (size_t i = 0; i != len; ++i)
    {
      // Destroy the object group member in reverse order in case the
      // array list is only partially destroyed and another call to
      // GenericFactory::delete_object() occurs afterwards.
      --ilen;

      TAO_PG_Factory_Node & factory_node = factory_set[ilen];

      PortableGroup::GenericFactory_ptr factory =
        factory_node.factory_info.the_factory.in ();
      const PortableGroup::GenericFactory::FactoryCreationId & member_fcid =
        factory_node.factory_creation_id.in ();

      ACE_TRY
        {
          factory->delete_object (member_fcid
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Exceptions are generally only ignored when this
          // GenericFactory (not the one being invoked above) is
          // destroyed.  The idea is to allow the GenericFactory to be
          // destroyed regardless of whether or not all object group
          // members have been destroyed, and minimize the number of
          // object group members that have not been destroyed.
          if (!ignore_exceptions)
            ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;

      // Since GenericFactory::delete_object() can throw an exception,
      // decrease the size of the factory array incrementally since
      // some object group members may not have been destroyed yet.
      // Note that this size reduction is fast since no memory is
      // actually deallocated.
      factory_set.size (ilen);
    }
}

void
TAO_PG_GenericFactory::delete_member (
  PortableGroup::ObjectGroupId group_id,
  const PortableGroup::Location & location
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  // If no entry exists in the factory map, infrastructure
  // controlled membership was not used.
  TAO_PG_Factory_Map::ENTRY *entry = 0;
  if (this->factory_map_.find (group_id, entry) == 0)
    {
      TAO_PG_Factory_Set & factory_set = entry->int_id_;

      const size_t len = factory_set.size ();

      // Iterate through the factory_set until a location match
      // occurs.  If a location match occurs, the member was created
      // by the infrastructure, i.e. this GenericFactory
      // implementation.  If no location matches, the member was
      // created by the application, and no operation will be
      // performed.
      //
      // @todo This is linear search.  Change to use a container with
      //       better search times.
      for (size_t i = 0; i < len; ++i)
        {
          TAO_PG_Factory_Node & node = factory_set[i];
          PortableGroup::FactoryInfo & info = node.factory_info;

          if (info.the_location == location)
            {
              info.the_factory->delete_object (node.factory_creation_id.in ()
                                               ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // The member has been successfully deleted.  Reduce the
              // size of the factory_set accordingly.
              if (len > 1)
                {
                  // Move the last element to the location of the
                  // current one and reduce the size of the set by
                  // one.
                  const size_t new_len = len - 1;
                  node = factory_set[new_len]; // Memberwise copy
                  factory_set.size (new_len);
                }
              else
                {
                  // A copy isn't necessary if the last member was
                  // deleted.
                  factory_set.size (0);
                }

              return;
            }
        }
    }
}

void
TAO_PG_GenericFactory::poa (PortableServer::POA_ptr p)
{
  ACE_ASSERT (CORBA::is_nil (this->poa_.in ())
              && !CORBA::is_nil (p));

  this->poa_ = PortableServer::POA::_duplicate (p);
}

void
TAO_PG_GenericFactory::populate_object_group (
  PortableGroup::ObjectGroup_ptr object_group,
  const char * type_id,
  const PortableGroup::FactoryInfos & factory_infos,
  PortableGroup::MinimumNumberMembersValue minimum_number_members,
  TAO_PG_Factory_Set & factory_set
  ACE_ENV_ARG_DECL)
{
  CORBA::ULong factory_infos_count = factory_infos.length ();
  factory_set.size (factory_infos_count);

  for (CORBA::ULong j = 0; j < factory_infos_count; ++j)
    {
      TAO_PG_Factory_Node & factory_node = factory_set[j];

      const PortableGroup::FactoryInfo &factory_info = factory_infos[j];

      if (j < ACE_static_cast (CORBA::ULong, minimum_number_members))
        {
          PortableGroup::GenericFactory_ptr factory =
            factory_info.the_factory.in ();

          if (CORBA::is_nil (factory))
            {
              // @@ instead InvalidProperty?
              ACE_THROW (PortableGroup::NoFactory (factory_info.the_location,
                                                   type_id));
            }

          // Do not allow the PortableGroup::MemberAlreadyPresent
          // exception to be propagated to this scope.
          const CORBA::Boolean propagate_member_already_present = 0;

          factory_node.factory_creation_id =
            this->create_member (object_group,
                                 factory_info,
                                 type_id,
                                 propagate_member_already_present
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      factory_node.factory_info = factory_info;  // Memberwise copy
    }
}

void
TAO_PG_GenericFactory::process_criteria (
  const char * type_id,
  const PortableGroup::Criteria & criteria,
  PortableGroup::MembershipStyleValue & membership_style,
  PortableGroup::FactoriesValue & factory_infos,
  PortableGroup::InitialNumberMembersValue & initial_number_members,
  PortableGroup::MinimumNumberMembersValue & minimum_number_members
  ACE_ENV_ARG_DECL)
{
  // Get type-specific properties.
  PortableGroup::Properties_var props =
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Merge the given criteria with the type-specific criteria.
  TAO_PG::override_properties (criteria, props.inout ());

  PortableGroup::Criteria unmet_criteria;
  unmet_criteria.length (4);  // The four criteria understood by this
                              // method.

  // Unmet criteria count.
  CORBA::ULong uc = 0;

  PortableGroup::Name name (1);
  name.length (1);

  PortableGroup::Value value;

  // MembershipStyle
  name[0].id = CORBA::string_dup (PortableGroup::PG_MEMBERSHIP_STYLE);
  if (TAO_PG::get_property_value (name, props.in (), value)
      && (!(value >>= membership_style)
          || (membership_style != PortableGroup::MEMB_APP_CTRL
              && membership_style != PortableGroup::MEMB_INF_CTRL)))
    {
      ACE_THROW (PortableGroup::InvalidProperty (name, value));
    }

  // Factories
  const PortableGroup::FactoryInfos * factory_infos_tmp = 0;

  PortableGroup::Value value1;
  name[0].id = CORBA::string_dup (PortableGroup::PG_FACTORIES);
  if (TAO_PG::get_property_value (name, props.in (), value1)
      && !(value1 >>= factory_infos_tmp))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value1));
    }

  const CORBA::ULong factory_infos_count =
    (factory_infos_tmp == 0 ? 0 : factory_infos_tmp->length ());

  PortableGroup::Value value2;
  // InitialNumberMembers
  name[0].id =
    CORBA::string_dup (PortableGroup::PG_INITIAL_NUMBER_MEMBERS);
  if (TAO_PG::get_property_value (name, props.in (), value2)
      && !(value2 >>= initial_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value2));
    }

  if (membership_style == PortableGroup::MEMB_INF_CTRL)
    {
      // If the number of factories is less than the initial number of
      // members or the desired number of initial members cannot
      // possibly be created.

      if (factory_infos_count < ACE_static_cast (CORBA::ULong,
                                                 minimum_number_members))
        {
          unmet_criteria[uc].nam = name;
          unmet_criteria[uc++].val = value2;
        }
    }

  // MinimumNumberMembers
  PortableGroup::Value value3;
  name[0].id =
    CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");
  if (TAO_PG::get_property_value (name, props.in (), value3)
      && !(value3 >>= minimum_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value3));
    }

  // If the minimum number of members is less than the initial number
  // of members, the MinimumNumberMembers property is cannot be
  // initially met.
  //
  // @note This code is not part of the above "MEMB_INF_CTRL" criteria
  //       check since the "name" and "value" variables have been
  //       changed.
  if (membership_style == PortableGroup::MEMB_INF_CTRL)
    {
      if (minimum_number_members > initial_number_members
          || ACE_static_cast (CORBA::ULong,
                              minimum_number_members) > factory_infos_count)
        {
          unmet_criteria[uc].nam = name;
          unmet_criteria[uc++].val = value3;
        }
      else if (factory_infos_tmp != 0)
      {
        factory_infos.length (factory_infos_count);
        factory_infos = *factory_infos_tmp;
      }
    }

  if (uc > 0)
    {
      // Reduce the length of the unmet criteria sequence in an effort
      // to optimize the copying that will occur when the below
      // exception is thrown.  Reducing the length is fast since no
      // deallocations should occur.
      unmet_criteria.length (uc);

      ACE_THROW (PortableGroup::CannotMeetCriteria (unmet_criteria));
    }
}

void
TAO_PG_GenericFactory::check_minimum_number_members (
  PortableGroup::ObjectGroup_ptr object_group,
  PortableGroup::ObjectGroupId group_id,
  const char * type_id
  ACE_ENV_ARG_DECL)
{

  //TODO - Fix this code. The original implementation for the load balancer assumed
  //       that the factory-creation-id was the same as the object-group-id. This
  //       is not longer true. The find below is supposed to be a factory-creation-id.
{
  int _todo_fix_temporarily_disabled_code_;
}
  return;

#if 0
 // Check if we've dropped below the MinimumNumberMembers threshold.
  // If so, attempt to create enough new members to fill the gap.

  // If no entry exists in the factory map, infrastructure (this
  // GenericFactory implementation) controlled membership was not
  // used.
  TAO_PG_Factory_Map::ENTRY *entry = 0;
  if (this->factory_map_.find (group_id, entry) != 0)
    return;

  TAO_PG_Factory_Set & factory_set = entry->int_id_;

  PortableGroup::Properties_var props =
    this->property_manager_.get_properties (object_group
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableGroup::Name name (1);
  name.length (1);

  PortableGroup::Value value;

  // MinimumNumberMembers
  name[0].id =
    CORBA::string_dup (PG_MINIMUM_NUMBER_MEMBERS);

  PortableGroup::MinimumNumberMembersValue minimum_number_members;

  if (TAO_PG::get_property_value (name, props.in (), value))
    {
      if (!(value >>= minimum_number_members))
        {
          // This only occurs if extraction of the actual value from
          // the Any fails.  It shouldn't fail at this point.
          ACE_THROW (CORBA::INTERNAL ());
        }

      const CORBA::ULong count =
        this->object_group_manager_.member_count (object_group
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (count >= ACE_static_cast (CORBA::ULong, minimum_number_members))
        return;

      const CORBA::ULong gap =
        ACE_static_cast (CORBA::ULong,
                         minimum_number_members) - count;

      CORBA::ULong creation_count = 0;

      const size_t len = factory_set.size ();

      static const PortableGroup::GenericFactory::FactoryCreationId *
        nil_fcid = 0;

      for (size_t i = 0; i < len; ++i)
        {
          TAO_PG_Factory_Node & node = factory_set[i];

          PortableGroup::GenericFactory::FactoryCreationId * const tmp_fcid =
            node.factory_creation_id;

          // Check if the application supplied GenericFactory was
          // already invoked.
          if (tmp_fcid != nil_fcid)
            continue;

          ACE_TRY
            {
              const CORBA::Boolean propagate_member_already_present = 1;

              node.factory_creation_id =
                this->create_member (object_group,
                                     node.factory_info,
                                     type_id,
                                     propagate_member_already_present
                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ++creation_count;

              // Check if the MinimumNumberMembers threshold gap has
              // been filled.
              if (gap == creation_count)
                return;
            }
          ACE_CATCH (PortableGroup::MemberAlreadyPresent, ex)
            {
              // Ignore this exception and continue.
            }
          ACE_ENDTRY;
          ACE_CHECK;
        }

      // @todo If we get this far, and the MinimumNumberMembers
      //       threshold gap hasn't been filled, what do we do?  Throw
      //       a CORBA::TRANSIENT?
    }
#endif
}

PortableGroup::GenericFactory::FactoryCreationId *
TAO_PG_GenericFactory::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::FactoryInfo & factory_info,
    const char * type_id,
    const CORBA::Boolean propagate_member_already_present
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria,
                   PortableGroup::MemberAlreadyPresent))
{
  PortableGroup::GenericFactory::FactoryCreationId_var fcid;

  CORBA::Object_var member =
    factory_info.the_factory->create_object (type_id,
                                             factory_info.the_criteria,
                                             fcid.out ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TRY
    {
      // @@ Should an "_is_a()" be performed here?  While it
      //    appears to be the right thing to do, it can be
      //    expensive.
      //
      // Make sure an Object of the correct type was created.
      // It is possible that an object of the wrong type was
      // created if the type_id parameter does not match the
      // type of object the GenericFactory creates.
      CORBA::Boolean right_type_id =
        member->_is_a (type_id
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // @todo Strategize this -- e.g. strict type checking.
      if (!right_type_id)
        {
          // An Object of incorrect type was created.  Delete
          // it, and throw a NoFactory exception.
          factory_info.the_factory->delete_object (fcid.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_TRY_THROW (PortableGroup::NoFactory (factory_info.the_location,
                                                   type_id));
        }

      this->object_group_manager_._tao_add_member (
        object_group,
        factory_info.the_location,
        member.in (),
        type_id,
        propagate_member_already_present
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // If the member reference is not nil, then the factory
      // was successfully invoked.  Since an exception was
      // thrown, clean up the up created member.
      if (!CORBA::is_nil (member.in ()))
        {
          factory_info.the_factory->delete_object (fcid.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return fcid._retn ();
}
