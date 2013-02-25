// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Object_Group.h
 *
 *  $Id$
 *
 *  Manage all information related to an object group.
 *  @@ Note: the above description is optimistic.  The hope is to eventually
 *  @@ consolidate all information related to an object group into this object.
 *  @@ however at the moment GenericFactory, ObjectGroupManager, and
 *  FT_ReplicationManager have parallel collections of object group
 *  information.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PG_OBJECT_GROUP_H_
#define TAO_PG_OBJECT_GROUP_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/////////////////////////////////
// Includes needed by this header
#include "orbsvcs/PortableGroup/PG_Property_Set.h"
#include "orbsvcs/PortableGroup/PG_Location_Hash.h"
#include "orbsvcs/PortableGroup/PG_Location_Equal_To.h"
#include "orbsvcs/PortableGroup/PG_Object_Group_Manipulator.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/ACE.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/////////////////////
// Forward references

namespace TAO_IOP
{
  class TAO_IOR_Property;
}

////////////////
// Class declarations
namespace TAO
{
  /**
   */
  class TAO_PortableGroup_Export PG_Object_Group
  {

  protected:

    // Information about an object group member
    struct MemberInfo
    {
      /// Reference to the member.
      CORBA::Object_var member_;

      /// The factory that was used to create this object
      /// nil if application created.
      PortableGroup::GenericFactory_var factory_;

      /// FactoryCreationId assigned to the member. Empty if application created
      PortableGroup::GenericFactory::FactoryCreationId factory_id_;

      /// Location where this member exists
      PortableGroup::Location location_;

      /// TRUE if this is primary member
      CORBA::Boolean is_primary_;


      ///////////////
      // Methods

      /// Construct an application-supplied member.
      MemberInfo (CORBA::Object_ptr member, const PortableGroup::Location & location);

      /// Construct a infrastructure-created member.
      MemberInfo (
        CORBA::Object_ptr member,
        const PortableGroup::Location & location,
        PortableGroup::GenericFactory_ptr factory,
        PortableGroup::GenericFactory::FactoryCreationId factory_id);

      /// Destructor
      ~MemberInfo();
    };

    typedef TAO_SYNCH_MUTEX MemberMapMutex;
    typedef ACE_Hash_Map_Manager_Ex <
      PortableGroup::Location,
      MemberInfo *,
      TAO_PG_Location_Hash,
      TAO_PG_Location_Equal_To,
      MemberMapMutex>  MemberMap;
    typedef ACE_Hash_Map_Entry <PortableGroup::Location, MemberInfo *> MemberMap_Entry;
    typedef ACE_Hash_Map_Iterator_Ex <
      PortableGroup::Location,
      MemberInfo *,
      TAO_PG_Location_Hash,
      TAO_PG_Location_Equal_To,
      MemberMapMutex> MemberMap_Iterator;

    /////////////////////
    // Construct/Destruct
  public:
    /**
     * @@TODO DOC
     */
    PG_Object_Group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      const TAO::PG_Property_Set_var & type_properties);
    /**
     * This constructor is to be used for initialization when
     * reading the object group from a stream.
     */
    PG_Object_Group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator);

    /// Destructor
    virtual ~PG_Object_Group ();

    /////////////////
    // public methods

  public:
    /// return a duplicated reference to this group (IOGR)
    PortableGroup::ObjectGroup_ptr reference() const;

    /**
     * Note the caller receives a copy of the factoryinfos in the result argument.
     * inefficient, but thread safe.
     */
    void get_group_specific_factories (PortableGroup::FactoryInfos & result) const;

    /**
     * get location of primary member
     */
    virtual const PortableGroup::Location & get_primary_location();

    /**
     * returns a duplicate
     * caller must release
     */
    PortableGroup::TypeId get_type_id ()const;


    /**
     * @@TODO DOC
     */
    PortableGroup::MembershipStyleValue get_membership_style() const;

    /**
     * @@TODO DOC
     */
    PortableGroup::MinimumNumberMembersValue get_minimum_number_members () const;

    /**
     * @@TODO DOC
     */
    PortableGroup::InitialNumberMembersValue get_initial_number_members () const;



    /**
     * @@TODO DOC
     */
    void set_properties_dynamically (
        const PortableGroup::Properties & overrides);

    /**
     * @@TODO DOC
     */
    void get_properties (PortableGroup::Properties_var & result) const;

    /**
     * @@TODO DOC
     */
    virtual PortableGroup::ObjectGroupId  get_object_group_id () const;

    /**
     * Add a new member to the group.
     * @param the_location the location for the new member
     * @param member the member to be added
     */
    virtual void add_member (
        const PortableGroup::Location & the_location,
        CORBA::Object_ptr member);

    /**
     * set the replica at the given location to be primary.
     * Note: This should return void and throw FT::PrimaryNotSet
     * but to avoid dependancies between PortableGroup and FaultTolerance
     * it returns a boolean result.  A false return means caller should
     * throw FT::PrimaryNot_Set.
     */
    virtual int set_primary_member (
      TAO_IOP::TAO_IOR_Property * prop,
      const PortableGroup::Location & the_location);

    /**
     * @@TODO DOC
     */
    virtual void remove_member (
        const PortableGroup::Location & the_location);


    /**
     * @@TODO DOC
     */
    virtual void create_member (
        const PortableGroup::Location & the_location,
        const char * type_id,
        const PortableGroup::Criteria & the_criteria);

    /**
     * @@TODO DOC
     */
    virtual PortableGroup::Locations * locations_of_members (void);

    /**
     * @@TODO DOC
     */
    virtual CORBA::Object_ptr get_member_reference (
        const PortableGroup::Location & the_location);


    /**
     * @@TODO DOC
     */
    virtual void initial_populate (void);

    /**
     * @@TODO DOC
     */
    virtual void minimum_populate (void);


    /**
     * @@TODO DOC
     */
    virtual int has_member_at (const PortableGroup::Location & location );


    /**
     * Tell the object group that it should distribute updates to the object
     * group state.
     */
    virtual void distribute (int value);

    virtual void set_name (const char* group_name);

    virtual const char* get_name (void);

    /////////////////////////
    // Implementation methods
  private:

    int increment_version ();

    void distribute_iogr (void);

    void create_members (size_t count);

  protected:

    virtual PortableGroup::ObjectGroup_ptr add_member_to_iogr(CORBA::Object_ptr member);

    void clear_members_map (void);

    /////////////////////////
    // Forbidden methods
  private:
    PG_Object_Group ();
    PG_Object_Group (const PG_Object_Group & rhs);
    PG_Object_Group & operator = (const PG_Object_Group & rhs);


    /////////////////
    // Static Methods
  public:

    ///////////////
    // Static Data
  private:

    ///////////////
    // Data Members
  private:

    /**
     * Protect internal state.
     */
    mutable TAO_SYNCH_MUTEX internals_;

  protected:
    CORBA::ORB_var orb_;

  private:

    /// Where to find the factories for replicas.
    PortableGroup::FactoryRegistry_var factory_registry_;

  protected:

    // The object group manipulator
    TAO::PG_Object_Group_Manipulator & manipulator_;

    /// boolean true if updates should be distributed
    int distribute_;

    /// boolean true if empty group
    int empty_;

    ACE_CString role_;


    PortableGroup::TypeId_var type_id_;

    /**
     * the GroupTaggedComponent that defines this group
     * contains:
     *   GIOP::Version component_version;
     *   TAO::String_Manager group_domain_id;
     *   PortableGroup::ObjectGroupId object_group_id;
     *   PortableGroup::ObjectGroupRefVersion object_group_ref_version;
     */
    PortableGroup::TagGroupTaggedComponent tagged_component_;

    /**
     * the reference (IOGR) to this group
     */
    PortableGroup::ObjectGroup_var reference_;

    /**
     * The CORBA object id assigned to this object group
     */
    PortableServer::ObjectId_var object_id_;

    /**
     * an optional attribute of the object group which is a string
     * name that is assigned to the object group by the creator.
     */
    char* group_name_;

    // group members
    MemberMap members_;

    PortableGroup::Location primary_location_;

    // Miscellaneous properties passed to create_object when this group
    // was initially created.  To be used to create new members.
    TAO::PG_Property_Set properties_;

    // Cached property information

    PortableGroup::InitialNumberMembersValue initial_number_members_;
    PortableGroup::MinimumNumberMembersValue minimum_number_members_;
    PortableGroup::FactoryInfos group_specific_factories_;

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_PG_OBJECT_GROUP_H_
