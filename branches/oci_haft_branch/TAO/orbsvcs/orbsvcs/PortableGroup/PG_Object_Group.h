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
 *  @@ however at the moment GenericFactory, ObjectGroupManager, and FT_ReplicationManager
 *  @@ have parallel collections of object group information.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PG_OBJECT_GROUP_H_
#define TAO_PG_OBJECT_GROUP_H_
#include /**/ <ace/pre.h>
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////
// Classes declared in this header
namespace TAO
{
  class PG_Object_Group;
}

/////////////////////////////////
// Includes needed by this header
#include <orbsvcs/PortableGroupC.h>
#include <tao/PortableServer/PortableServer.h>
#include <ace/Hash_Map_Manager_T.h>
#include "PG_Location_Hash.h"
#include "PG_Location_Equal_To.h"


/////////////////////
// Forward references

namespace TAO
{
  /**
   */
  class TAO_PortableGroup_Export PG_Object_Group
  {

    // Information about an object group member
    struct MemberInfo
    {
      /// Reference to the object.
      CORBA::Object_var member_;
      /// The factory that was used to create this object
      /// nil if application created.
      PortableGroup::GenericFactory_var factory_;
      /// FactoryCreationId assigned to the member. Empty if application created
      PortableGroup::GenericFactory::FactoryCreationId factory_id_;
      /// Construct an application-supplied member.
      MemberInfo (CORBA::Object_var member);
      /// Construct a infrastructure-created member.
      MemberInfo (
        CORBA::Object_var member, 
        PortableGroup::GenericFactory_var factory, 
        PortableGroup::GenericFactory::FactoryCreationId factory_id);
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
    
  public:
    /// Constructor
    PG_Object_Group ();

    /// Constructor
    PG_Object_Group (
      PortableGroup::ObjectGroupId oid,
      const char * type_id,
      PortableGroup::Criteria the_criteria);

    /// Destructor
    ~PG_Object_Group ();

    
    /////////////////
    // public methods

  public:


    void set_oid (PortableGroup::ObjectGroupId oid);
    void set_typeid (PortableGroup::TypeId type_id);
    void set_properties (PortableGroup::Criteria the_criteria);
    /**
     *
     * @ param reference the new IOGR for this group.
     * @ distribute bool if true, distribute IOGR to all members
     */
    void set_reference (PortableGroup::ObjectGroup_ptr reference, int distribute);

    PortableGroup::ObjectGroup_ptr reference()const;

    void set_membership_style (PortableGroup::MembershipStyleValue style);
    PortableGroup::MembershipStyleValue membership_style () const;

    void initial_number_replicas (PortableGroup::InitialNumberReplicasValue count);
    PortableGroup::InitialNumberReplicasValue initial_number_replicas () const;

    void set_minimum_number_replicas (PortableGroup::MinimumNumberReplicasValue count);
    PortableGroup::MinimumNumberReplicasValue minimum_number_replicas ()const;

    void set_group_specific_factories (const PortableGroup::FactoryInfos & infos);
    /**
     * Note the caller receives a copy of the factoryinfos in the result argument.
     * inefficient, but thread safe.
     */
    void group_specific_factories (PortableGroup::FactoryInfos & result) const;

    // Note: primary location is a concept from FT CORBA.
    // It doesn't hurt other PortableGroup-based services to 
    // have these two metods and the underlying member.
    void set_primary_location (PortableGroup::Location & primary_location_);
    const PortableGroup::Location & primary_location() const;

    /**
     * returns a duplicate
     * caller must release
     */
    PortableGroup::TypeId get_type_id ()const;

    void get_properties (PortableGroup::Properties_var & result) const;

    /////////////////////////////////////////////
    // Applicable ObjectGroupManager(OGM) methods
    // The following methods in OGM all have an object group as the
    // first parameter.  The OGM should implement these methods by finding
    // the corresponding TAO_PG_Object_Group and delegating to these methods.
#ifdef NOT_IMPLEMENTED
    void create_member (
        const PortableGroup::Location & the_location,
        const char * type_id,
        const PortableGroup::Criteria & the_criteria
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableGroup::MemberAlreadyPresent,
        PortableGroup::ObjectNotCreated,
        PortableGroup::InvalidCriteria,
        PortableGroup::CannotMeetCriteria));
#endif

    void add_member (
        const PortableGroup::Location & the_location,
        CORBA::Object_ptr member
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableGroup::MemberAlreadyPresent,
        PortableGroup::ObjectNotAdded));

#ifdef NOT_IMPLEMENTED
    void remove_member (
        const PortableGroup::Location & the_location
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableGroup::MemberNotFound));
#endif

#ifdef NOT_IMPLEMENTED
    void locations_of_members (
        PortableGroup::Locations & locations
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
#endif

    PortableGroup::ObjectGroupId  get_object_group_id () const;

#ifdef NOT_IMPLEMENTED
    CORBA::Object_ptr get_member_ref (
        const PortableGroup::Location & loc
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableGroup::MemberNotFound));
#endif

#ifdef NOT_IMPLEMENTED
    void set_properties_dynamically (
        const PortableGroup::Properties & overrides
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableGroup::InvalidProperty,
                       PortableGroup::UnsupportedProperty));

#endif

    /////////////////////////
    // Implementation methods
  private:

    /////////////////////////
    // Forbidden methods
  private:
    PG_Object_Group (const PG_Object_Group & rhs);
    PG_Object_Group & operator = (const PG_Object_Group & rhs);
  

    /////////////////
    // Static Methods
  public:
    /**
     * Set the poa to be used to manage object groups
     * Note: this is NOT the poa used to create object group members.
     * @param poa a reference to a var to avoid reference count confusion.
     */
    void set_poa (PortableServer::POA_var & poa);

    ///////////////
    // Static Data
  private:
    /**
     * The POA used to manage object groups
     */
    static PortableServer::POA_var poa_;


    ///////////////
    // Data Members
  private:

    /**
     * Protect internal state.
     * Implementation methods should assume the mutex is
     * locked if necessary.
     */
    ACE_SYNCH_MUTEX internals_;
    typedef ACE_Guard<ACE_SYNCH_MUTEX> InternalGuard;

    ACE_CString role_;
    PortableGroup::TypeId type_id_;
    PortableGroup::ObjectGroupId group_id_;
    /**
     * the reference (IOGR) to this group
     */
    PortableGroup::ObjectGroup_var reference_;

    /**
     * The CORBA object id assigned to this object group
     */
    PortableServer::ObjectId_var object_id_;

    // group members
    MemberMap members_;

    PortableGroup::Location primary_location_;

    // Cached property information

    PortableGroup::MembershipStyleValue membership_style_;
    PortableGroup::InitialNumberReplicasValue initial_number_replicas_;
    PortableGroup::MinimumNumberReplicasValue minimum_number_replicas_;
    PortableGroup::FactoryInfos group_specific_factories_;

    // Miscellaneous properties passed to create_object when this group
    // was initially created.  To be used to create new replicas.
    PortableGroup::Properties properties_;

  };
} // namespace TAO

#include /**/ <ace/post.h>

#endif // TAO_PG_OBJECT_GROUP_H_
