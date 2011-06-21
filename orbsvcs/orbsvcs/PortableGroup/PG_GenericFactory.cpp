// $Id$

#include "orbsvcs/PortableGroup/PG_GenericFactory.h"
#include "orbsvcs/PortableGroup/PG_MemberInfo.h"
#include "orbsvcs/PortableGroup/PG_ObjectGroupManager.h"
#include "orbsvcs/PortableGroup/PG_PropertyManager.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "ace/OS_NS_stdio.h"

#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_GenericFactory::TAO_PG_GenericFactory (
  TAO_PG_ObjectGroupManager & object_group_manager,
  TAO_PG_PropertyManager & property_manager)
  : poa_ (),
    object_group_manager_ (object_group_manager),
    property_manager_ (property_manager),
    factory_map_ (TAO_PG_MAX_OBJECT_GROUPS),
    next_fcid_ (0),
    lock_ ()
{
  this->object_group_manager_.generic_factory (this);
}

TAO_PG_GenericFactory::~TAO_PG_GenericFactory (void)
{

  TAO_PG_Factory_Map::iterator end = this->factory_map_.end ();
  for (TAO_PG_Factory_Map::iterator i = this->factory_map_.begin ();
       i != end;
       ++i)
    {
      TAO_PG_Factory_Set & factory_set = (*i).int_id_;

      try
        {
          this->delete_object_i (factory_set,
                                 1 /* Ignore exceptions */);
        }
      catch (const CORBA::Exception&)
        {
          // Ignore all exceptions.
        }
    }

  (void) this->factory_map_.close ();
}

CORBA::Object_ptr
TAO_PG_GenericFactory::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  PortableGroup::Properties_var properties =
    this->property_manager_.get_type_properties (type_id);

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
                          initial_number_members,
                          minimum_number_members);

  CORBA::ULong fcid = 0;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      CORBA::Object::_nil ());

    // Start out with an initial value.
    fcid = this->next_fcid_;

    // Loop until a free FactoryCreationId is found, being careful to
    // search through the range of FactoryCreationIds only once.
    while (this->factory_map_.find (this->next_fcid_) == 0)
      {
        this->next_fcid_++;

        // If this is true, then no FactoryCreationIds are available.
        // This is highly unlikely since TAO implements a
        // FactoryCreationId as a 32 bit unsigned integer, meaning
        // that over 4 billion object groups are being managed by this
        // generic factory!
        if (this->next_fcid_ == fcid)
          throw PortableGroup::ObjectNotCreated ();
      }

    // Just in case this->next_fcid_ was modified in the above search,
    // reassign the value.
    fcid = this->next_fcid_;
  }

  // The ObjectId for the newly created object group is comprised
  // solely of the FactoryCreationId.
  PortableServer::ObjectId_var oid;
  this->get_ObjectId (fcid, oid.out ());

  PortableGroup::ObjectGroup_var object_group =
    this->object_group_manager_.create_object_group (fcid,
                                                     oid.in (),
                                                     type_id,
                                                     the_criteria);

  TAO_PG_Factory_Set factory_set;

  const CORBA::ULong factory_infos_count = factory_infos.length ();

  try
    {
      if (factory_infos_count > 0
          && membership_style == PortableGroup::MEMB_INF_CTRL)
        {
          this->populate_object_group (object_group.in (),
                                       type_id,
                                       factory_infos,
                                       initial_number_members,
                                       factory_set);

          if (this->factory_map_.bind (fcid, factory_set) != 0)
            throw PortableGroup::ObjectNotCreated ();

        }

      // Allocate a new FactoryCreationId for use as an "out" parameter.
      PortableGroup::GenericFactory::FactoryCreationId * tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        PortableGroup::GenericFactory::FactoryCreationId,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      factory_creation_id = tmp;

      *tmp <<= fcid;
    }
  catch (const CORBA::Exception&)
    {
      this->delete_object_i (factory_set,
                             1 /* Ignore exceptions */);

      this->object_group_manager_.destroy_object_group (oid.in ());

      throw;
    }

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      CORBA::Object::_nil ());

    // Object group was successfully created.  Increment the next
    // FactoryCreationId in preparation for the next object group.
    this->next_fcid_++;
  }

  return object_group._retn ();
}

void
TAO_PG_GenericFactory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id)
{
  CORBA::ULong fcid = 0;

  if (factory_creation_id >>= fcid) // Extract the actual FactoryCreationId.
    {
      // Successfully extracted the FactoryCreationId.  Now find the
      // TAO_PG_Factory_Set corresponding to it.

      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

      // If no entry exists in the factory map, infrastructure
      // controlled membership was not used.
      TAO_PG_Factory_Map::ENTRY *entry = 0;
      if (this->factory_map_.find (fcid, entry) == 0)
        {
          TAO_PG_Factory_Set & factory_set = entry->int_id_;

          this->delete_object_i (factory_set,
                                 0  /* Do not ignore exceptions */);

          if (this->factory_map_.unbind (fcid) != 0)
            throw CORBA::INTERNAL ();
        }
    }
  else
    throw PortableGroup::ObjectNotFound ();  // @@
                                                   //    CORBA::BAD_PARAM
                                                   //    instead?

  // The ObjectId for the newly created object group is comprised
  // solely of the FactoryCreationId.
  PortableServer::ObjectId_var oid;
  this->get_ObjectId (fcid, oid.out ());

  // Destroy the object group entry.
  this->object_group_manager_.destroy_object_group (
    oid.in ());
}

void
TAO_PG_GenericFactory::delete_object_i (TAO_PG_Factory_Set & factory_set,
                                        CORBA::Boolean ignore_exceptions)
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

      try
        {
          factory->delete_object (member_fcid);
        }
      catch (const CORBA::Exception&)
        {
          // Exceptions are generally only ignored when this
          // GenericFactory (not the one being invoked above) is
          // destroyed.  The idea is to allow the GenericFactory to be
          // destroyed regardless of whether or not all object group
          // members have been destroyed, and minimize the number of
          // object group members that have not been destroyed.
          if (!ignore_exceptions)
            throw;
        }

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
  CORBA::ULong group_id,
  const PortableGroup::Location & location)
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
              try {
                info.the_factory->delete_object (node.factory_creation_id.in ());
              }
              catch (const CORBA::Exception& ex)
              {
                 // Common failure is CORBA::TRANSIENT due to remote factory
                 // is shutdown.
                 // We just ignore the exception and continue.
                 if (TAO_debug_level > 0)
                 {
                   ex._tao_print_exception ("TAO_PG_GenericFactory::delete_member \n");
                 }
              }

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
  PortableGroup::InitialNumberMembersValue initial_number_members,
  TAO_PG_Factory_Set & factory_set)
{
  CORBA::ULong factory_infos_count = factory_infos.length ();
  factory_set.size (factory_infos_count);

  for (CORBA::ULong j = 0; j < factory_infos_count; ++j)
    {
      TAO_PG_Factory_Node & factory_node = factory_set[j];

      const PortableGroup::FactoryInfo &factory_info = factory_infos[j];

      if (j < static_cast<CORBA::ULong> (initial_number_members))
        {
          PortableGroup::GenericFactory_ptr factory =
            factory_info.the_factory.in ();

          if (CORBA::is_nil (factory))
            {
              // @@ instead InvalidProperty?
              throw PortableGroup::NoFactory (
                factory_info.the_location,
                type_id);
            }

          // Do not allow the PortableGroup::MemberAlreadyPresent
          // exception to be propagated to this scope.
          const CORBA::Boolean propagate_member_already_present = 0;

          factory_node.factory_creation_id =
            this->create_member (object_group,
                                 factory_info,
                                 type_id,
                                 propagate_member_already_present);
        }

      factory_node.factory_info = factory_info;  // Memberwise copy
    }
}

void
TAO_PG_GenericFactory::get_ObjectId (
  CORBA::ULong fcid,
  PortableServer::ObjectId_out oid)
{
  // Since the POA used by the LoadManager uses the NON_RETAIN
  // policy, explicitly choose an ObjectId that is unique to a given
  // type.

  // Make the ObjectId be the next value of the number of types that
  // have been registered with the LoadManager.  For example, if two
  // types of objects have been registered with the LoadManager, then
  // the ObjectId for the object currently being registered will be
  // "3" since the object will be the third type of object registered
  // with the LoadManager.  Previously used values will not be reused
  // to ensure that a ServantLocator does not inadvertently return a
  // reference to an object that had a previously used ObjectId.
  // Specifcally, the numerical value used for the ObjectId increases
  // monotonically.

  // 4294967295UL -- Largest 32 bit unsigned integer
  // 123456789012 -- 10 digits
  //                + 2 for "UL"  (unnecessary, but let's be safe)
  //                + 1 for null terminator
  //                + 1 for good luck. :-)
  const size_t MAX_OID_LEN = 14;

  char oid_str[MAX_OID_LEN] = { 0 };
  ACE_OS::sprintf (oid_str,
                   "%ul",
                   fcid);

  oid = PortableServer::string_to_ObjectId (oid_str);
}

void
TAO_PG_GenericFactory::process_criteria (
  const char * type_id,
  const PortableGroup::Criteria & criteria,
  PortableGroup::MembershipStyleValue & membership_style,
  PortableGroup::FactoriesValue & factory_infos,
  PortableGroup::InitialNumberMembersValue & initial_number_members,
  PortableGroup::MinimumNumberMembersValue & minimum_number_members)
{
  // Get type-specific properties.
  PortableGroup::Properties_var props =
    this->property_manager_.get_type_properties (type_id);

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
  PortableGroup::Value value1;
  PortableGroup::Value value2;
  PortableGroup::Value value3;

  // MembershipStyle
  name[0].id = CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");
  if (TAO_PG::get_property_value (name, props.in (), value)
      && (!(value >>= membership_style)
          || (membership_style != PortableGroup::MEMB_APP_CTRL
              && membership_style != PortableGroup::MEMB_INF_CTRL)))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      throw PortableGroup::InvalidProperty (name, value);
    }

  // Factories
  const PortableGroup::FactoryInfos * factory_infos_tmp = 0;
  name[0].id = CORBA::string_dup ("org.omg.PortableGroup.Factories");
  if (TAO_PG::get_property_value (name, props.in (), value1)
      && !(value1 >>= factory_infos_tmp))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      throw PortableGroup::InvalidProperty (name, value1);
    }

  const CORBA::ULong factory_infos_count =
    (factory_infos_tmp == 0 ? 0 : factory_infos_tmp->length ());

  // InitialNumberMembers
  name[0].id =
    CORBA::string_dup ("org.omg.PortableGroup.InitialNumberMembers");
  if (TAO_PG::get_property_value (name, props.in (), value2)
      && !(value2 >>= initial_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      throw PortableGroup::InvalidProperty (name, value2);
    }

  if (membership_style == PortableGroup::MEMB_INF_CTRL)
    {
      // If the number of factories is less than the initial number of
      // members or the desired number of initial members cannot
      // possibly be created.

      if (factory_infos_count < static_cast<CORBA::ULong> (initial_number_members))
        {
          unmet_criteria[uc].nam = name;
          unmet_criteria[uc++].val = value2;
        }
    }

  // MinimumNumberMembers
  name[0].id =
    CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");
  if (TAO_PG::get_property_value (name, props.in (), value3)
      && !(value3 >>= minimum_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      throw PortableGroup::InvalidProperty (name, value3);
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
      if (minimum_number_members < initial_number_members
          || static_cast<CORBA::ULong> (minimum_number_members) > factory_infos_count)
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

      throw PortableGroup::CannotMeetCriteria (unmet_criteria);
    }
}

void
TAO_PG_GenericFactory::check_minimum_number_members (
  PortableGroup::ObjectGroup_ptr object_group,
  CORBA::ULong group_id,
  const char * type_id)
{
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
    this->property_manager_.get_properties (object_group);

  PortableGroup::Name name (1);
  name.length (1);

  PortableGroup::Value value;

  // MinimumNumberMembers
  name[0].id =
    CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");

  PortableGroup::MinimumNumberMembersValue minimum_number_members;

  if (TAO_PG::get_property_value (name, props.in (), value))
    {
      if (!(value >>= minimum_number_members))
        {
          // This only occurs if extraction of the actual value from
          // the Any fails.  It shouldn't fail at this point.
          throw CORBA::INTERNAL ();
        }

      const CORBA::ULong count =
        this->object_group_manager_.member_count (object_group);

      if (count >= static_cast<CORBA::ULong> (minimum_number_members))
        return;

      const CORBA::ULong gap =
        static_cast<CORBA::ULong> (minimum_number_members) - count;

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

          try
            {
              const CORBA::Boolean propagate_member_already_present = 1;

              node.factory_creation_id =
                this->create_member (object_group,
                                     node.factory_info,
                                     type_id,
                                     propagate_member_already_present);

              ++creation_count;

              // Check if the MinimumNumberMembers threshold gap has
              // been filled.
              if (gap == creation_count)
                return;
            }
          catch (const PortableGroup::MemberAlreadyPresent&)
            {
              // Ignore this exception and continue.
            }
        }

      // @todo If we get this far, and the MinimumNumberMembers
      //       threshold gap hasn't been filled, what do we do?  Throw
      //       a CORBA::TRANSIENT?
    }
}

PortableGroup::GenericFactory::FactoryCreationId *
TAO_PG_GenericFactory::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::FactoryInfo & factory_info,
    const char * type_id,
    const CORBA::Boolean propagate_member_already_present)
{
  PortableGroup::GenericFactory::FactoryCreationId_var fcid;

  CORBA::Object_var member =
    factory_info.the_factory->create_object (type_id,
                                             factory_info.the_criteria,
                                             fcid.out ());

  try
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
        member->_is_a (type_id);

      // @todo Strategize this -- e.g. strict type checking.
      if (!right_type_id)
        {
          // An Object of incorrect type was created.  Delete
          // it, and throw a NoFactory exception.
          factory_info.the_factory->delete_object (fcid.in ());

          throw PortableGroup::NoFactory (factory_info.the_location, type_id);
        }

      this->object_group_manager_._tao_add_member (
        object_group,
        factory_info.the_location,
        member.in (),
        type_id,
        propagate_member_already_present);
    }
  catch (const CORBA::Exception&)
    {
      // If the member reference is not nil, then the factory
      // was successfully invoked.  Since an exception was
      // thrown, clean up the up created member.
      if (!CORBA::is_nil (member.in ()))
        {
          factory_info.the_factory->delete_object (fcid.in ());
        }

      throw;
    }

  return fcid._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
