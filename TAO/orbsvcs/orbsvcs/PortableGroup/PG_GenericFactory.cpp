#include "PG_GenericFactory.h"
#include "PG_MemberInfo.h"
#include "PG_ObjectGroupManager.h"
#include "PG_PropertyManager.h"
#include "PG_Property_Utils.h"
#include "PG_conf.h"

//#include "ace/Auto_Ptr.h"

ACE_RCSID (PortableGroup,
           PG_GenericFactory,
           "$Id$")


TAO_PG_GenericFactory::TAO_PG_GenericFactory (
  TAO_PG_ObjectGroupManager & object_group_manager,
  TAO_PG_PropertyManager & property_manager)
  : poa_ (),
    object_group_manager_ (object_group_manager),
    property_manager_ (property_manager),
    factory_map_ (TAO_PG_MAX_NUMBER_OF_OBJECT_GROUPS),
    next_fcid_ (0),
    lock_ ()
{
}

TAO_PG_GenericFactory::~TAO_PG_GenericFactory (void)
{

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
          ACE_CHECK;
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
  PortableGroup::Properties_var properties =
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableGroup::MembershipStyleValue membership_style =
    TAO_PG_MEMBERSHIP_STYLE;
  PortableGroup::FactoriesValue * factory_infos = 0;  // Memory owned
                                                      // by Any.
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
                          minimum_number_members
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::ULong factory_infos_count = factory_infos->length ();

  // If the number of factories is less than the initial number of
  // members, then the desired number of members cannot possibly be
  // created.
  if (factory_infos_count < initial_number_members)
    ACE_THROW_RETURN (PortableGroup::CannotMeetCriteria (),
                      CORBA::Object::_nil ());

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
          ACE_THROW_RETURN (PortableGroup::ObjectNotCreated (),
                            CORBA::Object::_nil ());
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
                                                     the_criteria
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (factory_infos_count > 0
      && membership_style == PortableGroup::MEMB_INF_CTRL)
    {
      this->populate_object_group (object_group.in (),
                                   oid.in (),
                                   type_id,
                                   *factory_infos
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }

  // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId * tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = tmp;

  *tmp <<= fcid;

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
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  CORBA::ULong fcid = 0;

  if (factory_creation_id >>= fcid) // Extract the actual FactoryCreationId.
    {
      // Successfully extracted the FactoryCreationId.  Now find the
      // TAO_PG_Factory_Set corresponding to it.

      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

      TAO_PG_Factory_Map::ENTRY *entry = 0;
      if (this->factory_map_.find (fcid, entry) == 0)
        {
          TAO_PG_Factory_Set & factory_set = entry->int_id_;

          this->delete_object_i (factory_set,
                                 0  /* Do not ignore exceptions */
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        ACE_THROW (PortableGroup::ObjectNotFound ());
    }
  else
    ACE_THROW (PortableGroup::ObjectNotFound ());  // @@
                                                   //    CORBA::BAD_PARAM
                                                   //    instead?

  // The ObjectId for the newly created object group is comprised
  // solely of the FactoryCreationId.
  PortableServer::ObjectId_var oid;
  this->get_ObjectId (fcid, oid.out ());

  // Destroy the object group entry.
  this->object_group_manager_.destroy_object_group (
    oid.in ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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
        factory_node.factory.in ();
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
TAO_PG_GenericFactory::poa (PortableServer::POA_ptr p)
{
  this->poa_ = PortableServer::POA::_duplicate (p);
}

void
TAO_PG_GenericFactory::populate_object_group (
  PortableGroup::ObjectGroup_ptr object_group,
  const PortableServer::ObjectId & oid,
  const char * type_id,
  const PortableGroup::FactoryInfos & factory_infos
  ACE_ENV_ARG_DECL)
{
  CORBA::ULong factory_infos_count = factory_infos.length ();
  TAO_PG_Factory_Set factory_set (factory_infos_count);
  for (CORBA::ULong j = 0; j < factory_infos_count; ++j)
    {
      const PortableGroup::FactoryInfo &factory_info =
        factory_infos[j];

      PortableGroup::GenericFactory_ptr factory =
        factory_info.the_factory.in ();

      if (CORBA::is_nil (factory))
        ACE_THROW (PortableGroup::NoFactory ());  // @@
                                                  // InvalidProperty
                                                  // instead?

      PortableGroup::GenericFactory::FactoryCreationId_var
        member_fcid;

      CORBA::Object_var member =
        factory->create_object (type_id,
                                factory_info.the_criteria,
                                member_fcid.out ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_TRY
        {
          // @@ Should an "_is_a()" be performed here?  While it
          //    appears to be the right thing to do, it can be expensive.
          //
          // Make sure an Object of the correct type was created.  It
          // is possible that an object of the wrong type was created
          // if the type_id parameter does not match the type of
          // object the GenericFactory creates.
          CORBA::Boolean right_type_id =
            member->_is_a (type_id
                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // @todo Strategize this -- e.g. strict type checking.
          if (!right_type_id)
            {
              // An Object of incorrect type was created.  Delete it, and
              // throw a NoFactory exception.
              factory->delete_object (member_fcid.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_TRY_THROW (PortableGroup::NoFactory ());
            }

          TAO_PG_Factory_Node factory_node;
          factory_node.factory =
            PortableGroup::GenericFactory::_duplicate (factory);
          factory_node.factory_creation_id = member_fcid;

          factory_set[j] = factory_node;

          this->object_group_manager_.add_member (object_group,
                            factory_info.the_location,
                            member.in ()
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // If the member reference is not nil, then the factory was
          // successfully invoked.  Since an exception was thrown,
          // clean up the up created member.
          if (!CORBA::is_nil (member.in ()))
            {
              factory->delete_object (member_fcid.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          this->object_group_manager_.destroy_object_group (
            oid
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

  if (this->factory_map_.bind (fcid, factory_set) != 0)
    ACE_THROW ();
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
  PortableGroup::FactoriesValue * factory_infos,
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

  PortableGroup::Name name (1);
  name.length (1);

  PortableGroup::Value value;

  name[0].id = CORBA::string_dup ("org.omg.pg.MembershipStyle");

  if (TAO_PG::get_property_value (name, props.in (), value)
      && (!(value >>= membership_style)
          || (membership_style != PortableGroup::MEMB_APP_CTRL
              && membership_style != PortableGroup::MEMB_INF_CTRL)))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value));
    }

  name[0].id = CORBA::string_dup ("org.omg.pg.Factories");
  if (TAO_PG::get_property_value (name, props.in (), value)
      && !(value >>= factory_infos))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value));
    }

  name[0].id = CORBA::string_dup ("org.omg.pg.InitialNumberMembers");
  if (TAO_PG::get_property_value (name, props.in (), value)
      && !(value >>= initial_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value));
    }

  name[0].id = CORBA::string_dup ("org.omg.pg.MinimumNumberMembers");
  if (TAO_PG::get_property_value (name, props.in (), value)
      && !(value >>= minimum_number_members))
    {
      // This only occurs if extraction of the actual value from the
      // Any fails.
      ACE_THROW (PortableGroup::InvalidProperty (name, value));
    }
}

// #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// template class auto_ptr<TAO_PG_Factory_Node>;

// #  if defined (ACE_LACKS_AUTO_PTR) \
//       || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
//            && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
// template class ACE_Auto_Basic_Ptr<TAO_PG_Factory_Node>;
// #  endif  /* ACE_LACKS_AUTO_PTR */

// #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// #pragma instantiate auto_ptr<TAO_PG_Factory_Node>

// #  if defined (ACE_LACKS_AUTO_PTR) \
//       || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
//            && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
// #pragma instantiate ACE_Auto_Basic_Ptr<TAO_PG_Factory_Node>

// #  endif  /* ACE_LACKS_AUTO_PTR */

// #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
